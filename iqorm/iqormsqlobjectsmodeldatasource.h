#ifndef IQORMSQLOBJECTSMODELDATASOURCE_H
#define IQORMSQLOBJECTSMODELDATASOURCE_H

#include "iqormabstractobjectsmodeldatasource.h"
#include <QSqlDriver>

class IQORMAbstractFilter;

class IQORMSQLObjectsModelDataSource : public IQORMAbstractObjectsModelDataSource
{
    Q_OBJECT
public:
    explicit IQORMSQLObjectsModelDataSource(IQORMSQLDataSource *sqlDataSource = 0);

    virtual bool loadModel(IQORMObjectsModelBase *model,
                           const qint64 limit = -1,
                           const qint64 offset = 0);

    virtual QList<qint64> findObjects(const IQORMModel *ormModel,
                                      const IQORMAbstractFilter *filter,
                                      const qint64 limit = -1,
                                      const qint64 offset = 0,
                                      bool *ok = NULL,
                                      QString *errorMessage = NULL) const;

private:
    IQORMSQLDataSource* _sqlDataSource;

    QString filterString(const IQORMModel *ormModel,
                         const IQORMAbstractFilter *filter,
                         QList<QVariant> *bindValues) const;

    QStringList referencingTableNames(const IQORMModel *ormModel,
                                      const IQORMAbstractFilter *filter) const;
};

#endif // IQORMSQLOBJECTSMODELDATASOURCE_H
