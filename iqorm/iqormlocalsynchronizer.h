#ifndef IQORMLOCALSYNCHRONIZER_H
#define IQORMLOCALSYNCHRONIZER_H

#include "iqormabstractsynchronizer.h"
#include <QMultiHash>

class IQORMLocalSynchronizer : public IQORMAbstractSynchronizer
{
    Q_OBJECT
public:
    explicit IQORMLocalSynchronizer(QObject *parent = 0);

    virtual void registerObject(IQORMObject *object);

    virtual void unregisterObject(IQORMObject *object);

    virtual void updateLinkedObjects(IQORMObject *sourceObject);

    virtual void setLinkedObjectsId(IQORMObject *sourceObject, const qint64 oldId, const qint64 newId);

    virtual void registerObjectsModel(IQORMObjectsModelBase *model);

    virtual void unregisterObjectsModel(IQORMObjectsModelBase *model);

    virtual void updateLinkedObjectModels(IQORMObject *sourceObject);

private:
    //Тип объекта, объект
    QMultiHash<QString, IQORMObject*> _objects;

    //Тип объекта модели, модель
    QMultiHash<QString, IQORMObjectsModelBase*> _objetsModels;
};

#endif // IQORMLOCALSYNCHRONIZER_H
