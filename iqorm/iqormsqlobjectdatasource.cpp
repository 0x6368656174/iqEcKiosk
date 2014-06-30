#include "iqormsqlobjectdatasource.h"
#include "iqormsqldatasource.h"
#include "iqormpropertydescription.h"
#include "iqormdirectpropertydescription.h"
#include "iqormmanytomanyreferencepropertydescription.h"
#include "iqormmodel.h"
#include <QSqlDriver>
#include <QSqlQuery>
#include <QDebug>

IQORMSQLObjectDataSource::IQORMSQLObjectDataSource(IQORMSQLDataSource *sqlDataSource) :
    IQORMAbstractObjectDataSource(sqlDataSource),
    _sqlDataSource(sqlDataSource)
{
}

bool IQORMSQLObjectDataSource::loadObjectFromSQLRecord(IQORMObject *object, const QSqlRecord &record)
{
    if (!object)
        return false;

    //Установи objectId
    object->setObjectId(record.value(0).toLongLong());

    IQORMModel *ormModel = object->ormModel();
    if (!ormModel)
    {
        object->reset();
        object->lastError()->addError(tr("Not found valid object ORM model."));
        return false;
    }

    int i = 0;
    foreach (IQORMPropertyDescription *propDescription, ormModel->propertyDescriptions())
    {
        i++;
        if (!propDescription)
            continue;

        //Получаем значение из записи для свойства
        //Свойства в выборке должны идти в том же порядке, что и свойства в модели, со смещением на 1
        //т.к. первым свойством в выборке будет objectId, который не перечислен в модели
        //Поэтому будем использовать индексы для получения свойств с целью оптимизации скорости
        QVariant value = record.value(i);
        if (!value.isValid())
            continue;

        //Устанавливаем значение
        QString fullPropPaht = propDescription->propertyName();
        QObject *lastPropObj = lastPropertyObject(dynamic_cast<QObject*>(object), fullPropPaht);

        if (!lastPropObj)
            continue;

        QString lastPropName = fullPropPaht.split('.').last();

        lastPropObj->setProperty(lastPropName.toLocal8Bit().constData(), value);
    }

    return true;
}

QObject * IQORMSQLObjectDataSource::lastPropertyObject(QObject *object, const QString &fullPropertyPath)
{
    QStringList propertyPath = fullPropertyPath.split(".");

    QObject *lastObject = qobject_cast<QObject *>(object);
    for (int i = 0; i < propertyPath.count() - 1; i++)
    {
        lastObject = lastObject->property(propertyPath[i].toLocal8Bit().constData()).value<QObject *>();
        if (!lastObject)
        {
            return NULL;
        }
    }

    return lastObject;
}

bool IQORMSQLObjectDataSource::loadObjectById(IQORMObject *object, const qint64 id) const
{
    if (!object)
        return false;

    object->resetReference();

    QObject *qobject = dynamic_cast<QObject*>(object);
    if (!qobject)
    {
        object->reset();
        object->lastError()->addError(tr("Object must be child of QObject"));
        return false;
    }

    if (id < 0)
    {
        object->reset();
        object->lastError()->addError(tr("ObjectId not set. Set objectId first."));
        return false;
    }

    if (!_sqlDataSource)
    {
        object->reset();
        object->lastError()->addError(tr("Not found valid SQL Data Source."));
        return false;
    }

    QSqlDriver* driver = _sqlDataSource->database().driver();

    QString queryStr = "SELECT ";

    IQORMModel *ormModel = object->ormModel();
    if (!ormModel)
    {
        object->reset();
        object->lastError()->addError(tr("Not found valid object ORM model."));
        return false;
    }

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

    queryStr.append(" FROM ");

    QString tableName = driver->escapeIdentifier(ormModel->tableName(), QSqlDriver::TableName);

    if (tableName.isEmpty())
    {
        object->reset();
        object->lastError()->addError(tr("Table name for ORM Model not set. Use IQORMModel::setTableName() first."));
        return false;
    }

    queryStr.append(tableName);

    queryStr.append(" WHERE id=?");

    QVariantList bindValues;
    bindValues << id;

    QString error;
    bool ok = false;

    QSqlQuery query = _sqlDataSource->execQuery(queryStr, bindValues, true, &ok, &error);
    if (!ok)
    {
        object->reset();
        object->lastError()->addError(error);
        return false;
    }

    if (!query.isActive())
    {
        object->reset();
        object->lastError()->addError(tr("SQL query not active."));
        return false;
    }

    //Получим первую запись
    if (!query.next())
    {
        object->reset();
        object->lastError()->addError(tr("Not found object with objectId = %0.")
                                      .arg(id));
        return false;
    }

    //Загрузим данные в объект из первой записи
    return loadObjectFromSQLRecord(object, query.record());
}

