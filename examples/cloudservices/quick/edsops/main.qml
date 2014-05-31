import QtQuick 2.2
import QtCloudServices 1.0

Item {
    id: container
    width: 480
    height: 800

    function foobar() {
        console.log("Z..")


        var x = eds.collection("todos")

        //console.log("Z.." + x.length)
    }

    Rectangle {
        id: root
        color: "#f4f4f4"
        anchors.fill: parent

        Rectangle {
            id: headere
            anchors.top: parent.top
            width: parent.width
            height: 70
            color: "white"

            MouseArea {
                anchors.fill: parent
                onClicked: foobar()
            }

            data: [
                QEnginioDataStorage {
                    id: eds
                    backendId: "5379dea0698b3c1dc00cdf57"
                    instanceAddress: "https://api.engin.io"
                }

            ]
        }


    }

}
