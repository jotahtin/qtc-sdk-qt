import QtQuick 2.2
import QtCloudServices 1.0

Item {
    id: container
    width: 480
    height: 800

    function foobar() {
        console.log("Z..")
    }

    Rectangle {
        id: root
        color: "#f4f4f4"
        anchors.fill: parent

        Rectangle {
            id: header
            anchors.top: parent.top
            width: parent.width
            height: 70
            color: "white"

            MouseArea {
                anchors.fill: parent
                onClicked: foobar()
            }
        }


    }

}
