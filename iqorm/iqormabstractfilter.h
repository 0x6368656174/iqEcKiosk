#ifndef IQORMABSTRACTFILTER_H
#define IQORMABSTRACTFILTER_H

#include <QObject>

class IQORMAbstractFilter : public QObject
{
    Q_OBJECT
public:
    explicit IQORMAbstractFilter(QObject *parent = 0);
};

#endif // IQORMABSTRACTFILTER_H
