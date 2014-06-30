#ifndef IQORMERROR_H
#define IQORMERROR_H

#include <QObject>
#include <QStringList>

class IQORMError : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text NOTIFY textChanged)
    Q_PROPERTY(bool error READ error NOTIFY errorChanged)
    Q_PROPERTY(QStringList errors READ errors WRITE setErrors NOTIFY errorsChanged)

public:
    explicit IQORMError(QObject *parent = 0);

    void addError(const QString &errorText);

    void clearErrors();

public:
    QString text() const;

    bool error() const;

    inline QStringList errors() const {return _errors;}
    void setErrors(const QStringList &errors);

signals:
    void textChanged();
    void errorChanged();
    void errorsChanged();

private:
    QStringList _errors;
    QString _oldText;
    bool _oldError;
};

#endif // IQORMERROR_H