bool IQORMSQLObjectDataSource::loadObjectManyToManyReference(IQORMObject *object, const QString &referenceProperty) const
{
    if (!object)
        return false;

    if (object->objectId() < 0)
    {
        object->lastError()->addError(tr("Object must be loaded first. Use IQORMObject::load()"));
        return false;
    }

    if (referenceProperty.isEmpty())
    {
        object->lastError()->addError(tr("Reference property can not be empty."));
        return false;
    }

    QObject *qobject = dynamic_cast<QObject*>(object);
    if (!qobject)
    {
        qobject->setProperty(referenceProperty.toLocal8Bit().constData(), QVariantList());
        object->lastError()->addError(tr("Object must be child of QObject"));
        return false;
    }

    if (!_sqlDataSource)
    {
        qobject->setProperty(referenceProperty.toLocal8Bit().constData(), QVariantList());
        object->lastError()->addError(tr("Not found valid SQL Data Source."));
        return false;
    }

    QSqlDriver* driver = _sqlDataSource->database().driver();

    IQORMModel *ormModel = object->ormModel();
    if (!ormModel)
    {
        qobject->setProperty(referenceProperty.toLocal8Bit().constData(), QVariantList());
        object->lastError()->addError(tr("Not found valid object ORM model."));
        return false;
    }

    QVariantList bindValues;
    bindValues << object->objectId();

    //ОБРАБОТКА СВОЙСТВ С ВНЕШНИМИ СВЯЗЯМИ
    foreach (IQORMPropertyDescription *propDescription, ormModel->propertyDescriptions())
    {
        if (!propDescription)
            continue;

        if (propDescription->propertyName() != referenceProperty)
            continue;

        if (propDescription->propertyType() != IQORMPropertyDescription::ManyToManyReference)
        {
            qobject->setProperty(referenceProperty.toLocal8Bit().constData(), QVariantList());
            object->lastError()->addError(tr("Property \"%0\" must be ManyToManyReference type.").arg(referenceProperty));
            return false;
        }
        //Создадим запрос для заполнения свойств, содержащих связаные таблицы
        QString queryStr = "SELECT ";

        IQORMManyToManyReferencePropertyDescription * mtmPropDescription = dynamic_cast<IQORMManyToManyReferencePropertyDescription *>(propDescription);
        QString referenceTableName = driver->escapeIdentifier(mtmPropDescription->referenceTableName(), QSqlDriver::TableName);
        QString objectIdColumName = driver->escapeIdentifier(mtmPropDescription->objectIdColumnName(), QSqlDriver::FieldName);
        QString referenceColumn = driver->escapeIdentifier(mtmPropDescription->referencingIdColumnName(), QSqlDriver::FieldName);
        referenceColumn.append(" AS ");
        referenceColumn.append(IQORMSQLDataSource::escapedPropertyName(mtmPropDescription->propertyName()));

        queryStr.append(referenceColumn);

        queryStr.append(" FROM ");

        queryStr.append(referenceTableName);

        queryStr.append(" WHERE ");

        queryStr.append(QString("%0=?").arg(objectIdColumName));

        bool ok = false;
        QString error;

        QSqlQuery referenceQuery = _sqlDataSource->execQuery(queryStr, bindValues, true, &ok, &error);
        if (!ok)
        {
            qobject->setProperty(referenceProperty.toLocal8Bit().constData(), QVariantList());
            object->lastError()->addError(error);
            return false;
        }

        if (!referenceQuery.isActive())
        {
            qobject->setProperty(referenceProperty.toLocal8Bit().constData(), QVariantList());
            object->lastError()->addError(tr("SQL reference query not active."));
            return false;
        }

        QVariantList referenceIds;
        while (referenceQuery.next())
        {
            referenceIds << referenceQuery.value(0);
        }

        qobject->setProperty(propDescription->propertyName().toLocal8Bit().constData(),
                             referenceIds);

        break;
    }

    return true;
}

