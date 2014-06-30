import QtQuick 2.0

Item {
    id: rootItem
    property int order
    clip: true

    function hideLeft() {
        hideLeftAnimations.start()
    }

    function showLeft() {
        showLeftAnimations.start()
    }

    function showRight() {
        showRightAnimations.start()
    }

    function hideRight() {
        hideRightAnimations.start()
    }

    SequentialAnimation {
        id: hideLeftAnimations
        ScriptAction {
            script: {
                rootItem.anchors.left = undefined
                rootItem.anchors.horizontalCenter = parent.horizontalCenter
                rootItem.z = 1
            }
        }
        PropertyAnimation {
            target: rootItem
            properties: "scale"
            from: 1
            to: 0.8
            duration: 200
        }
        ScriptAction {
            script: {
                hideRect.visible = true
            }
        }

        PauseAnimation {
            duration: 400
        }
        ScriptAction {
            script: {
                rootItem.z = 0
                rootItem.visible = false
            }
        }
    }

    SequentialAnimation {
        id: showLeftAnimations
        ScriptAction {
            script: {
                rootItem.anchors.left = undefined
                rootItem.anchors.horizontalCenter = parent.horizontalCenter
                rootItem.z = 1
                rootItem.scale = 0.8
                hideRect.visible = true
                rootItem.visible = true
            }
        }
        PauseAnimation {
            duration: 400
        }
        ScriptAction {
            script: {
                hideRect.visible = false
            }
        }
        PropertyAnimation {
            target: rootItem
            properties: "scale"
            from: 0.8
            to: 1
            duration: 200
        }
        ScriptAction {
            script: {
                rootItem.z = 2
            }
        }
    }

    SequentialAnimation {
        id: showRightAnimations
        ScriptAction {
            script: {
                rootItem.anchors.horizontalCenter = undefined
                rootItem.anchors.left = parent.left
                rootItem.anchors.leftMargin = rootItem.width
                rootItem.z = 2
                scale = 1
                hideRect.visible = false
                rootItem.visible = true
            }
        }
        PauseAnimation {
            duration: 200
        }
        PropertyAnimation {
            target: rootItem
            properties: "anchors.leftMargin"
            from: rootItem.width
            to: 0
            duration: 400
        }
    }

    SequentialAnimation {
        id: hideRightAnimations
        ScriptAction {
            script: {
                rootItem.anchors.horizontalCenter = undefined
                rootItem.anchors.left = parent.left
                rootItem.anchors.leftMargin = 0
            }
        }
        PropertyAnimation {
            target: rootItem
            properties: "anchors.leftMargin"
            from: 0
            to: rootItem.width
            duration: 400
        }
        ScriptAction {
            script: {
                rootItem.visible = false
            }
        }
    }

    Rectangle {
        id: hideRect
        anchors.fill: parent
        z: 2
        color: "black"
        opacity: 0.75
        visible: false
    }
}
