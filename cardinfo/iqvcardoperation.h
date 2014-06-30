#ifndef IQVCARDOPERATION_H
#define IQVCARDOPERATION_H

#include <QObject>
#include <QDateTime>
#include "iqormobject.h"

class IQVCardOperation : public QObject, public IQORMObject
{
    Q_OBJECT
    IQORM_OBJECT
    Q_PROPERTY(QString operationName READ operationName WRITE setOperationName NOTIFY operationNameChanged)
    Q_PROPERTY(QString gameName READ gameName WRITE setGameName NOTIFY gameNameChanged)
    Q_PROPERTY(qint64 loyaltyCardId READ loyaltyCardId WRITE setLoyaltyCardId NOTIFY loyaltyCardIdChanged)
    Q_PROPERTY(qreal amount READ amount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(qreal points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(QDateTime operationDate READ operationDate WRITE setOperationDate NOTIFY operationDateChanged)

public:
    explicit IQVCardOperation(QObject *parent = 0);

    virtual void reset();

    virtual void initializeORMModel(IQORMModel *model) const;

public:
    inline qint64 loyaltyCardId() const {return _loyaltyCardId;}
    void setLoyaltyCardId(const qint64 loyaltyCardId);

    inline QString gameName() const {return _gameName;}
    void setGameName(const QString &gameName);

    inline QString operationName() const {return _operationName;}
    void setOperationName(const QString &operationName);

    inline qreal amount() const {return _amount;}
    void setAmount(const qreal amount);

    inline qreal points() const {return _points;}
    void setPoints(const qreal points);

    inline QDateTime operationDate() const {return _operationDate;}
    void setOperationDate(const QDateTime &date);

signals:
    void gameNameChanged();
    void operationNameChanged();
    void cardSecretChanged();
    void amountChanged();
    void pointsChanged();
    void operationDateChanged();
    void loyaltyCardIdChanged();

private:
    QString _gameName;
    QString _operationName;
    qreal _amount;
    qreal _points;
    QDateTime _operationDate;
    qint64 _loyaltyCardId;
};

#endif // IQVCARDOPERATION_H
