#include "iqormerror.h"

IQORMError::IQORMError(QObject *parent) :
    QObject(parent),
    _oldText(""),
    _oldError(false)
{
}

void IQORMError::setErrors(const QStringList &errors)
{
    if (_errors == errors)
        return;

    _errors = errors;

    if (_oldText != text())
        emit textChanged();
    _oldText = text();

    if (_oldError != error())
        emit errorChanged();
    _oldError = error();
}

void IQORMError::addError(const QString &errorText)
{
    if (_errors.contains(errorText))
        return;

    _errors.append(errorText);

    if (_oldText != text())
        emit textChanged();
    _oldText = text();

    if (_oldError != error())
        emit errorChanged();
    _oldError = error();
}

void IQORMError::clearErrors()
{
    _errors.clear();

    if (_oldText != text())
        emit textChanged();
    _oldText = text();

    if (_oldError != error())
        emit errorChanged();
    _oldError = error();
}

QString IQORMError::text() const
{
    QString result;

    for(int i = 1; i < _errors.count() + 1; i++)
    {
        result.append(QString("%0. %1\n\n")
                      .arg(i)
                      .arg(_errors[i - 1]));
    }

    return result.trimmed();
}

bool IQORMError::error() const
{
    return _errors.count() > 0;
}
