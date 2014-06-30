#include "iqormsqlobjectsmodeldatasource.h"
#include "iqormsqlobjectdatasource.h"
#include "iqormfilter.h"
#include "iqormgroupfilter.h"
#include "iqormabstractfilter.h"
#include "iqormmodel.h"
#include "iqormobjectsmodel.h"
#include "iqormsqldatasource.h"
#include "iqormdirectpropertydescription.h"
#include "iqormmanytomanyreferencepropertydescription.h"
#include <QDebug>
#include <QSqlQuery>


IQORMSQLObjectsModelDataSource::IQORMSQLObjectsModelDataSource(IQORMSQLDataSource *sqlDataSource) :
    IQORMAbstractObjectsModelDataSource(sqlDataSource),
    _sqlDataSource(sqlDataSource)
{
}

bool IQORMSQLObjectsModelDataSource::loadModel(IQORMObjectsModelBase *model,
                                               const qint64 limit,
                                               const qint64 offset)
{
    if (!model)
        return false;

    model->lastError()->clearErrors();

    if (!_sqlDataSource)
    {
        model->clear();
        model->lastError()->addError(tr("Not found valid SQL Data Source."));
        return false;
    }

    IQORMModel *ormModel = model->childsOrmModel();
    if (!ormModel)
    {
        model->clear();
        model->lastError()->addError(tr("Not found valid child objects ORM model."));
        return false;
    }

    QSqlDriver* driver = _sqlDataSource->database().driver();

    QString queryStr = "SELECT ";

    QStringList columns;
    columns << driver->escapeIdentifier("id", QSqlDriver::FieldName);

    foreach (IQORMPropertyDescription *propDescription, ormModel->propertyDescriptions())
    {
        if (!propDescription || propDescription->propertyType() != IQORMPropertyDescription::Direct)
            continue;
        IQORMDirectPropertyDescription *directPropertyDescription = dynamic_cast<IQORMDirectPropertyDescription *>(propDescription);
        QString column = driver->escapeIdentifier(directPropertyDescription->columnName(), QSqlDriver::FieldName);
        column.append(" AS ");
        column.append(IQORMSQLDataSource::escapedPropertyName(directPropertyDescription->propertyName()));
        columns << column;
    }

    queryStr.append(columns.join(", "));

    queryStr += " FROM ";

    QString tableName = driver->escapeIdentifier(ormModel->tableName(), QSqlDriver::TableName);

    if (tableName.isEmpty())
    {
        model->clear();
        model->lastError()->addError(tr("Table name for child objects ORM Model not set. Use IQORMModel::setTableName() first."));
        return false;
    }

    queryStr.append(tableName);

    QStringList refTableNamesList = referencingTableNames(ormModel, model->filters());
    QString refTableNamesString = refTableNamesList.join(", ");
    if (!refTableNamesString.isEmpty())
        queryStr.append(QString(", %0").arg(refTableNamesString));

    QList<QVariant> bindValues;
    QString whereStr;
    whereStr = filterString(ormModel, model->filters(), &bindValues);
    if (!whereStr.isEmpty())
        queryStr.append(QString(" WHERE %0").arg(whereStr));

    queryStr.append(" ORDER BY " + driver->escapeIdentifier("id", QSqlDriver::FieldName));

    if (limit != -1)
        queryStr.append(QString(" LIMIT %0 ").arg(limit));

    if (offset > 0)
        queryStr.append(QString(" OFFSET %0 ").arg(offset));

    bool queryOk = false;
    QString errorMessage;
    QSqlQuery query = _sqlDataSource->execQuery(queryStr, bindValues, true, &queryOk, &errorMessage);

    if (!queryOk)
    {
        model->clear();
        model->lastError()->addError(errorMessage);
        return false;
    }

    //Список объектов на удаление
    QList<IQORMObject *> objectsToRemove = model->toObjectList();

    //Для быстрого поиска сделаем список ид объектов
    QList<qint64> objectIds;
    foreach (IQORMObject *object, objectsToRemove)
    {
        if (!object)
            continue;
        objectIds << object->objectId();
    }

    //Пройдемся по всем записям
    while (query.next())
    {
        //Порядок колонок в ответе соответсвует порядку описания свойств в модели
        //Первым в отвтете всегда идет objectId
        //Получим objectId
        qint64 objectId = query.value(0).toLongLong();

        //Если модель содержит объект с данным ид
        int indexOfObject = objectIds.indexOf(objectId);
        if (indexOfObject > -1)
        {
            //Найдем объект с таким ид
            IQORMObject *oldObject = dynamic_cast<IQORMObject*>(model->get(indexOfObject));

            //Установим для объекта параметры из запроса
            //Будем использовать метод источника данных для объекта
            if(IQORMSQLObjectDataSource::loadObjectFromSQLRecord(oldObject, query.record()))
            {
                //Такой объект есть в модели и в выборке, значит удалять его не надо
                objectsToRemove.removeOne(oldObject);
            }
            else
            {
                model->lastError()->addError(tr("Load object from SQL Record for object with Id=%0 fauled. Removed from result.")
                                             .arg(objectId));
                //Т.к. объект не загрузился, оставим его в объекта на удаление
            }
        }
        else
        {
            //Если не нашли объект с таким ид, то создадим новый объект
            IQORMObject *newObject = model->createChildObject();

            //Установим для объекта параметры из запроса
            //Будем использовать метод источника данных для объекта
            if(!IQORMSQLObjectDataSource::loadObjectFromSQLRecord(newObject, query.record()))
            {
                model->lastError()->addError(tr("Load object from SQL Record for object with Id=%0 fauled. Removed from result.")
                                             .arg(objectId));
                //Т.к. объект не загрузился, удалим его
                delete newObject;
            }
            else
            {
                //Добавим новый объект в модель
                model->appendObject(newObject);
            }
        }
    }

    //Удалим из модели все объекты на удаление
    foreach (IQORMObject *objectToRemove, objectsToRemove)
    {
        model->removeObject(objectToRemove);
    }

    return true;
}

