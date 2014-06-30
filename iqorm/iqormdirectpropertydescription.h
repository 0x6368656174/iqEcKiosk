#ifndef IQORMDIRECTPROPERTYDESCRIPTION_H
#define IQORMDIRECTPROPERTYDESCRIPTION_H

#include "iqormpropertydescription.h"

class IQORMDirectPropertyDescription : public IQORMPropertyDescription
{
    Q_OBJECT
    Q_PROPERTY(QString columnName READ columnName WRITE setColumnName NOTIFY columnNameChanged)
public:
    explicit IQORMDirectPropertyDescription(QObject *parent = 0);

    virtual PropertyType propertyType() const;

public:
    inline QString columnName() const {return _columnName;}
    void setColumnName(const QString &columnName);

signals:
    void columnNameChanged();

private:
    QString _columnName;
};

#endif // IQORMDIRECTPROPERTYDESCRIPTION_H
