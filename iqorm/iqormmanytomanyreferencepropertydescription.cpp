#include "iqormmanytomanyreferencepropertydescription.h"

IQORMManyToManyReferencePropertyDescription::IQORMManyToManyReferencePropertyDescription(QObject *parent) :
    IQORMPropertyDescription(parent),
    _referenceTableName(""),
    _objectIdColumnName(""),
    _referencingIdColumnName("")
{
}

IQORMPropertyDescription::PropertyType IQORMManyToManyReferencePropertyDescription::propertyType() const
{
    return IQORMPropertyDescription::ManyToManyReference;
}

void IQORMManyToManyReferencePropertyDescription::setReferenceTableName(const QString &tableName)
{
    if (_referenceTableName == tableName)
        return;
    _referenceTableName = tableName;

    emit referenceTableNameChanged();
}

void IQORMManyToManyReferencePropertyDescription::setObjectIdColumnName(const QString &columnName)
{
    if (_objectIdColumnName == columnName)
        return;
    _objectIdColumnName = columnName;

    emit objectIdColumnNameChanged();
}

void IQORMManyToManyReferencePropertyDescription::setReferencingIdColumnName(const QString &columnName)
{
    if (_referencingIdColumnName == columnName)
        return;
    _referencingIdColumnName = columnName;

    emit referencingIdColumnNameChanged();
}
