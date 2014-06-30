#include "iqormsqldatasource.h"
#include "iqormobject.h"
#include "iqormmodel.h"
#include "iqormpropertydescription.h"
#include <QMetaProperty>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

IQORMSQLDataSource::IQORMSQLDataSource(QObject *parent) :
    IQORMAbstractDataSource(parent),
    _objectDataSource(new IQORMSQLObjectDataSource(this)),
    _objectsModelDataSource(new IQORMSQLObjectsModelDataSource(this))
{
}

void IQORMSQLDataSource::setDatabase(QSqlDatabase &database)
{
    _database = database;
}

bool IQORMSQLDataSource::openDB() const
{
    if (!_database.isValid())
    {
        qWarning() << "Database not valid. Use IQORMSQLDataSource::setDatabase() first.";
        return false;
    }

    if (!_database.isOpen() && !_database.open())
    {
        qWarning() << QString("Database open failed. SQL Error: \"%0\"").arg(_database.lastError().text());
        return false;
    }

    return true;
}

QSqlQuery IQORMSQLDataSource::execQuery(const QString &prepareString,
                                        const QList<QVariant> bindValues,
                                        bool forwardOnly,
                                        bool *ok,
                                        QString *errorText) const
{
    if (ok)
        *ok = false;

    //Готовим запрос
    QSqlQuery query (_database);
    query.setForwardOnly(forwardOnly);

    if (!openDB())
        return query;

    //Если не удалось подготовить запрос, то выходим
    if (!query.prepare(prepareString))
    {
        if (errorText)
            *errorText = tr("Can not prepare SQL query \"%0\". \nSQL Engine return error: \"%1\".")
                .arg(query.executedQuery())
                .arg(query.lastError().text());
        return query;
    }

    //Биндим параметры
    foreach (QVariant value, bindValues)
    {
        query.addBindValue(value);
    }

    //Если не удалось выполнить запрос, то выходим
    if (!query.exec())
    {
        if (errorText)
            *errorText = tr("Can not exec SQL query \"%0\". \nSQL Engine return error: \"%1\".")
                .arg(query.executedQuery())
                .arg(query.lastError().text());
        return query;
    }

    //Все ок
    if (ok)
        *ok = true;
    return query;
}

QString IQORMSQLDataSource::escapedPropertyName(const QString &propertyName)
{
    return propertyName.split(".").join("");
}
