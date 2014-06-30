#include "iqormobject.h"
#include "iqormcore.h"

#include "iqormabstractdatasource.h"

IQORMObject::IQORMObject():
    _lastError(new IQORMError()),
    _autoLoadReference(true)
{
    setObjectId(-1);

    IQORMCore::synchronizer()->registerObject(this);
}

IQORMObject::~IQORMObject()
{
    IQORMCore::synchronizer()->unregisterObject(this);
    _lastError->deleteLater();
}

IQORMModel * IQORMObject::ormModel() const
{
    return IQORMCore::model(this);
}

IQORMAbstractDataSource * IQORMObject::lastDataSource() const
{
    return _lastDataSource;
}

bool IQORMObject::load(const qint64 id, IQORMAbstractDataSource *dataSource)
{
    lastError()->clearErrors();

    if (!setLastDataSource(dataSource))
    {
        lastError()->addError(QObject::tr("Can not set Data Source for object."));
        return false;
    }

    bool result = _lastDataSource->objectDataSource()->loadObjectById(this, id);

    if (result)
    {
        setObjectId(id);
    }
    else
    {
        setObjectId(-1);
    }

    if (autoLoadReference())
    {
        if (!loadAllManyToManyReferences(dataSource))
            return false;
    }

    return result;
}

bool IQORMObject::loadAllManyToManyReferences(IQORMAbstractDataSource *dataSource)
{
    if (!setLastDataSource(dataSource))
    {
        lastError()->addError(QObject::tr("Can not set Data Source for object."));
        return false;
    }

    bool result = _lastDataSource->objectDataSource()->loadObjectAllManyToManyReferences(this);

    return result;
}

bool IQORMObject::reload(IQORMAbstractDataSource *dataSource)
{
    lastError()->clearErrors();

    if (objectId() < 0)
    {
        lastError()->addError(QObject::tr("Can not reload object without objectId. Use load() first."));
        return false;
    }

    return load(objectId(), dataSource);
}

bool IQORMObject::setLastDataSource(IQORMAbstractDataSource *dataSource)
{
    if (dataSource)
    {
        if (_lastDataSource.data() == dataSource)
            return true;

        _lastDataSource = dataSource;
    }

    if (!_lastDataSource)
        _lastDataSource = IQORMCore::dataSource();

    if (!_lastDataSource)
    {
        lastError()->addError(QObject::tr("Not found any Data Source. Use IQORMCore::setDataSource() first."));
        return false;
    }

    if (!_lastDataSource->objectDataSource())
    {
        lastError()->addError(QObject::tr("Not found any Object Data Source. Maybe Data Source broken, check Data Source."));
        return false;
    }

    return true;
}

bool IQORMObject::save(IQORMAbstractDataSource *dataSource)
{
    lastError()->clearErrors();

    if (!setLastDataSource(dataSource))
    {
        lastError()->addError(QObject::tr("Can not set Data Source for object."));
        return false;
    }

    bool result = false;

    if (objectId() < 0)
    {
        qint64 newId = -1;

        result = _lastDataSource->objectDataSource()->insertObject(this, &newId);

        if (result)
        {
            setObjectId(newId);
            IQORMCore::synchronizer()->updateLinkedObjectModels(this);
        }
    }
    else
    {
        result = _lastDataSource->objectDataSource()->updateObject(this);

        if (result)
            IQORMCore::synchronizer()->updateLinkedObjects(this);
    }



    return result;
}

void IQORMObject::setObjectId(const qint64 objectId)
{
    if (_objectId == objectId)
        return;
    _objectId = objectId;

    emitObjectIdChanged();
}

void IQORMObject::setAutoLoadReference(const bool autoLoadReference)
{
    if (_autoLoadReference == autoLoadReference)
        return;
    _autoLoadReference = autoLoadReference;

    emitAutoLoadReferenceChanged();
}

bool IQORMObject::remove(IQORMAbstractDataSource *dataSource)
{
    lastError()->clearErrors();

    if (!setLastDataSource(dataSource))
    {
        lastError()->addError(QObject::tr("Can not set Data Source for object."));
        return false;
    }

    bool result = _lastDataSource->objectDataSource()->removeObject(this);

    if (result)
    {
        setObjectId(-1);
        IQORMCore::synchronizer()->updateLinkedObjectModels(this);
    }

    return result;
}

void IQORMObject::resetReference()
{
    QObject *qobject = dynamic_cast<QObject *>(this);
    if (!qobject)
        return;

    foreach (IQORMPropertyDescription *propDescription, ormModel()->propertyDescriptions())
    {
        if (!propDescription || propDescription->propertyType() != IQORMPropertyDescription::ManyToManyReference)
            continue;

        qobject->setProperty(propDescription->propertyName().toLocal8Bit().constData(),
                             QList<QVariant>());
    }
}
