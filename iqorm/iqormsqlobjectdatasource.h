#ifndef IQORMSQLOBJECTDATASOURCE_H
#define IQORMSQLOBJECTDATASOURCE_H

#include "iqormabstractobjectdatasource.h"
#include <QSqlRecord>

class IQORMSQLObjectDataSource : public IQORMAbstractObjectDataSource
{
    Q_OBJECT
public:
    explicit IQORMSQLObjectDataSource(IQORMSQLDataSource *sqlDataSource = 0);

    virtual bool loadObjectById(IQORMObject* object, const qint64 id) const;

    virtual bool loadObjectManyToManyReference(IQORMObject *object, const QString &referenceProperty) const;

    virtual bool updateObject(IQORMObject* object);

    virtual bool insertObject(IQORMObject *object, qint64 *newId = 0);

    virtual bool removeObject(IQORMObject* object);

    static bool loadObjectFromSQLRecord(IQORMObject* object, const QSqlRecord &record);

private:
    IQORMSQLDataSource* _sqlDataSource;

    static QObject *lastPropertyObject(QObject *object, const QString &fullPropertyPath);

    QVariant excapedValue(const QVariant &value) const;
};

#endif // IQORMSQLOBJECTDATASOURCE_H