QList<qint64> IQORMSQLObjectsModelDataSource::findObjects(const IQORMModel *ormModel,
                                                          const IQORMAbstractFilter *filter,
                                                          const qint64 limit,
                                                          const qint64 offset,
                                                          bool *ok,
                                                          QString *errorMessage) const
{
    if (!_sqlDataSource)
    {
        if (errorMessage)
            *errorMessage = tr("Not found valid SQL Data Source.");
        if (ok)
            *ok = false;
        return QList<qint64>();
    }


    QSqlDriver* driver = _sqlDataSource->database().driver();

    QString queryStr = "SELECT id FROM ";

    if (!ormModel)
    {
        if (errorMessage)
            *errorMessage =tr("Not found valid child objects ORM model.");
        if (ok)
            *ok = false;
        return QList<qint64>();
    }

    QString tableName = driver->escapeIdentifier(ormModel->tableName(), QSqlDriver::TableName);

    if (tableName.isEmpty())
    {
        if (errorMessage)
            *errorMessage = tr("Table name for child objects ORM Model not set. Use IQORMModel::setTableName() first.");
        if (ok)
            *ok = false;
        return QList<qint64>();
    }

    queryStr.append(tableName);

    QStringList refTableNamesList = referencingTableNames(ormModel, filter);
    QString refTableNamesString = refTableNamesList.join(", ");
    if (!refTableNamesString.isEmpty())
        queryStr.append(QString(", %0").arg(refTableNamesString));

    QList<QVariant> bindValues;
    QString whereStr;
    whereStr = filterString(ormModel, filter, &bindValues);
    if (!whereStr.isEmpty())
        queryStr.append(QString(" WHERE %0").arg(whereStr));

    queryStr.append(" ORDER BY " + driver->escapeIdentifier("id", QSqlDriver::FieldName));

    if (limit != -1)
        queryStr.append(QString(" LIMIT %0 ").arg(limit));

    if (offset > 0)
        queryStr.append(QString(" OFFSET %0 ").arg(offset));

    bool queryOk = false;
    QSqlQuery query = _sqlDataSource->execQuery(queryStr, bindValues, true, &queryOk, errorMessage);

    if (!queryOk)
    {
        if (ok)
            *ok = false;
        return QList<qint64>();
    }

    QList<qint64> result;

    while (query.next())
    {
        result << query.value(0).toLongLong();
    }

    if (ok)
        *ok = true;

    return result;
}

