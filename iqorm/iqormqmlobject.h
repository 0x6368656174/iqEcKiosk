#ifndef IQORMQMLOBJECT_H
#define IQORMQMLOBJECT_H

#include <QObject>
#include <QQmlParserStatus>
#include "iqormobject.h"
#include "iqormmodel.h"

class IQORMQmlObject : public QObject, public IQORMObject, public QQmlParserStatus
{
    Q_OBJECT
    IQORM_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(IQORMModel * model READ model WRITE setModel NOTIFY modelChanged)
public:
    explicit IQORMQmlObject(QObject *parent = 0);

    virtual void reset();
    virtual void initializeORMModel(IQORMModel *model) const;

    virtual void classBegin();
    virtual void componentComplete();

    inline IQORMModel * ormModel() const {return model();}

public:
    inline IQORMModel *model() const {return _model;}
    void setModel(IQORMModel *model);

signals:
    void modelChanged();
private:
    IQORMModel *_model;
    QVariantHash _defaultValues;
};

#endif // IQORMQMLOBJECT_H
