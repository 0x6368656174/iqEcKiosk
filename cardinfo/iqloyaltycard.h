#ifndef IQLOYALTYCARD_H
#define IQLOYALTYCARD_H

#include "iqormobject.h"

class IQLoyaltyCard : public QObject, public IQORMObject
{
    Q_OBJECT
    IQORM_OBJECT
    Q_PROPERTY(QString cardSecret READ cardSecret WRITE setCardSecret NOTIFY cardSecretChanged)
    Q_PROPERTY(QString customerShortName READ customerShortName WRITE setCustomerShortName NOTIFY customerShortNameChanged)
    Q_PROPERTY(QString cardNumber READ cardNumber WRITE setCardNumber NOTIFY cardNumberChanged)
    Q_PROPERTY(qint64 cardUsageId READ cardUsageId WRITE setCardUsageId NOTIFY cardUsageIdChanged)
    Q_PROPERTY(qreal balance READ balance WRITE setBalance NOTIFY balanceChanged)
    Q_PROPERTY(qreal pointsBalance READ pointsBalance WRITE setPointsBalance NOTIFY pointsBalanceChanged)
public:
    explicit IQLoyaltyCard(QObject *parent = 0);

    virtual void reset();

    virtual void initializeORMModel(IQORMModel *model) const;

public:
    inline QString cardSecret() const {return _cardSecret;}
    void setCardSecret(const QString &secret);

    inline QString customerShortName() const {return _customerShortName;}
    void setCustomerShortName(const QString &customerShortName);

    inline QString cardNumber() const {return _cardNumber;}
    void setCardNumber(const QString &cardNumber);

    inline qint64 cardUsageId() const {return _cardUsageId;}
    void setCardUsageId(const qint64 cardUsageId);

    inline qreal balance() const {return _balance;}
    void setBalance(const qreal balance);

    inline qreal pointsBalance() const {return _pointsBalance;}
    void setPointsBalance(const qreal pointsBalance);

signals:
    void cardSecretChanged();
    void customerShortNameChanged();
    void cardNumberChanged();
    void cardUsageIdChanged();
    void balanceChanged();
    void pointsBalanceChanged();

private:
    QString _cardSecret;
    QString _customerShortName;
    QString _cardNumber;
    qint64 _cardUsageId;
    qreal _balance;
    qreal _pointsBalance;
};

#endif // IQLOYALTYCARD_H
