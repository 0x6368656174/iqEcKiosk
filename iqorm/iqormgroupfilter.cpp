#include "iqormgroupfilter.h"

IQORMGroupFilter::IQORMGroupFilter(QObject *parent) :
    IQORMAbstractFilter(parent),
    _type(Or)
{
}

QQmlListProperty<IQORMAbstractFilter> IQORMGroupFilter::qmlFilters()
{
    return QQmlListProperty<IQORMAbstractFilter>(this, _filters);
}

int IQORMGroupFilter::count() const
{
    return _filters.count();
}

void IQORMGroupFilter::add(IQORMAbstractFilter *filter)
{
    if (!filter)
        return;
    filter->setParent(this);

    _filters.append(filter);
}

void IQORMGroupFilter::remove(IQORMAbstractFilter *filter)
{
    if (!filter)
        return;
    filter->setParent(NULL);

    _filters.removeAll(filter);
}

IQORMAbstractFilter* IQORMGroupFilter::at(const int index) const
{
    if (index < 0 || index >= _filters.count())
        return NULL;

    return _filters.at(index);
}

void IQORMGroupFilter::clear()
{
    qDeleteAll(_filters);
    _filters.clear();
}

void IQORMGroupFilter::setType(const GroupType type)
{
    if (_type == type)
        return;
    _type = type;
}
