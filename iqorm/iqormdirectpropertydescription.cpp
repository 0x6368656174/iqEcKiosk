#include "iqormdirectpropertydescription.h"

IQORMDirectPropertyDescription::IQORMDirectPropertyDescription(QObject *parent) :
    IQORMPropertyDescription(parent),
    _columnName("")
{
}

IQORMPropertyDescription::PropertyType IQORMDirectPropertyDescription::propertyType() const
{
    return IQORMPropertyDescription::Direct;
}

void IQORMDirectPropertyDescription::setColumnName(const QString &columnName)
{
    if (_columnName == columnName)
        return;
    _columnName = columnName;

    emit columnNameChanged();
}
