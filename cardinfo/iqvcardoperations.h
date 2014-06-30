#ifndef IQVCARDOPERATIONS_H
#define IQVCARDOPERATIONS_H

#include "iqormobjectsmodel.h"
#include "iqvcardoperation.h"

class IQVCardOperations : public IQORMObjectsModel<IQVCardOperation>
{
    Q_OBJECT
public:
    explicit IQVCardOperations(QObject *parent = 0);
};

#endif // IQVCARDOPERATIONS_H
