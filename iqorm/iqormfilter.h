#ifndef IQORMFILTER_H
#define IQORMFILTER_H

#include "iqormabstractfilter.h"
#include <QVariant>

class IQORMFilter : public IQORMAbstractFilter
{
    Q_OBJECT
    Q_PROPERTY(Operation operation READ operation WRITE setOperation NOTIFY operationChanged)
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY propertyChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ caseSensitivity WRITE setCaseSensitivity NOTIFY caseSensitivityChanged)

    Q_ENUMS(Operation)
public:
    enum Operation
    {
        None,
        Equals,
        NotEquals,
        GreaterThan,
        LessThan,
        GreaterOrEquals,
        LessOrEquals,
        StartsWith,
        EndsWith,
        Contains
    };

    explicit IQORMFilter(QObject *parent = 0);

    explicit IQORMFilter(const QString &property, const Operation operation, const QVariant &value, QObject *parent = 0);

public:

    inline Operation operation() const {return _operation;}

    void setOperation(const Operation operation);

    inline QString property() const {return _property;}

    void setProperty(const QString &property);

    inline QVariant value() const {return _value;}

    void setValue(const QVariant &value);

    inline Qt::CaseSensitivity caseSensitivity() const {return _caseSensitivity;}

    void setCaseSensitivity(const Qt::CaseSensitivity caseSensitivity);

signals:
    void operationChanged();
    void propertyChanged();
    void valueChanged();
    void caseSensitivityChanged();

private:
    Operation _operation;
    QString _property;
    QVariant _value;
    Qt::CaseSensitivity _caseSensitivity;
};

#endif // IQORMFILTER_H
