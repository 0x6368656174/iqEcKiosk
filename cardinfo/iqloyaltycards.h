#ifndef IQLOYALTYCARDS_H
#define IQLOYALTYCARDS_H

#include "iqloyaltycard.h"
#include "iqormobjectsmodel.h"

class IQLoyaltyCards : public IQORMObjectsModel<IQLoyaltyCard>
{
    Q_OBJECT
public:
    explicit IQLoyaltyCards(QObject *parent = 0);
};

#endif // IQLOYALTYCARDS_H
