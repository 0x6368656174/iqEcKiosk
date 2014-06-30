#ifndef IQORMABSTRACTOBJECTSMODELDATASOURCE_H
#define IQORMABSTRACTOBJECTSMODELDATASOURCE_H

#include <QObject>
#include "iqormobject.h"
#include "iqormabstractfilter.h"

class IQORMAbstractDataSource;

class IQORMObjectsModelBase;

class IQORMAbstractObjectsModelDataSource : public QObject
{
    Q_OBJECT
public:
    explicit IQORMAbstractObjectsModelDataSource(IQORMAbstractDataSource *parent = 0);

    virtual ~IQORMAbstractObjectsModelDataSource();

    virtual QList<qint64> findObjects(const IQORMModel *ormModel,
                                      const IQORMAbstractFilter *filter,
                                      const qint64 limit = -1,
                                      const qint64 offset = 0,
                                      bool *ok = NULL,
                                      QString *errorMessage = NULL) const = 0;

    virtual bool loadModel(IQORMObjectsModelBase *model,
                           const qint64 limit = -1,
                           const qint64 offset = 0);
};

#endif // IQORMABSTRACTOBJECTSMODELDATASOURCE_H
