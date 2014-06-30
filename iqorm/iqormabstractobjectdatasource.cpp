#include "iqormabstractobjectdatasource.h"
#include "iqormabstractdatasource.h"
#include "iqormpropertydescription.h"
#include "iqormmodel.h"

IQORMAbstractObjectDataSource::IQORMAbstractObjectDataSource(IQORMAbstractDataSource *parent) :
    QObject(parent)
{
}

IQORMAbstractObjectDataSource::~IQORMAbstractObjectDataSource()
{
}

bool IQORMAbstractObjectDataSource::loadObjectAllManyToManyReferences(IQORMObject *object) const
{
    object->resetReference();

    IQORMModel *ormModel = object->ormModel();
    if (!ormModel)
    {
        object->lastError()->addError(tr("Not found valid object ORM model."));
        return false;
    }

    foreach (IQORMPropertyDescription *propDescription, ormModel->propertyDescriptions())
    {
        if (!propDescription || propDescription->propertyType() != IQORMPropertyDescription::ManyToManyReference)
            continue;

        if(loadObjectManyToManyReference(object, propDescription->propertyName()))
        {
            object->resetReference();
            return false;
        }
    }

    return true;
}
