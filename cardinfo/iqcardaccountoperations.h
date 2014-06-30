#ifndef IQCARDACCOUNTOPERATIONS_H
#define IQCARDACCOUNTOPERATIONS_H

#include "iqcardaccountoperation.h"
#include "iqormobjectsmodel.h"

class IQCardAccountOperations : public IQORMObjectsModel<IQCardAccountOperation>
{
    Q_OBJECT
public:
    explicit IQCardAccountOperations(QObject *parent = 0);
};

#endif // IQCARDACCOUNTOPERATIONS_H
