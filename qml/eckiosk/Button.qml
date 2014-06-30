import QtQuick 2.0

GlowRectangle {
    id: rootItem
    property alias text: textText.text
    signal clicked()
    scale: ma.pressed?1.2:1

    borderColor: "white"
    borderWidth: 1
    Text {
        anchors.centerIn: parent
        id: textText
        color: borderColor
        font.pixelSize: parent.width / 6
        font.family: "Courier"
    }

    MouseArea {
        id: ma
        anchors.fill: parent
        onClicked: rootItem.clicked()
    }
}
