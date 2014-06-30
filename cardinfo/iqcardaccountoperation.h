#ifndef IQCARDACCOUNTOPERATION_H
#define IQCARDACCOUNTOPERATION_H

#include <QObject>
#include <QDateTime>
#include "iqormobject.h"

class IQCardAccountOperation : public QObject, public IQORMObject
{
    Q_OBJECT
    IQORM_OBJECT
    Q_PROPERTY(qint64 loyaltyCardId READ loyaltyCardId WRITE setLoyaltyCardId NOTIFY loyaltyCardIdChanged)
    Q_PROPERTY(qint64 cardAccountOperationTypeId READ cardAccountOperationTypeId WRITE setCardAccountOperationTypeId NOTIFY cardAccountOperationTypeIdChanged)
    Q_PROPERTY(qint64 gameObjectId READ gameObjectId WRITE setGameObjectId NOTIFY gameObjectIdChanged)
    Q_PROPERTY(qreal amount READ amount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(qreal points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(QDateTime operationDate READ operationDate WRITE setOperationDate NOTIFY operationDateChanged)
public:
    explicit IQCardAccountOperation(QObject *parent = 0);

    virtual void reset();

    virtual void initializeORMModel(IQORMModel *model) const;

public:
    inline qint64 loyaltyCardId() const {return _loyaltyCardId;}
    void setLoyaltyCardId(const qint64 loyaltyCardId);

    inline qint64 cardAccountOperationTypeId() const {return _cardAccountOperationTypeId;}
    void setCardAccountOperationTypeId(const qint64 id);

    inline qint64 gameObjectId() const {return _gameObjectId;}
    void setGameObjectId(const qint64 id);

    inline qreal amount() const {return _amount;}
    void setAmount(const qreal amount);

    inline qreal points() const {return _points;}
    void setPoints(const qreal points);

    inline QDateTime operationDate() const {return _operationDate;}
    void setOperationDate(const QDateTime &date);

signals:
    void loyaltyCardIdChanged();
    void cardAccountOperationTypeIdChanged();
    void gameObjectIdChanged();
    void amountChanged();
    void pointsChanged();
    void operationDateChanged();

private:
    qint64 _loyaltyCardId;
    qint64 _cardAccountOperationTypeId;
    qint64 _gameObjectId;
    qreal _amount;
    qreal _points;
    QDateTime _operationDate;
};

#endif // IQCARDACCOUNTOPERATION_H
