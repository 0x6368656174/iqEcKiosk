import QtQuick 2.0
import ru.itquasar.Qt.ECKiosk.MitecController 1.0
import ru.itquasar.Qt.ECKiosk.CardInfo 1.0

Rectangle {
    width: 800
    height: 600
    color: "black"

    function showPage(page) {
        //Найдем текущую показываемую страницу
        var currentPage
        for (var i = 0; i < pages.children.length; i++) {
            if (pages.children[i].visible) {
                currentPage = pages.children[i]
                break
            }
        }
        if (currentPage === page)
            return
        //Сравним порядки страниц
        if (currentPage.order > page.order) {
            currentPage.hideRight()
            page.showLeft()
        } else {
            currentPage.hideLeft()
            page.showRight()
        }
    }

    Component.onCompleted: {
        //Для того, чтоб установить кодировку в правильную, выполним простой запрос
        codSetObject.load(1)
    }
    LoyaltyCard {
        id: codSetObject
    }

    //Для тестирования
    //При раскоментировании, через 1 секунду эмитирует активацию служебной карты
    Timer {
        interval: 1000
        running: true
        onTriggered: {
            if (cardSumPage.findCard("680=206450001=100001")) {
                //Покажем вторую стрницу
                showPage(cardSumPage)
            }
        }
    }

    CardController {
        id: mitecController

        onCardFound: {
            if (cardSumPage.findCard(cardSecret)) {
                //Покажем вторую стрницу
                showPage(cardSumPage)
            }
        }

        Component.onCompleted: connectToMitec()
    }

    Item {
        anchors.fill: parent
        id: pages
        StartPage {
            id: firstPage
            order: 0
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.width
        }

        CardSumPage {
            id: cardSumPage
            order: 1
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.width
            visible: false

            onBackPressed: showPage(firstPage)

            onCardDetailPressed: {
                if(cardDetailPage.load(loyaltyCardId))
                    showPage(cardDetailPage)
            }
        }

        CardDetailPage {
            id: cardDetailPage
            order: 2
            visible: false
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.width

            onBackPressed: showPage(firstPage)
        }
    }
}
