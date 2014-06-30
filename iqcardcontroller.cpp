#include "iqcardcontroller.h"
#include <QSettings>
#include <QRegExp>
#include <QDebug>

IQCardController::IQCardController(QObject *parent) :
    QObject(parent),
    _mitecHost("127.0.0.1"),
    _mitecPort(5556),
    _mitecSocket(new QTcpSocket()),
    _breadTimer(new QTimer()),
    _reconectTimer(new QTimer())
{
    QSettings settings;
    if (!settings.contains("breadInterval"))
        settings.setValue("breadInterval", 1000);

    if (!settings.contains("breadIdleTimeout"))
        settings.setValue("breadIdleTimeout", 5000);

    if (!settings.contains("mitecHost"))
        settings.setValue("mitecHost", "127.0.0.1");

    if (!settings.contains("mitecPort"))
        settings.setValue("mitecPort", 5556);

    _mitecHost = settings.value("mitecHost").toString();
    _mitecPort = settings.value("mitecPort").toUInt();

    _breadTimer->setInterval(settings.value("breadInterval").toInt());
    _reconectTimer->setInterval(settings.value("breadIdleTimeout").toInt());

    connect(_breadTimer, SIGNAL(timeout()), this, SLOT(sendBread()));
    connect(_reconectTimer, SIGNAL(timeout()), this, SLOT(reconect()));
    connect(_mitecSocket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));
}

void IQCardController::sendBread()
{
    if (_mitecSocket->state() == QTcpSocket::ConnectedState)
        _mitecSocket->write("ver\n");
}

void IQCardController::reconect()
{
    if (_mitecSocket->state() != QTcpSocket::UnconnectedState)
    {
        _mitecSocket->disconnectFromHost();
        if (_mitecSocket->state() != QTcpSocket::UnconnectedState)
            _mitecSocket->waitForDisconnected();
    }

    qWarning() << "Reconect to mitec...";
    _mitecSocket->connectToHost(_mitecHost, _mitecPort);
}

void IQCardController::readDataFromSocket()
{
    QByteArray data = _mitecSocket->readAll();

    QString dataString (data);

    //Найдем ответ на дыхание
    QRegExp verRX ("v\\d\\.\\d{2}\\.\\d");
    if (verRX.indexIn(dataString) > -1)
    {
        _reconectTimer->start();
        return;
    }

    //Найдем информацию о том, что поднесли карточку
    QRegExp cardRX ("\\d{4} CARD ([\\dFA]{2} [\\dFA]{2} [\\dFA]{2} [\\dFA]{2} [\\dFA]{2} [\\dFA]{2} "
                    "[\\dFA]{2} [\\dFA]{2} [\\dFA]{2} [\\dFA]{2} [\\dFA]{2} [\\dFA]{2} [\\dFA]{2} "
                    "[\\dFA]{2} [\\dFA]{2} [\\dFA]{2})");

    if (cardRX.indexIn(dataString) > -1)
    {
        QString cardSecret = cardRX.cap(1);
        cardSecret.replace(" ", "");
        cardSecret.replace("F", "");
        cardSecret.replace("A", "=");
        emit cardFound(cardSecret);
    }
}

void IQCardController::connectToMitec()
{
    if (_mitecSocket->state() != QTcpSocket::UnconnectedState)
        return;

        qWarning() << "Conect to mitec...";
        _breadTimer->start();
        _reconectTimer->start();
        _mitecSocket->connectToHost(_mitecHost, _mitecPort);
}

void IQCardController::disconnectFromMitec()
{
    _breadTimer->stop();
    _reconectTimer->stop();
    _mitecSocket->disconnectFromHost();
    qWarning() << "Disconect from mitec...";
}
