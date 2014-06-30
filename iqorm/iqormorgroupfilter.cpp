#include "iqormorgroupfilter.h"

IQORMOrGroupFilter::IQORMOrGroupFilter(QObject *parent) :
    IQORMGroupFilter(parent)
{
    setType(Or);
}
