#ifndef IQORMMANYTOMANYREFERENCEPROPERTYDESCRIPTION_H
#define IQORMMANYTOMANYREFERENCEPROPERTYDESCRIPTION_H

#include "iqormpropertydescription.h"

class IQORMManyToManyReferencePropertyDescription : public IQORMPropertyDescription
{
    Q_OBJECT
    Q_PROPERTY(QString referenceTableName READ referenceTableName WRITE setReferenceTableName NOTIFY referenceTableNameChanged)
    Q_PROPERTY(QString objectIdColumnName READ objectIdColumnName WRITE setObjectIdColumnName NOTIFY objectIdColumnNameChanged)
    Q_PROPERTY(QString referencingIdColumnName READ referencingIdColumnName WRITE setReferencingIdColumnName NOTIFY referencingIdColumnNameChanged)
public:
    explicit IQORMManyToManyReferencePropertyDescription(QObject *parent = 0);

    virtual PropertyType propertyType() const;

public:
    inline QString referenceTableName() const {return _referenceTableName;}
    void setReferenceTableName(const QString &tableName);

    inline QString objectIdColumnName() const {return _objectIdColumnName;}
    void setObjectIdColumnName(const QString &columnName);

    inline QString referencingIdColumnName() const {return _referencingIdColumnName;}
    void setReferencingIdColumnName(const QString &columnName);

signals:
    void referenceTableNameChanged();
    void objectIdColumnNameChanged();
    void referencingIdColumnNameChanged();

private:
    QString _referenceTableName;
    QString _objectIdColumnName;
    QString _referencingIdColumnName;
};

#endif // IQORMMANYTOMANYREFERENCEPROPERTYDESCRIPTION_H
