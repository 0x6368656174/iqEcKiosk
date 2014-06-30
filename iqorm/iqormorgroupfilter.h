#ifndef IQORMORGROUPFILTER_H
#define IQORMORGROUPFILTER_H

#include "iqormgroupfilter.h"

class IQORMOrGroupFilter : public IQORMGroupFilter
{
    Q_OBJECT
public:
    explicit IQORMOrGroupFilter(QObject *parent = 0);
};

#endif // IQORMORGROUPFILTER_H
