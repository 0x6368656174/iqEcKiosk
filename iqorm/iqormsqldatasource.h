#ifndef IQORMSQLDATASOURCE_H
#define IQORMSQLDATASOURCE_H

#include "iqormabstractdatasource.h"
#include "iqormsqlobjectdatasource.h"
#include "iqormsqlobjectsmodeldatasource.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlDatabase>
#include <QHash>
#include <QVariant>

class IQORMObject;
class IQORMSQLObjectDataSource;
class IQORMSQLObjectsModelDataSource;

class IQORMSQLDataSource : public IQORMAbstractDataSource
{
    Q_OBJECT
public:
    explicit IQORMSQLDataSource(QObject *parent = 0);

    void setDatabase(QSqlDatabase &database);

    inline virtual IQORMAbstractObjectDataSource *objectDataSource() const {return _objectDataSource;}

    inline virtual IQORMAbstractObjectsModelDataSource *objectsModelDataSource() const {return _objectsModelDataSource;}

private:
    mutable QSqlDatabase _database;
    IQORMSQLObjectDataSource *_objectDataSource;
    IQORMSQLObjectsModelDataSource *_objectsModelDataSource;

    inline QSqlDatabase database () const {return _database;}

    bool openDB() const;

    QSqlQuery execQuery(const QString &prepareString,
                        const QList<QVariant> bindValues = QList<QVariant>(),
                        bool forwardOnly = true,
                        bool *ok = NULL,
                        QString *errorText = NULL) const;

    static QString escapedPropertyName(const QString &propertyName);

    friend class IQORMSQLObjectDataSource;
    friend class IQORMSQLObjectsModelDataSource;
};

#endif // IQORMSQLDATASOURCE_H
