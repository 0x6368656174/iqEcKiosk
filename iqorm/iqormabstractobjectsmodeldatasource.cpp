#include "iqormabstractobjectsmodeldatasource.h"
#include "iqormabstractdatasource.h"
#include "iqormobjectsmodel.h"

IQORMAbstractObjectsModelDataSource::IQORMAbstractObjectsModelDataSource(IQORMAbstractDataSource *parent) :
    QObject(parent)
{
}

IQORMAbstractObjectsModelDataSource::~IQORMAbstractObjectsModelDataSource()
{
}

bool IQORMAbstractObjectsModelDataSource::loadModel(IQORMObjectsModelBase *model, const qint64 limit, const qint64 offset)
{
    if (!model)
        return false;

    bool ok = false;
    QString error;
    QList<qint64> selection = findObjects(model->childsOrmModel(),
                                          model->filters(),
                                          limit,
                                          offset,
                                          &ok,
                                          &error);

    if (!ok)
    {
        model->clear();
        model->lastError()->addError(error);
        return false;
    }

    QList<qint64> newObjectsSelection = selection;

    //Удалим из модели объекты, которых нет в выборке,
    //а так же создадим выборку из номеров объектов, которых нет в модели
    foreach (IQORMObject *object, model->toObjectList())
    {
        if (!selection.contains(object->objectId()))
        {
            model->removeObject(object);
            delete object;
        }
        else
        {
            newObjectsSelection.removeOne(object->objectId());
        }
    }

    qint32 id = 0;
    //Создадим и заргурзим необходимые объекты
    foreach (qint64 objectId, newObjectsSelection)
    {
        IQORMObject *newObject = model->createChildObject();
        newObject->load(objectId, model->lastDataSource());
        model->appendObject(newObject);
        qDebug() << id++;
    }

    return true;
}
