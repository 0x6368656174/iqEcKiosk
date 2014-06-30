#ifndef IQORMABSTRACTSYNCHRONIZER_H
#define IQORMABSTRACTSYNCHRONIZER_H

#include <QObject>

class IQORMObject;
class IQORMObjectsModelBase;

class IQORMAbstractSynchronizer : public QObject
{
    Q_OBJECT
public:
    explicit IQORMAbstractSynchronizer(QObject *parent = 0);

    virtual ~IQORMAbstractSynchronizer();

    virtual void registerObject(IQORMObject *object) = 0;

    virtual void unregisterObject(IQORMObject *object) = 0;

    virtual void updateLinkedObjects(IQORMObject *sourceObject) = 0;

    virtual void registerObjectsModel(IQORMObjectsModelBase *model) = 0;

    virtual void unregisterObjectsModel(IQORMObjectsModelBase *model) = 0;

    virtual void updateLinkedObjectModels(IQORMObject *sourceObject) = 0;
};

#endif // IQORMABSTRACTSYNCHRONIZER_H
