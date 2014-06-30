#ifndef IQCARDCONTROLLER_H
#define IQCARDCONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class IQCardController : public QObject
{
    Q_OBJECT
public:
    explicit IQCardController(QObject *parent = 0);

    Q_INVOKABLE void connectToMitec();

    Q_INVOKABLE void disconnectFromMitec();

signals:
    void cardFound(const QString &cardSecret);

private slots:
    void sendBread();
    void reconect();
    void readDataFromSocket();

private:
    QString _mitecHost;
    quint16 _mitecPort;

    QTcpSocket *_mitecSocket;
    QTimer *_breadTimer;
    QTimer *_reconectTimer;

};

#endif // IQCARDCONTROLLER_H