QString IQORMSQLObjectsModelDataSource::filterString(const IQORMModel *ormModel,
                                                     const IQORMAbstractFilter *filter,
                                                     QList<QVariant> *bindValues) const
{
    if (!ormModel)
        return "";

    if (!filter)
        return "";

    if (!bindValues)
        return "";

    QSqlDriver* sqlDriver = _sqlDataSource->database().driver();
    if (!sqlDriver)
        return "";

    //Если это просто фильтр
    const IQORMFilter *simpleFilter = dynamic_cast<const IQORMFilter *>(filter);
    if (simpleFilter)
    {
        if (simpleFilter->operation() == IQORMFilter::None)
            return "";

        //Получим свойство модели
        IQORMPropertyDescription *propDescription = ormModel->propertyDescription(simpleFilter->property());
        if (!propDescription)
            return "";

        QString referencingString = "";
        QString columnName = "";

        if (propDescription->propertyType() == IQORMPropertyDescription::Direct)
        {
            IQORMDirectPropertyDescription *directPropertyDescription = dynamic_cast<IQORMDirectPropertyDescription *>(propDescription);

            columnName = sqlDriver->escapeIdentifier(directPropertyDescription->columnName(), QSqlDriver::FieldName);
        }
        else if (propDescription->propertyType() == IQORMPropertyDescription::ManyToManyReference)
        {
            IQORMManyToManyReferencePropertyDescription * mtmPropDescription = dynamic_cast<IQORMManyToManyReferencePropertyDescription *>(propDescription);

            QString referenceTableName = sqlDriver->escapeIdentifier(mtmPropDescription->referenceTableName(), QSqlDriver::TableName);
            QString objectIdColumnName = sqlDriver->escapeIdentifier(mtmPropDescription->objectIdColumnName(), QSqlDriver::FieldName);
            QString referencingIdColumnName = sqlDriver->escapeIdentifier(mtmPropDescription->referencingIdColumnName(), QSqlDriver::FieldName);
            columnName = QString("%0.%1")
                    .arg(referenceTableName)
                    .arg(referencingIdColumnName);

            referencingString = QString(" AND %0.%1 = %2.%3")
                    .arg(referenceTableName)
                    .arg(objectIdColumnName)
                    .arg(referenceTableName)
                    .arg(referencingIdColumnName);
        }

        if (columnName.isEmpty())
            return "";

        QString filterString = "";

        //В зависимости от типа свойства модели
        switch (propDescription->type())
        {
        case QVariant::Invalid:
            return "";
            break;

            //Строка
        case QVariant::String:
        {
            QString caseSesitivityColumnName = "";
            QString caseSesitivityBindParam = "";
            if (simpleFilter->caseSensitivity())
            {
                caseSesitivityColumnName = columnName;
                caseSesitivityBindParam = "?";
            }
            else
            {
                caseSesitivityColumnName = QString("LOWER(%0)").arg(columnName);
                caseSesitivityBindParam = "LOWER(?)";
            }

            switch (simpleFilter->operation())
            {
            case IQORMFilter::NotEquals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 <> %1")
                        .arg(caseSesitivityColumnName)
                        .arg(caseSesitivityBindParam);
                break;
            case IQORMFilter::Equals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 = %1")
                        .arg(caseSesitivityColumnName)
                        .arg(caseSesitivityBindParam);
                break;
            case IQORMFilter::StartsWith:
                bindValues->append(simpleFilter->value().toString() + "%");
                filterString = QString("%0 LIKE %1")
                        .arg(caseSesitivityColumnName)
                        .arg(caseSesitivityBindParam);
                break;
            case IQORMFilter::Contains:
                bindValues->append("%" + simpleFilter->value().toString() + "%");
                filterString = QString("%0 LIKE %1")
                        .arg(caseSesitivityColumnName)
                        .arg(caseSesitivityBindParam);
                break;
            case IQORMFilter::EndsWith:
                bindValues->append("%" + simpleFilter->value().toString());
                filterString = QString("%0 LIKE %1")
                        .arg(caseSesitivityColumnName)
                        .arg(caseSesitivityBindParam);
                break;
            default:
                filterString = "";
                break;
            }
            break;
        }

            //Числа
        case QVariant::Char:
        case QVariant::UInt:
        case QVariant::Int:
        case QVariant::ULongLong:
        case QVariant::LongLong:
        case QVariant::Double:
        {
            switch (simpleFilter->operation())
            {
            case IQORMFilter::Equals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 = ?")
                        .arg(columnName);
                break;
            case IQORMFilter::NotEquals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 <> ?")
                        .arg(columnName);
                break;
            case IQORMFilter::GreaterThan:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 > ?")
                        .arg(columnName);
                break;
            case IQORMFilter::GreaterOrEquals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 >= ?")
                        .arg(columnName);
                break;
            case IQORMFilter::LessThan:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 < ?")
                        .arg(columnName);
                break;
            case IQORMFilter::LessOrEquals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 <= ?")
                        .arg(columnName);
                break;
            default:
                filterString = "";
                break;
            }
            break;
        }
        default:
            //Ведем себя как с неизвестным типом
            switch (simpleFilter->operation())
            {
            case IQORMFilter::Equals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 = ?")
                        .arg(columnName);
                break;
            case IQORMFilter::NotEquals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 <> ?")
                        .arg(columnName);
                break;
            case IQORMFilter::GreaterThan:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 > ?")
                        .arg(columnName);
                break;
            case IQORMFilter::GreaterOrEquals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 >= ?")
                        .arg(columnName);
                break;
            case IQORMFilter::LessThan:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 < ?")
                        .arg(columnName);
                break;
            case IQORMFilter::LessOrEquals:
                bindValues->append(simpleFilter->value());
                filterString = QString("%0 <= ?")
                        .arg(columnName);
                break;
            default:
                filterString = "";
                break;
            }

            break;
        }

        return QString("%0 %1")
                .arg(filterString)
                .arg(referencingString);
    }


    //Если это групповой фильтр
    const IQORMGroupFilter *groupFilter = dynamic_cast<const IQORMGroupFilter*>(filter);
    if (groupFilter)
    {
        QStringList resultList;
        foreach (IQORMAbstractFilter *childFilter, groupFilter->toList())
        {
            if (!childFilter)
                continue;

            resultList << filterString(ormModel, childFilter, bindValues);
        }

        if (!resultList.isEmpty())
        {
            //В зависимости от типа фильтра
            if (groupFilter->type() == IQORMGroupFilter::Or)
            {
                return QString("( %0 )").arg(resultList.join(" OR "));
            }
            else
            {
                return QString("( %0 )").arg(resultList.join(" AND "));
            }
        }
    }

    return "";
}

