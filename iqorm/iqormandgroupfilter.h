#ifndef IQORMANDGROUPFILTER_H
#define IQORMANDGROUPFILTER_H

#include "iqormgroupfilter.h"

class IQORMAndGroupFilter : public IQORMGroupFilter
{
    Q_OBJECT
public:
    explicit IQORMAndGroupFilter(QObject *parent = 0);
};

#endif // IQORMANDGROUPFILTER_H
