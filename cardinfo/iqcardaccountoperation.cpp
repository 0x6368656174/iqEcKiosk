#include "iqcardaccountoperation.h"
#include "iqormmodel.h"

IQCardAccountOperation::IQCardAccountOperation(QObject *parent) :
    QObject(parent),
    IQORMObject()
{
    reset();
}

void IQCardAccountOperation::reset()
{
    setLoyaltyCardId(-1);
    setCardAccountOperationTypeId(-1);
    setGameObjectId(-1);
    setAmount(0);
    setPoints(0);
    setOperationDate(QDateTime());
}

void IQCardAccountOperation::initializeORMModel(IQORMModel *model) const
{
    model->setTableName("CardAccountOperation");
    model->setPropertyColumn("loyaltyCardId", "LoyaltyCardID");
    model->setPropertyColumn("cardAccountOperationTypeId", "CardAccountOperationTypeID");
    model->setPropertyColumn("gameObjectId", "GameObjectID");
    model->setPropertyColumn("amount", "Amount");
    model->setPropertyColumn("points", "Points");
    model->setPropertyColumn("operationDate", "OperationDate");
}

void IQCardAccountOperation::setLoyaltyCardId(const qint64 loyaltyCardId)
{
    if (_loyaltyCardId != loyaltyCardId)
    {
        _loyaltyCardId = loyaltyCardId;

        emit loyaltyCardIdChanged();
    }
}

void IQCardAccountOperation::setCardAccountOperationTypeId(const qint64 id)
{
    if (_cardAccountOperationTypeId != id)
    {
        _cardAccountOperationTypeId = id;

        emit cardAccountOperationTypeIdChanged();
    }
}

void IQCardAccountOperation::setGameObjectId(const qint64 id)
{
    if (_gameObjectId != id)
    {
        _gameObjectId = id;

        emit gameObjectIdChanged();
    }
}

void IQCardAccountOperation::setAmount(const qreal amount)
{
    if (_amount != amount)
    {
        _amount = amount;

        emit amountChanged();
    }
}

void IQCardAccountOperation::setPoints(const qreal points)
{
    if (_points != points)
    {
        _points = points;

        emit pointsChanged();
    }
}

void IQCardAccountOperation::setOperationDate(const QDateTime &date)
{
    if (_operationDate != date)
    {
        _operationDate = date;

        emit operationDateChanged();
    }
}
