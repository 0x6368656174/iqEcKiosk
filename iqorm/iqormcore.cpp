#include "iqormcore.h"
#include "iqormnullsynchronizer.h"
#include "iqormnulldatasource.h"

QHash<QString, QPointer<IQORMModel> > IQORMCore::_models = QHash<QString, QPointer<IQORMModel> >();
QPointer<IQORMAbstractSynchronizer> IQORMCore::_synchronizer = NULL;
QPointer<IQORMAbstractDataSource> IQORMCore::_dataSource = NULL;

IQORMCore::IQORMCore(QObject *parent) :
    QObject(parent)
{
}

IQORMModel * IQORMCore::model(const IQORMObject *object)
{
    if (!object)
        return NULL;

    QString objectTypeName = QString(typeid(*object).name());

    if (!IQORMCore::_models.contains(objectTypeName) || !IQORMCore::_models[objectTypeName])
    {
        //Создадим новую модель
        IQORMModel *newModel = new IQORMModel();
        newModel->setTempInitObject(dynamic_cast<const QObject*>(object));
        object->initializeORMModel(newModel);
        IQORMCore::_models[objectTypeName] = newModel;
        newModel->setTempInitObject(NULL);
    }

    IQORMModel *result = _models[objectTypeName];

    return result;
}

QList<IQORMModel* > IQORMCore::allModels()
{
    QList<IQORMModel* > result;
    QHashIterator<QString, QPointer<IQORMModel> > i (IQORMCore::_models);
    while (i.hasNext())
    {
        i.next();
        result << i.value();
    }

    return result;
}

void IQORMCore::setSynchronizer(IQORMAbstractSynchronizer *syncronizer)
{
    if (IQORMCore::_synchronizer)
        IQORMCore::_synchronizer->deleteLater();
    syncronizer->setParent(NULL);
    IQORMCore::_synchronizer = syncronizer;
}


IQORMAbstractSynchronizer * IQORMCore::synchronizer()
{
    if (!IQORMCore::_synchronizer)
    {
        IQORMCore::setSynchronizer(new IQORMNullSynchronizer());
    }
    return IQORMCore::_synchronizer;
}

void IQORMCore::setDataSource(IQORMAbstractDataSource *dataSource)
{
    if (IQORMCore::_dataSource)
        IQORMCore::_dataSource->deleteLater();
    dataSource->setParent(NULL);
    IQORMCore::_dataSource = dataSource;
}

IQORMAbstractDataSource * IQORMCore::dataSource()
{
    if (!IQORMCore::_dataSource)
    {
        IQORMCore::setDataSource(new IQORMNullDataSource());
    }
    return IQORMCore::_dataSource;
}
