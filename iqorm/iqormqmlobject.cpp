#include "iqormqmlobject.h"
#include <QMetaProperty>

IQORMQmlObject::IQORMQmlObject(QObject *parent) :
    QObject(parent),
    IQORMObject(),
    QQmlParserStatus(),
    _model(NULL)
{
}

void IQORMQmlObject::classBegin()
{
}

void IQORMQmlObject::componentComplete()
{
    for(int i = 0; i < metaObject()->propertyCount(); i++)
    {
        _defaultValues[QString(metaObject()->property(i).name())] = property(metaObject()->property(i).name());
    }
}

void IQORMQmlObject::reset()
{
    QHashIterator<QString, QVariant> defaultValuesI (_defaultValues);
    while (defaultValuesI.hasNext())
    {
        defaultValuesI.next();

        setProperty(defaultValuesI.key().toLocal8Bit().constData(), defaultValuesI.value());
    }
}

void IQORMQmlObject::initializeORMModel(IQORMModel *model) const
{
    Q_UNUSED(model);
}

void IQORMQmlObject::setModel(IQORMModel *model)
{
    if (_model != model)
    {
        if (_model)
            _model->deleteLater();
        _model = model;

        emit modelChanged();
    }
}
