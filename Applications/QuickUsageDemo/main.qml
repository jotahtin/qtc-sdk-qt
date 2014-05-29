import QtQuick 2.2

Item {
    width: 400
    height: 640

    Rectangle {
        id: root
        color: "#f4f4f4"
        anchors.fill: parent

        Rectangle {
            id: rectangle1
            x: 8
            y: 8
            width: 384
            height: 624
            color: "#ffffff"

            MouseArea {
                id: mouseArea1
                x: 67
                y: 417
                width: 250
                height: 151
            }

            TextEdit {
                id: textEdit1
                x: 67
                y: 50
                width: 250
                height: 75
                text: qsTr("Text Edit")
                font.pixelSize: 12
            }

            ListView {
                id: listView1
                x: 67
                y: 171
                width: 250
                height: 188
                delegate: Item {
                    x: 5
                    width: 80
                    height: 40
                    Row {
                        id: row1
                        Rectangle {
                            width: 40
                            height: 40
                            color: colorCode
                        }

                        Text {
                            text: name
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                        }
                        spacing: 10
                    }
                }
                model: ListModel {
                    ListElement {
                        name: "Grey"
                        colorCode: "grey"
                    }

                    ListElement {
                        name: "Red"
                        colorCode: "red"
                    }

                    ListElement {
                        name: "Blue"
                        colorCode: "blue"
                    }

                    ListElement {
                        name: "Green"
                        colorCode: "green"
                    }
                }
            }
        }

    }
}
