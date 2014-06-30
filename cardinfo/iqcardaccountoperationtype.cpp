#include "iqcardaccountoperationtype.h"
#include "iqormmodel.h"

IQCardAccountOperationType::IQCardAccountOperationType(QObject *parent) :
    QObject(parent),
    IQORMObject()
{
    reset();
}

void IQCardAccountOperationType::reset()
{
    setName("");
}

void IQCardAccountOperationType::initializeORMModel(IQORMModel *model) const
{
    model->setTableName("CardAccountOperationType");
    model->setPropertyColumn("name", "Name");
}

void IQCardAccountOperationType::setName(const QString &name)
{
    if (_name != name)
    {
        _name = name;

        emit nameChanged();
    }
}
