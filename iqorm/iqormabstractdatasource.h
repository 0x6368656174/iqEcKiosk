#ifndef IQORMABSTRACTDATASOURCE_H
#define IQORMABSTRACTDATASOURCE_H

#include <QObject>
#include "iqormabstractobjectdatasource.h"
#include "iqormabstractobjectsmodeldatasource.h"

class IQORMObject;

class IQORMAbstractDataSource : public QObject
{
    Q_OBJECT
public:
    explicit IQORMAbstractDataSource(QObject *parent = 0);

    virtual ~IQORMAbstractDataSource();

    virtual IQORMAbstractObjectDataSource *objectDataSource() const = 0;

    virtual IQORMAbstractObjectsModelDataSource *objectsModelDataSource() const = 0;
};

#endif // IQORMABSTRACTDATASOURCE_H
