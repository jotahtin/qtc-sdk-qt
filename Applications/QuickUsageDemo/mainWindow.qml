import QtQuick 2.0
import QtQuick.Controls 1.2

ApplicationWindow {
    visible: true
    title: "Main Window"

    TextArea {
        id: textArea1
        x: 81
        y: 69
        width: 240
        height: 159
    }

    Button {
        id: button1
        x: 145
        y: 347
        text: qsTr("Button")
    }
}
