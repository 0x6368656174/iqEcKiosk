import QtQuick 2.0
import ru.itquasar.Qt.ECKiosk.CardInfo 1.0
import ru.itquasar.Qt.ECKiosk.QML 1.0
import Qt.labs.settings 1.0

Page {
    id: rootItem

    signal backPressed()

    function load(loyaltyCardId) {
        card.load(loyaltyCardId)
        cardDetail.loyaltyCardId = loyaltyCardId
        var result = cardDetail.load()
        if (result)
            sortModel.sort()
        return result
    }

    Timer {
        id: cardDetailBackTimer
        interval: 20000
        running: rootItem.visible
        onTriggered: {
            rootItem.hideRight()
            firstPage.showLeft()
        }
    }

    Item {
        id: cardDetailBackground
        anchors.fill: parent
        Image {
            id: cardDetailBackgroundImage
            anchors.fill: parent
            anchors.margins: -10
            source: "images/background.png"
        }
    }

    GlowRectangle {
        anchors.fill: parent
        source: cardDetailBackground
        sourceRect: Qt.rect(0, 0, width, height)

        Rectangle {
            id: cardDetailHeader
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: rootItem.height / 12
            anchors.margins: -border.width
            color: "Transparent"
            border.width: 1
            border.color: "white"

            LoyaltyCard {
                id: card
            }

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: font.pixelSize
                color: "white"
                text: card.customerShortName
                font.family: "Courier"
                font.pixelSize: parent.height / 2
            }

            Text {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: font.pixelSize
                color: "white"
                text: "Баланс: "+ card.balance + " р. " + card.pointsBalance + " бал."
                font.family: "Courier"
                font.pixelSize: parent.height / 2
            }
        }

        Item {
            id: detailTable
            anchors.top: cardDetailHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: cardDetailBackButton.top
            anchors.bottomMargin: fontSize
            property real fontSize: width/50

            Row {
                id: cardDetailTitle
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: parent.height / 12

                Text {
                    width: detailTable.fontSize*3
                    text: ""
                    font.family: "Courier"
                    font.pixelSize: detailTable.fontSize
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }

                Text {
                    width: parent.width * 0.25
                    text: "Дата"
                    font.family: "Courier"
                    font.pixelSize: detailTable.fontSize
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }
                Text {
                    width: parent.width * 0.25
                    text: "Операция"
                    font.family: "Courier"
                    font.pixelSize: detailTable.fontSize
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }
                Text {
                    width: parent.width * 0.25
                    text: "Автомат"
                    font.family: "Courier"
                    font.pixelSize: detailTable.fontSize
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }
                Text {
                    width: parent.width * 0.1
                    text: "Сумма"
                    font.family: "Courier"
                    font.pixelSize: detailTable.fontSize
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }
                Text {
                    width: parent.width * 0.1
                    text: "Баллы"
                    font.family: "Courier"
                    font.pixelSize: detailTable.fontSize
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }
            }

            Rectangle {
                id: lineRect
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: cardDetailTitle.bottom
                color: "white"
                height: 1
            }

            Component {
                id: listViewDelegate
                Row {
                    id: row
                    anchors.left: parent.left
                    anchors.right: parent.right
                    width: 20
                    Text {
                        width: detailTable.fontSize*3
                        text: index
                        font.family: "Courier"
                        font.pixelSize: detailTable.fontSize
                        color: "white"
                    }
                    Text {
                        width: parent.width * 0.25
                        text: {
                            var date =  operationDate
                            var result = ""
                            if (date.getDate() < 10) {
                                result = result + "0" + date.getDate()
                            } else {
                                result = result + date.getDate()
                            }
                            result = result + "."
                            if (date.getMonth() < 9) {
                                result = result + "0" + (date.getMonth() + 1)
                            } else {
                                result = result + (date.getMonth() + 1)
                            }
                            result = result + "." + date.getFullYear() + " "
                            if (date.getHours() < 10) {
                                result = result + "0" + (date.getHours())
                            } else {
                                result = result + (date.getHours())
                            }
                            result = result + ":"
                            if (date.getMinutes() < 10) {
                                result = result + "0" + (date.getMinutes())
                            } else {
                                result = result + (date.getMinutes())
                            }
                            return result
                        }
                        font.family: "Courier"
                        font.pixelSize: detailTable.fontSize
                        color: "white"
                    }
                    Text {
                        width: parent.width * 0.25
                        text: operationName
                        font.family: "Courier"
                        font.pixelSize: detailTable.fontSize
                        color: "white"
                    }
                    Text {
                        width: parent.width * 0.25
                        text: gameName
                        font.family: "Courier"
                        font.pixelSize: detailTable.fontSize
                        color: "white"
                    }
                    Text {
                        width: parent.width * 0.1
                        text: amount
                        font.family: "Courier"
                        font.pixelSize: detailTable.fontSize
                        color: "white"
                    }
                    Text {
                        width: parent.width * 0.1
                        text: points
                        font.family: "Courier"
                        font.pixelSize: detailTable.fontSize
                        color: "white"
                    }
                }
            }

            VCardOperations {
                id: cardDetail
                property int loyaltyCardId
                filters: Filter {
                    property: "loyaltyCardId"
                    operation: Filter.Equals
                    value: cardDetail.loyaltyCardId
                }
            }

            Settings {
                id: settings
                property bool sortAscending: true
            }

            SortModel {
                id: sortModel
                sourceModel: cardDetail
                sortRoleName: "objectId"
                sortOrder: settings.sortAscending?Qt.AscendingOrder:Qt.DescendingOrder
            }

            ListView {
                id: lv
                anchors.top: lineRect.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                clip: true
                model: sortModel
                delegate: listViewDelegate
                onContentYChanged: cardDetailBackTimer.restart()

                Binding {
                    target: handle
                    property: "y"
                    value: (lv.contentY * clicker.drag.maximumY / (lv.contentHeight - lv.height))
                    when: (!clicker.drag.active && !clicker.pressed)
                }
                Binding {
                    target: lv
                    property: "contentY"
                    value: (handle.y * (lv.contentHeight - lv.height) / clicker.drag.maximumY)
                    when: (clicker.drag.active || clicker.pressed)
                }

                Rectangle {
                    id: handle
                    visible: lv.contentHeight > lv.height
                    anchors.right: parent.right
                    width: lv.width / 150
                    height: lv.visibleArea.heightRatio * lv.height > lv.height * 0.1 ? lv.visibleArea.heightRatio * lv.height : lv.height * 0.1
                    color: "white"
                    opacity: clicker.drag.active || clicker.pressed ? 1.0:0.5

                    MouseArea {
                        id: clicker
                        preventStealing: true
                        drag {
                            target: handle
                            minimumY: 0
                            maximumY: (lv.height - handle.height)
                            axis: Drag.YAxis
                        }

                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.topMargin: -parent.width * 3
                        anchors.bottomMargin: -parent.width * 3
                        width: parent.width * 4
                    }
                }
            }
        }

        Button {
            id: cardDetailBackButton
            source: cardDetailBackground
            sourceRect: Qt.rect(x, y, width, height)
            width: height * 3
            height: rootItem.height / 12

            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: -border.width

            text: "Назад"

            onClicked: backPressed()
        }
    }
}
