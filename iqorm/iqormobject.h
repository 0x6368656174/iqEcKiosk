#ifndef IQORMOBJECT_H
#define IQORMOBJECT_H

#include <QObject>
#include <QPointer>
#include "iqormerror.h"



#define IQORM_OBJECT \
    Q_PROPERTY(qint64 objectId READ objectId WRITE setObjectId NOTIFY objectIdChanged) \
    Q_PROPERTY(bool autoLoadReference READ autoLoadReference WRITE setAutoLoadReference NOTIFY autoLoadReferenceChanged) \
    Q_PROPERTY(IQORMError * lastError READ lastError CONSTANT) \
\
protected: \
    virtual void emitObjectIdChanged() {emit objectIdChanged();} \
\
    virtual void emitAutoLoadReference() {emit autoLoadReferenceChanged();}\
\
public: \
    Q_SIGNAL void objectIdChanged(); \
\
    Q_SIGNAL void autoLoadReferenceChanged(); \
\
    Q_INVOKABLE bool load(const qint64 objectId, IQORMAbstractDataSource *dataSource = 0) {return IQORMObject::load(objectId, dataSource);} \
\
    Q_INVOKABLE bool reload(IQORMAbstractDataSource *dataSource = 0) {return IQORMObject::reload(dataSource);} \
\
    Q_INVOKABLE bool save(IQORMAbstractDataSource *dataSource = 0) {return IQORMObject::save(dataSource);} \
\
    Q_INVOKABLE bool remove(IQORMAbstractDataSource *dataSource = 0) {return IQORMObject::remove(dataSource);}





class IQORMAbstractDataSource;
class IQORMSQLDataSource;
class IQORMLocalSynchronizer;
class IQORMModel;

class IQORMObject
{
public:
    explicit IQORMObject();

    virtual ~IQORMObject();

    /*!
     * \brief Сбрасывает все параметры объекта
     *
     * Проходит по всем параметрам объекта и сбрасывает их
     */
    virtual void reset() = 0;

    void resetReference();

    /*!
     * \brief Инециализирует ORM-модель, связаную с данным объектом
     *
     * Для работы ORM, используются специальные модели, в которых описано какие параметры и как
     * храняться в источниках данных. Данный метод вызывается объектом, имеющим интерфейсы ORM,
     * для того чтобы инециализировать модель, связанную с ним. Модель инециализирутся только один
     * раз и в дальнейшем она будет использоваться для любых объектов того же класса, поэтому данный
     * метод выполниться только один раз в течении работы программы.
     *
     * \param model Указатель на ORM-модель
     */
    virtual void initializeORMModel(IQORMModel *model) const = 0;

    bool load(const qint64 objectId, IQORMAbstractDataSource *dataSource = 0);

    bool loadAllManyToManyReferences(IQORMAbstractDataSource *dataSource = 0);

    bool reload(IQORMAbstractDataSource *dataSource = 0);

    bool save(IQORMAbstractDataSource *dataSource = 0);

    bool remove(IQORMAbstractDataSource *dataSource = 0);

    IQORMModel * ormModel() const;

    inline IQORMError *lastError() const {return _lastError;}

    IQORMAbstractDataSource* lastDataSource() const;

public:
    inline qint64 objectId() const {return _objectId;}
    void setObjectId(const qint64 objectId);

    inline bool autoLoadReference() const {return _autoLoadReference;}
    void setAutoLoadReference(const bool autoLoadReference);

protected:
    virtual void emitObjectIdChanged(){;}
    virtual void emitAutoLoadReferenceChanged(){;}

private:
    qint64 _objectId;
    mutable QPointer<IQORMAbstractDataSource> _lastDataSource;
    IQORMError *_lastError;
    bool _autoLoadReference;

    bool setLastDataSource(IQORMAbstractDataSource* dataSource);

    friend class IQORMModel;
};

#endif // IQORMOBJECT_H
