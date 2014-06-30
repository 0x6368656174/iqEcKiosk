#include "iqormnulldatasource.h"
#include <QDebug>

IQORMNullDataSource::IQORMNullDataSource(QObject *parent) :
    IQORMAbstractDataSource(parent)
{
}

void IQORMNullDataSource::printWarnign() const
{
    qWarning() << "Data source not set. Use IQORMCore::setDataSource() first.";
}