QStringList IQORMSQLObjectsModelDataSource::referencingTableNames(const IQORMModel *ormModel,
                                                                  const IQORMAbstractFilter *filter) const
{
    if (!ormModel)
        return QStringList();

    if (!filter)
        return QStringList();

    QSqlDriver* sqlDriver = _sqlDataSource->database().driver();
    if (!sqlDriver)
        return QStringList();

    //Если это просто фильтр
    const IQORMFilter *simpleFilter = dynamic_cast<const IQORMFilter *>(filter);
    if (simpleFilter)
    {
        if (simpleFilter->operation() == IQORMFilter::None)
            return QStringList();

        //Получим свойство модели
        IQORMPropertyDescription *propDescription = ormModel->propertyDescription(simpleFilter->property());
        if (!propDescription)
            return QStringList();

        if (propDescription->propertyType() == IQORMPropertyDescription::Direct)
        {
            return QStringList();
        }
        else if (propDescription->propertyType() == IQORMPropertyDescription::ManyToManyReference)
        {
            IQORMManyToManyReferencePropertyDescription * mtmPropDescription = dynamic_cast<IQORMManyToManyReferencePropertyDescription *>(propDescription);

            return QStringList() << sqlDriver->escapeIdentifier(mtmPropDescription->referenceTableName(), QSqlDriver::TableName);
        }
    }

    //Если это групповой фильтр
    const IQORMGroupFilter *groupFilter = dynamic_cast<const IQORMGroupFilter*>(filter);
    if (groupFilter)
    {
        QStringList resultList;
        foreach (IQORMAbstractFilter *childFilter, groupFilter->toList())
        {
            if (!childFilter)
                continue;

            resultList << referencingTableNames(ormModel, childFilter);
        }

        return resultList;
    }

    return QStringList();
}
