#include "iqloyaltycard.h"
#include "iqormmodel.h"

IQLoyaltyCard::IQLoyaltyCard(QObject *parent) :
    QObject(parent),
    IQORMObject()
{
    reset();
}

void IQLoyaltyCard::setCardNumber(const QString &cardNumber)
{
    if (cardNumber != _cardNumber)
    {
        _cardNumber = cardNumber;

        emit cardNumberChanged();
    }
}

void IQLoyaltyCard::setCardSecret(const QString &secret)
{
    if (_cardSecret != secret)
    {
        _cardSecret = secret;

        emit cardSecretChanged();
    }
}

void IQLoyaltyCard::setCustomerShortName(const QString &customerShortName)
{
    if (_customerShortName != customerShortName)
    {
        _customerShortName = customerShortName;

        emit customerShortNameChanged();
    }
}

void IQLoyaltyCard::setCardUsageId(const qint64 cardUsageId)
{
    if (_cardUsageId != cardUsageId)
    {
        _cardUsageId = cardUsageId;

        emit cardUsageIdChanged();
    }
}

void IQLoyaltyCard::setBalance(const qreal balanse)
{
    if (_balance != balanse)
    {
        _balance = balanse;

        emit balanceChanged();
    }
}

void IQLoyaltyCard::setPointsBalance(const qreal pointBalanse)
{
    if (_pointsBalance != pointBalanse)
    {
        _pointsBalance = pointBalanse;

        emit pointsBalanceChanged();
    }
}

void IQLoyaltyCard::reset()
{
    setCardNumber("");
    setCardSecret("");
    setCustomerShortName("");
    setCardUsageId(-1);
    setBalance(0);
    setPointsBalance(0);
}

void IQLoyaltyCard::initializeORMModel(IQORMModel *model) const
{
    model->setTableName("v_LoyaltyCard");
    model->setPropertyColumn("cardNumber", "CardNumber");
    model->setPropertyColumn("cardSecret", "CardSecret");
    model->setPropertyColumn("customerShortName", "CustomerShortName");
    model->setPropertyColumn("cardUsageId", "CardUsageID");
    model->setPropertyColumn("balance", "Balance");
    model->setPropertyColumn("pointsBalance", "PointsBalance");
}
