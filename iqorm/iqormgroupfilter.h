#ifndef IQORMGROUPFILTER_H
#define IQORMGROUPFILTER_H

#include "iqormabstractfilter.h"
#include <QtQml/QQmlListProperty>

class IQORMGroupFilter : public IQORMAbstractFilter
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<IQORMAbstractFilter> filters READ qmlFilters)

    Q_CLASSINFO("DefaultProperty", "filters")

public:
    enum GroupType
    {
        And,
        Or
    };

    explicit IQORMGroupFilter(QObject *parent = 0);

    void add(IQORMAbstractFilter *filter);

    void remove(IQORMAbstractFilter *filter);

    IQORMAbstractFilter *at(const int index) const;

    void clear();

    int count() const;

    inline QList<IQORMAbstractFilter*> toList() const {return _filters;}

    QQmlListProperty<IQORMAbstractFilter> qmlFilters();

    inline GroupType type() const {return _type;}

    void setType(const GroupType type);

private:
    QList<IQORMAbstractFilter *> _filters;
    GroupType _type;
};

#endif // IQORMGROUPFILTER_H
