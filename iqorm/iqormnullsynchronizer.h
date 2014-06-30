#ifndef IQORMNULLSYNCHRONIZER_H
#define IQORMNULLSYNCHRONIZER_H

#include "iqormabstractsynchronizer.h"

class IQORMNullSynchronizer : public IQORMAbstractSynchronizer
{
    Q_OBJECT
public:
    explicit IQORMNullSynchronizer(QObject *parent = 0);

    virtual void registerObject(IQORMObject *object){Q_UNUSED(object);}

    virtual void unregisterObject(IQORMObject *object){Q_UNUSED(object);}

    virtual void updateLinkedObjects(IQORMObject *sourceObject){Q_UNUSED(sourceObject);}

    virtual void registerObjectsModel(IQORMObjectsModelBase *model){Q_UNUSED(model);}

    virtual void unregisterObjectsModel(IQORMObjectsModelBase *model){Q_UNUSED(model);}

    virtual void updateLinkedObjectModels(IQORMObject *sourceObject){Q_UNUSED(sourceObject);}
};

#endif // IQORMNULLSYNCHRONIZER_H
