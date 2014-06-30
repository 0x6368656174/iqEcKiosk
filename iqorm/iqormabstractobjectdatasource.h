#ifndef IQORMABSTRACTOBJECTDATASOURCE_H
#define IQORMABSTRACTOBJECTDATASOURCE_H

#include <QObject>
#include "iqormobject.h"

class IQORMAbstractDataSource;

class IQORMAbstractObjectDataSource : public QObject
{
    Q_OBJECT
public:
    explicit IQORMAbstractObjectDataSource(IQORMAbstractDataSource *parent = 0);

    virtual ~IQORMAbstractObjectDataSource();

    virtual bool loadObjectById(IQORMObject* object, const qint64 id) const = 0;

    bool loadObjectAllManyToManyReferences(IQORMObject *object) const;

    virtual bool loadObjectManyToManyReference(IQORMObject *object, const QString &referenceProperty) const = 0;

    virtual bool updateObject(IQORMObject* object) = 0;

    virtual bool insertObject(IQORMObject *object, qint64 *newId = 0) = 0;

    virtual bool removeObject(IQORMObject* object) = 0;
};

#endif // IQORMABSTRACTOBJECTDATASOURCE_H
