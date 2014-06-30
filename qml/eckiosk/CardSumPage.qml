import QtQuick 2.0
import ru.itquasar.Qt.ECKiosk.CardInfo 1.0

Page {
    id: rootItem

    signal backPressed()
    signal cardDetailPressed(var loyaltyCardId)

    function findCard(cardSecret) {
        loyaltyCardsFounder.cardSecret = cardSecret
        if (loyaltyCardsFounder.load()){
            if (loyaltyCardsFounder.count === 1) {
                return true
            }
        }
        console.log("Error: " + loyaltyCardsFounder.lastError.text)

        return false
    }

    Timer {
        interval: 20000
        running: rootItem.visible
        onTriggered: {
            rootItem.hideRight()
            firstPage.showLeft()
        }
    }

    Item {
        id: background
        anchors.fill: parent
        Image {
            id: backgroundImage
            anchors.fill: parent
            anchors.margins: -10
            source: "images/background.png"
        }
    }

    LoyaltyCards {
        id: loyaltyCardsFounder
        property string cardSecret
        filters: Filter {
            property: "cardSecret"
            operation: Filter.Equals
            value: loyaltyCardsFounder.cardSecret
        }

    }

    GlowRectangle {
        anchors.top: parent.top
        anchors.bottom: backButton.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: parent.height / 15
        width: height*1.5+2*anchors.margins < parent.width? height*1.5 : parent.width - 2*anchors.margins

        source: background
        sourceRect: Qt.rect(x, y, width, height)
        borderColor: "white"
        borderWidth: 1
        //            radius: height / 10

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: parent.height / 15
            source: "images/logo.png"
            fillMode: Image.PreserveAspectFit
            width: parent.width * 0.6
        }

        Text {
            id: balanceText
            text: {
                if (loyaltyCardsFounder.count === 1)
                    return loyaltyCardsFounder.get(0).customerShortName + "\nБаланс: "
                            + loyaltyCardsFounder.get(0).balance + " р.\n        "
                            + loyaltyCardsFounder.get(0).pointsBalance + " бал."
                return ""
            }
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: height / 4
            font.family: "Courier"
            font.pixelSize: parent.height /8
        }
    }

    Button {
        id: backButton
        source: background
        sourceRect: Qt.rect(x, y, width, height)
        width: height * 3
        height: rootItem.height / 12

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: -border.width

        text: "Назад"

        onClicked: backPressed()
    }

    Button {
        source: background
        sourceRect: Qt.rect(x, y, width, height)
        width: height * 3
        height: rootItem.height / 12

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: -border.width

        text: "Подробно"
        onClicked: {
            if (loyaltyCardsFounder.count === 1)
                cardDetailPressed(loyaltyCardsFounder.get(0).objectId)
        }
    }
}
