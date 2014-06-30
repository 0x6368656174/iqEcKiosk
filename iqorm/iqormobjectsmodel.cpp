#include "iqormobjectsmodel.h"

IQORMObjectsModelBase::IQORMObjectsModelBase(QObject *parent) :
    QAbstractTableModel(parent),
    _filters(NULL),
    _lastError(new IQORMError(this))
{
}

IQORMObjectsModelBase::~IQORMObjectsModelBase()
{
}

void IQORMObjectsModelBase::onObjectChanged()
{
    QObject * senderItem = sender();
    if (!senderItem)
        return;

    if (senderSignalIndex() < 0)
        return;

    QString property = _propertySignalsIndex[senderSignalIndex()];
    if (property.isEmpty())
        return;

    emitDataChangedSignal(senderItem, property);
}

void IQORMObjectsModelBase::setFilters(IQORMAbstractFilter *filters)
{
    if (_filters == filters)
        return;
    _filters = filters;

    filters->setParent(this);

    emit filtersChanged();
}
