#include "iqormpropertydescription.h"

IQORMPropertyDescription::IQORMPropertyDescription(QObject *parent) :
    QObject(parent),
    _propertyName(""),
    _type(QVariant::Invalid)
{
}

void IQORMPropertyDescription::setPropertyName(const QString &name)
{
    if (_propertyName == name)
        return;
    _propertyName = name;
}

void IQORMPropertyDescription::setType(const QVariant::Type type)
{
    if (_type == type)
        return;
    _type = type;
}
