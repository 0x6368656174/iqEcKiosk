#include "iqormlocalsynchronizer.h"
#include "iqormobject.h"
#include "iqormobjectsmodel.h"
#include <typeinfo>

IQORMLocalSynchronizer::IQORMLocalSynchronizer(QObject *parent) :
    IQORMAbstractSynchronizer(parent)
{
}

void IQORMLocalSynchronizer::registerObject(IQORMObject *object)
{
    if (!object)
        return;
    QString objectTypeName = QString(typeid(object).name());

    _objects.insert(objectTypeName, object);
}

void IQORMLocalSynchronizer::unregisterObject(IQORMObject *object)
{
    if (!object)
        return;

    QString objectTypeName = QString(typeid(object).name());

    _objects.remove(objectTypeName, object);
}

void IQORMLocalSynchronizer::updateLinkedObjects(IQORMObject *sourceObject)
{
    if (!sourceObject)
        return;

    QString sourceObjectTypeName = QString(typeid(sourceObject).name());

    //Найдем все связаные объекты
    foreach (IQORMObject *object, _objects.values(sourceObjectTypeName))
    {
        if (!object)
            continue;

        if (object->objectId() == sourceObject->objectId() && object->lastDataSource() == sourceObject->lastDataSource())
        {
            object->reload();
        }
    }
}

void IQORMLocalSynchronizer::setLinkedObjectsId(IQORMObject *sourceObject, const qint64 oldId, const qint64 newId)
{
    if (!sourceObject)
        return;

    QString sourceObjectTypeName = QString(typeid(sourceObject).name());

    //Найдем все связаные объекты
    foreach (IQORMObject *object, _objects.values(sourceObjectTypeName))
    {
        if (!object)
            continue;


        if (oldId == sourceObject->objectId() && object->lastDataSource() == sourceObject->lastDataSource())
        {
            object->setObjectId(newId);
        }
    }
}

void IQORMLocalSynchronizer::registerObjectsModel(IQORMObjectsModelBase *model)
{
    if (!model)
        return;

    _objetsModels.insert(model->childsTypeName(), model);
}

void IQORMLocalSynchronizer::unregisterObjectsModel(IQORMObjectsModelBase *model)
{
    if (!model)
        return;

    _objetsModels.remove(model->childsTypeName(), model);
}

void IQORMLocalSynchronizer::updateLinkedObjectModels(IQORMObject *sourceObject)
{
    if (!sourceObject)
        return;

    QString sourceObjectTypeName = QString(typeid(sourceObject).name());

    //Найдем все модели, содаржащие объекты того же типа
    foreach (IQORMObjectsModelBase *model, _objetsModels.values(sourceObjectTypeName))
    {
        if (!model)
            continue;

        if(sourceObject->lastDataSource() == model->lastDataSource())
            model->load();
    }
}
