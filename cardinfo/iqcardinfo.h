#ifndef IQCARDINFO_H
#define IQCARDINFO_H
#include "iqcardinfo.h"
#include "iqormobjectsmodel.h"
#include "iqormfilter.h"
#include "iqgameobject.h"
#include "iqloyaltycards.h"
#include "iqcardaccountoperationtype.h"
#include "iqloyaltycard.h"
#include "iqcardaccountoperation.h"
#include "iqcardaccountoperations.h"
#include "iqvcardoperation.h"
#include "iqvcardoperations.h"
#include <QtQml>


namespace IQCardInfo
{
    void registerQmlTypes()
    {
        qmlRegisterType<IQORMError>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "ORMError");
        qmlRegisterType<IQORMAbstractFilter>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "FilterBase");
        qmlRegisterType<IQORMFilter>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "Filter");

        qmlRegisterType<IQLoyaltyCard>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "LoyaltyCard");
        qmlRegisterType<IQCardAccountOperation>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "CardAccountOperation");
        qmlRegisterType<IQGameObject>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "GameObject");
        qmlRegisterType<IQCardAccountOperationType>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "CardAccountOperationType");
        qmlRegisterType<IQVCardOperation>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "VCardOperation");

        qmlRegisterType<IQCardAccountOperations>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "CardAccountOperations");
        qmlRegisterType<IQLoyaltyCards>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "LoyaltyCards");
        qmlRegisterType<IQVCardOperations>("ru.itquasar.Qt.ECKiosk.CardInfo", 1, 0, "VCardOperations");
    }
}

#endif // IQCARDINFO_H
