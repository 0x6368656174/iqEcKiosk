import QtQuick 2.0
import ru.itquasar.Qt.ECKiosk.CardInfo 1.0

Page {
    id: rootItem

    Image {
        id: startPageBackground
        anchors.fill: parent
        source: "images/background.png"
    }

    GlowRectangle {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: parent.height / 15
        width: height*1.5+2*anchors.margins < parent.width? height*1.5 : parent.width - 2*anchors.margins

        source: startPageBackground
        sourceRect: Qt.rect(x, parent.y + anchors.margins, width, height)
        borderColor: "white"
        borderWidth: 1
        //            radius: height / 10


        Image {
            id: startPageLogo
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: parent.height / 8
            source: "images/logo.png"
            fillMode: Image.PreserveAspectFit
            width: parent.width * 0.6
        }

        Item{
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: startPageLogo.bottom
            anchors.bottom: parent.bottom


            Text {
                anchors.fill: parent
                anchors.margins: parent.height / 6
                font.family: "Courier"
                font.pixelSize: height / 5
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                color: "white"
                text: "Поднесите игровую карту к считывателю"
            }
        }
    }
}
