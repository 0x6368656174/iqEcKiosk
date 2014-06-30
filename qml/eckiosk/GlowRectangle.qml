import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: rootItem
    property var source
    property rect sourceRect
    property color borderColor
    property real borderWidth
    property real radius

    ShaderEffectSource {
        visible: false
        id: se
        anchors.fill: parent
        sourceItem: source
        sourceRect: parent.sourceRect
    }

    GaussianBlur {
        id: blur
        anchors.fill: parent
        source: se
        radius: 16
        samples: 16
        visible: false
    }

    Rectangle {
        id: maskRect
        anchors.fill: parent
        radius: rootItem.radius
    }

    OpacityMask {
        id: om
        anchors.fill: parent
        source: blur
        maskSource: maskRect
    }

    LinearGradient {
        anchors.fill: parent
        opacity: 0.8
        start: Qt.point(0, 0)
        end: Qt.point(width, height)
        source: om
        gradient: Gradient {
            GradientStop {
               position: 0.0
               color: "black"
            }
            GradientStop {
                position: 0.5
                color: "Transparent"
            }
            GradientStop {
                position: 1.0
                color: "black"
            }
        }
    }

    Rectangle {
        id: borderRect
        anchors.fill: parent
        color: "Transparent"
        radius: rootItem.radius
        border.width: borderWidth
        border.color: borderColor
    }
}
