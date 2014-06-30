#include "iqgameobject.h"
#include "iqormmodel.h"

IQGameObject::IQGameObject(QObject *parent) :
    QObject(parent),
    IQORMObject()
{
    reset();
}

void IQGameObject::reset()
{
    setName("");
}

void IQGameObject::initializeORMModel(IQORMModel *model) const
{
    model->setTableName("GameObject");
    model->setPropertyColumn("name", "Name");
}

void IQGameObject::setName(const QString &name)
{
    if (_name != name)
    {
        _name = name;

        emit nameChanged();
    }
}
