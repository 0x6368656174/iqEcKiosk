#include "iqvcardoperation.h"
#include "iqormmodel.h"

IQVCardOperation::IQVCardOperation(QObject *parent) :
    QObject(parent),
    IQORMObject()
{
    reset();
}

void IQVCardOperation::initializeORMModel(IQORMModel *model) const
{
    model->setTableName("v_IQ_CardOperation");
    model->setPropertyColumn("gameName", "GameName");
    model->setPropertyColumn("operationName", "OperationName");
    model->setPropertyColumn("loyaltyCardId", "LoyaltyCardID");
    model->setPropertyColumn("amount", "Amount");
    model->setPropertyColumn("points", "Points");
    model->setPropertyColumn("operationDate", "OperationDate");
}

void IQVCardOperation::reset()
{
    setGameName("");
    setOperationName("");
    setLoyaltyCardId(-1);
    setAmount(0);
    setPoints(0);
    setOperationDate(QDateTime());
}

void IQVCardOperation::setLoyaltyCardId(const qint64 loyaltyCardId)
{
    if (_loyaltyCardId != loyaltyCardId)
    {
        _loyaltyCardId = loyaltyCardId;

        emit loyaltyCardIdChanged();
    }
}

void IQVCardOperation::setAmount(const qreal amount)
{
    if (_amount != amount)
    {
        _amount = amount;

        emit amountChanged();
    }
}

void IQVCardOperation::setPoints(const qreal points)
{
    if (_points != points)
    {
        _points = points;

        emit pointsChanged();
    }
}

void IQVCardOperation::setOperationDate(const QDateTime &date)
{
    if (_operationDate != date)
    {
        _operationDate = date;

        emit operationDateChanged();
    }
}

void IQVCardOperation::setGameName(const QString &gameName)
{
    if (_gameName != gameName)
    {
        _gameName = gameName;

        emit gameNameChanged();
    }
}

void IQVCardOperation::setOperationName(const QString &operationName)
{
    if (_operationName != operationName)
    {
        _operationName = operationName;

        emit operationNameChanged();
    }
}
