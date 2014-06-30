#include "iqormmodel.h"
#include "iqormdirectpropertydescription.h"
#include "iqormmanytomanyreferencepropertydescription.h"
#include <QMetaProperty>
#include <QDebug>

IQORMModel::IQORMModel(QObject *parent) :
    QObject(parent),
    _tableName("")
{
}

void IQORMModel::setTableName(const QString name)
{
    if (_tableName == name)
        return;
    _tableName = name;
}

bool IQORMModel::setPropertyORMDescription(const QString &property,
                                           IQORMPropertyDescription *description)
{
    if (property.isEmpty())
        return false;

    if (!description)
        return false;

    //Если для данного свойства уже есть описание
    if (_propertyDescriptions.contains(property))
    {
        //Если описание существует
        IQORMPropertyDescription *oldDescription = _propertyDescriptions[property];
        if (oldDescription)
        {
            oldDescription->deleteLater();
        }
        _propertyDescriptions.remove(property);
    }

    //Если доступен временый объект для инициализации
    if (_tempInitObject)
    {
        //Попробуем найти свойство у данного объекта
        QStringList propertyPath = property.split(".");

        QObject *lastObject = const_cast<QObject*>(_tempInitObject);
        for (int i = 0; i < propertyPath.count() - 1; i++)
        {
            lastObject = lastObject->property(propertyPath[i].toLocal8Bit().constData()).value<QObject *>();
            //Если не нашли это свойство в объекте, то выйдем
            if (!lastObject)
                return false;
        }

        int propertyIndex = lastObject->metaObject()->indexOfProperty(property.split(".").last().toLocal8Bit().constData());
        //Если не нашли это свойство в объекте, то выйдем
        if (propertyIndex < 0)
        {
            qDebug() << tr("Property \"%0\" no foud in object. Check Q_PROPERTY.").arg(property);
            return false;
        }

        //Получим тип содержимого свойства
        QVariant::Type propertyType = lastObject->metaObject()->property(propertyIndex).type();
        //Сохраним тип свойства
        description->setType(propertyType);
    }

    description->setPropertyName(property);

    description->setParent(this);
    _propertyDescriptions[property] = description;

    return true;
}

void IQORMModel::setPropertyManyToManyReference(const QString &property,
                                               const QString &referenceTableName,
                                               const QString &objectIdColumnName,
                                               const QString &referencingIdColumnName)
{
    if (property.isEmpty() || referenceTableName.isEmpty() || objectIdColumnName.isEmpty() || referencingIdColumnName.isEmpty())
        return;

    IQORMManyToManyReferencePropertyDescription *newDescription = new IQORMManyToManyReferencePropertyDescription(this);

    newDescription->setReferenceTableName(referenceTableName);
    newDescription->setObjectIdColumnName(objectIdColumnName);
    newDescription->setReferencingIdColumnName(referencingIdColumnName);

    if (!setPropertyORMDescription(property, newDescription))
        newDescription->deleteLater();
}

void IQORMModel::setPropertyColumn(const QString &property,
                                   const QString &columnName)
{
    if (property.isEmpty() || columnName.isEmpty())
        return;

    IQORMDirectPropertyDescription *newDescription = new IQORMDirectPropertyDescription(this);
    newDescription->setColumnName(columnName);

    if (!setPropertyORMDescription(property, newDescription))
        newDescription->deleteLater();
}

QList<IQORMPropertyDescription *> IQORMModel::propertyDescriptions() const
{
    return _propertyDescriptions.values();
}
void IQORMModel::setTempInitObject(const QObject *object)
{
    _tempInitObject = object;
}

void IQORMModel::setObjectClassName(const QString &className)
{
    _objectClassName = className;
}

IQORMPropertyDescription * IQORMModel::propertyDescription(const QString &propertyName) const
{
    if (!_propertyDescriptions.contains(propertyName))
        return NULL;

    return _propertyDescriptions[propertyName];
}
