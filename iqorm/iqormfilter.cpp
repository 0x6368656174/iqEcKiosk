#include "iqormfilter.h"

IQORMFilter::IQORMFilter(QObject *parent) :
    IQORMAbstractFilter(parent),
    _operation(None),
    _property(""),
    _value(QVariant()),
    _caseSensitivity(Qt::CaseSensitive)
{
}

IQORMFilter::IQORMFilter(const QString &property, const Operation operation, const QVariant &value, QObject *parent) :
    IQORMAbstractFilter(parent),
    _operation(operation),
    _property(property),
    _value(value),
    _caseSensitivity(Qt::CaseSensitive)
{
}

void IQORMFilter::setOperation(const Operation operation)
{
    if (_operation == operation)
        return;
    _operation = operation;

    emit operationChanged();
}

void IQORMFilter::setProperty(const QString &property)
{
    if (_property == property)
        return;
    _property = property;

    emit propertyChanged();
}

void IQORMFilter::setValue(const QVariant &value)
{
    if (_value == value)
        return;
    _value = value;

    emit valueChanged();
}

void IQORMFilter::setCaseSensitivity(const Qt::CaseSensitivity caseSensitivity)
{
    if (_caseSensitivity == caseSensitivity)
        return;
    _caseSensitivity = caseSensitivity;

    emit caseSensitivityChanged();
}
