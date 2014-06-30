#ifndef IQORMMODEL_H
#define IQORMMODEL_H

#include <QObject>
#include <QHash>
#include "iqormpropertydescription.h"
#include "iqormobject.h"

class IQORMModel: public QObject
{
    Q_OBJECT
public:
    explicit IQORMModel(QObject *parent = 0);

    inline QString objectClassName() const {return _objectClassName;}

    inline QString tableName() const {return _tableName;}

    void setTableName(const QString name);

    void setPropertyColumn(const QString &property,
                           const QString &columnName);

    void setPropertyManyToManyReference(const QString &property,
                                        const QString &referenceTableName,
                                        const QString &objectIdColumnName,
                                        const QString &referencingIdColumnName);

    bool setPropertyORMDescription(const QString &property,
                                   IQORMPropertyDescription *description);

    QList<IQORMPropertyDescription *> propertyDescriptions() const;

    IQORMPropertyDescription * propertyDescription(const QString &propertyName) const;

private:
    friend class IQORMCore;

    //имя свойства, его описание
    QHash<QString, IQORMPropertyDescription *> _propertyDescriptions;
    QString _objectClassName;
    QString _tableName;

    //Временный объект для инициализации модели
    const QObject *_tempInitObject;
    void setTempInitObject(const QObject *object);

    void setObjectClassName(const QString &className);
};

#endif // IQORMMODEL_H
