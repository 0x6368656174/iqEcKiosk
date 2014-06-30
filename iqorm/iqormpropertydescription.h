#ifndef IQORMPROPERTYDESCRIPTION_H
#define IQORMPROPERTYDESCRIPTION_H

#include <QObject>
#include <QVariant>

class IQORMPropertyDescription : public QObject
{
    Q_OBJECT
public:
    enum PropertyType
    {
        Direct,
        ManyToManyReference
    };

    explicit IQORMPropertyDescription(QObject *parent = 0);

    virtual PropertyType propertyType() const = 0;

public:
    inline QString propertyName() const {return _propertyName;}
    void setPropertyName(const QString &name);

    inline QVariant::Type type() const {return _type;}
    void setType(const QVariant::Type type);

private:
    QString _ormObjectTypeName;
    QString _propertyName;
    QVariant::Type _type;
};

#endif // IQORMPROPERTYDESCRIPTION_H
