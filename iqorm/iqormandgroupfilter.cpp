#include "iqormandgroupfilter.h"

IQORMAndGroupFilter::IQORMAndGroupFilter(QObject *parent) :
    IQORMGroupFilter(parent)
{
    setType(And);
}