bool IQORMSQLObjectDataSource::insertObject(IQORMObject *object, qint64 *newId)
{
    if (!object)
        return false;

    if (!_sqlDataSource)
    {
        object->lastError()->addError(tr("Not found valid SQL Data Source."));
        return false;
    }

    QSqlDriver* driver = _sqlDataSource->database().driver();

    QString queryStr = "INSERT INTO ";

    IQORMModel *ormModel = object->ormModel();
    if (!ormModel)
    {
        object->lastError()->addError(tr("Not found valid object ORM model."));
        return false;
    }

    QString tableName = driver->escapeIdentifier(ormModel->tableName(), QSqlDriver::TableName);

    queryStr.append(tableName);

    QStringList filds;
    QVariantList bindValues;

    foreach (IQORMPropertyDescription *propDescription, ormModel->propertyDescriptions())
    {
        if (!propDescription || propDescription->propertyType() != IQORMPropertyDescription::Direct)
            continue;

        IQORMDirectPropertyDescription *directPropertyDescription = dynamic_cast<IQORMDirectPropertyDescription *>(propDescription);

        QString fullPropPath = directPropertyDescription->propertyName();
        QObject *lastPropObj = lastPropertyObject(dynamic_cast<QObject*>(object), fullPropPath);

        if (!lastPropObj)
            continue;

        QString lastPropName = fullPropPath.split('.').last();

        QVariant propertyValue = lastPropObj->property(lastPropName.toLocal8Bit().constData());

        if (!propertyValue.isValid())
            continue;

        filds << driver->escapeIdentifier(directPropertyDescription->columnName(), QSqlDriver::FieldName);
        bindValues << excapedValue(propertyValue);
    }

    queryStr.append(" ( ");

    queryStr.append(filds.join(", "));

    queryStr.append(" ) VALUES ( ");

    QStringList valuesPlaceholders;
    for (int i = 0; i < filds.count(); i++)
    {
        valuesPlaceholders.append("?");
    }

    queryStr.append(valuesPlaceholders.join(", "));

    queryStr.append(" ) ");

    bool ok = false;
    QString error;

    QSqlQuery query = _sqlDataSource->execQuery(queryStr, bindValues, true, &ok, &error);

    if (!ok)
    {
        object->lastError()->addError(error);
        return false;
    }

    if (newId)
        *newId = query.lastInsertId().toInt();

    return ok;
}

QVariant IQORMSQLObjectDataSource::excapedValue(const QVariant &value) const
{
    QVariant result = value;
    switch (static_cast<QMetaType::Type>(value.type()))
    {
    case QMetaType::UChar:
    case QMetaType::Char:
        result = value.toInt();
    default:
        break;
    }

    return result;
}

bool IQORMSQLObjectDataSource::updateObject(IQORMObject *object)
{
    if (!object)
        return false;

    if (object->objectId() < 0)
    {
        object->lastError()->addError(tr("Can not update object without objectId."));
        return false;
    }

    if (!_sqlDataSource)
    {
        object->lastError()->addError(tr("Not found valid SQL Data Source."));
        return false;
    }

    QSqlDriver* driver = _sqlDataSource->database().driver();

    QString queryStr = "UPDATE ";

    IQORMModel *ormModel = object->ormModel();
    if (!ormModel)
    {
        object->lastError()->addError(tr("Not found valid object ORM model."));
        return false;
    }

    QString tableName = driver->escapeIdentifier(ormModel->tableName(), QSqlDriver::TableName);

    queryStr.append(tableName);

    queryStr.append(" SET ");

    QStringList sets;
    QVariantList bindValues;
    foreach (IQORMPropertyDescription *propDescription, ormModel->propertyDescriptions())
    {
        if (!propDescription || propDescription->propertyType() != IQORMPropertyDescription::Direct)
            continue;

        IQORMDirectPropertyDescription *directPropertyDescription = dynamic_cast<IQORMDirectPropertyDescription *>(propDescription);

        QString fullPropPath = directPropertyDescription->propertyName();
        QObject *lastPropObj = lastPropertyObject(dynamic_cast<QObject *>(object), fullPropPath);

        if (!lastPropObj)
            continue;

        QString lastPropName = fullPropPath.split('.').last();

        QVariant propertyValue = lastPropObj->property(lastPropName.toLocal8Bit().constData());

        if (!propertyValue.isValid())
            continue;

        QString set = driver->escapeIdentifier(directPropertyDescription->columnName(), QSqlDriver::FieldName);
        set.append("=?");
        sets << set;
        bindValues << excapedValue(propertyValue);
    }

    queryStr.append(sets.join(", "));

    queryStr.append(" WHERE id=?");

    bindValues << object->objectId();

    bool ok = false;
    QString error;

    _sqlDataSource->execQuery(queryStr, bindValues, true, &ok, &error);

    if (!ok)
    {
        object->lastError()->addError(error);
        return false;
    }

    return ok;
}


bool IQORMSQLObjectDataSource::removeObject(IQORMObject *object)
{
    if (!object)
        return false;

    if (object->objectId() < 0)
    {
        object->lastError()->addError(tr("Can not remove object without objectId."));
        return false;
    }

    if (!_sqlDataSource)
    {
        object->lastError()->addError(tr("Not found valid SQL Data Source."));
        return false;
    }

    QSqlDriver* driver = _sqlDataSource->database().driver();

    QString queryStr = "DELETE FROM ";

    IQORMModel *ormModel = object->ormModel();
    if (!ormModel)
    {
        object->lastError()->addError(tr("Not found valid object ORM model."));
        return false;
    }

    QString tableName = driver->escapeIdentifier(ormModel->tableName(), QSqlDriver::TableName);

    queryStr.append(tableName + " WHERE id=?");

    QVariantList bindValues;
    bindValues << object->objectId();

    bool ok = false;
    QString error;

    _sqlDataSource->execQuery(queryStr, bindValues, true, &ok, &error);

    if (!ok)
    {
        object->lastError()->addError(error);
        return false;
    }

    return ok;
}
