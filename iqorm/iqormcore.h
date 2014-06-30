#ifndef IQORMCORE_H
#define IQORMCORE_H

#include <QObject>
#include <QPointer>
#include <iqormobject.h>
#include <typeinfo>
#include "iqormmodel.h"
#include "iqormabstractsynchronizer.h"
#include "iqormabstractdatasource.h"

class IQORMCore : public QObject
{
    Q_OBJECT
public:

    explicit IQORMCore(QObject *parent = 0);

    static void setSynchronizer(IQORMAbstractSynchronizer *syncronizer);

    static IQORMAbstractSynchronizer* synchronizer();

    static void setDataSource(IQORMAbstractDataSource *dataSource);

    static IQORMAbstractDataSource* dataSource();

    template <typename T>
    static IQORMModel *staticORMModel();

    static IQORMModel *model(const IQORMObject *object);

    static QList<IQORMModel*> allModels();

private:
    static QHash<QString, QPointer<IQORMModel> > _models;
    static QPointer<IQORMAbstractSynchronizer> _synchronizer;
    static QPointer<IQORMAbstractDataSource> _dataSource;
};


template <typename T>
IQORMModel* IQORMCore::staticORMModel()
{
    QString objectTypeName = QString(typeid(T).name());

    if (!IQORMCore::_models.contains(objectTypeName) || !IQORMCore::_models[objectTypeName])
    {
        //Создадим новую модель
        IQORMModel *newModel = new IQORMModel();
        T object;
        newModel->setObjectClassName(object.metaObject()->className());
        newModel->setTempInitObject(&object);
        object.initializeORMModel(newModel);
        IQORMCore::_models[objectTypeName] = newModel;
        newModel->setTempInitObject(NULL);
    }

    IQORMModel *result = _models[objectTypeName];

    return result;
}

#endif // IQORMCORE_H
