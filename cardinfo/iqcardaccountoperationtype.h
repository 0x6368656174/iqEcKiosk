#ifndef IQCARDACCOUNTOPERATIONTYPE_H
#define IQCARDACCOUNTOPERATIONTYPE_H

#include <QObject>
#include "iqormobject.h"

class IQCardAccountOperationType : public QObject, public IQORMObject
{
    Q_OBJECT
    IQORM_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit IQCardAccountOperationType(QObject *parent = 0);

    virtual void reset();

    virtual void initializeORMModel(IQORMModel *model) const;

public:
    inline QString name() const {return _name;}
    void setName(const QString &name);

signals:
    void nameChanged();

private:
    QString _name;
};

#endif // IQCARDACCOUNTOPERATIONTYPE_H
