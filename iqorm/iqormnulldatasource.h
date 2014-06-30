#ifndef IQORMNULLDATASOURCE_H
#define IQORMNULLDATASOURCE_H

#include "iqormabstractdatasource.h"

class IQORMNullDataSource : public IQORMAbstractDataSource
{
    Q_OBJECT
public:
    explicit IQORMNullDataSource(QObject *parent = 0);

    virtual IQORMAbstractObjectDataSource * objectDataSource() const {printWarnign(); return NULL;}

    virtual IQORMAbstractObjectsModelDataSource * objectsModelDataSource() const {printWarnign(); return NULL;}

private:
    void printWarnign() const;
};

#endif // IQORMNULLDATASOURCE_H
