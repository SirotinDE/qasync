import QtQuick
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Column {
        anchors.centerIn: parent
        width: parent.width / 2

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width / 2
            text: "getExample"
            onClicked: {
                cppApi.getExample()
            }
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width / 2
            text: "getExampleVoid"
            onClicked: {
                cppApi.getExampleVoid()
            }
        }
    }


}
