import QtQuick 2.2
import QtCloudServices 1.0

Item {
    id: container
    width: 480
    height: 800

    function handleResult(op) {
        if (op.isValid) {
            console.log("Operation Success")
            //console.log("DATA: " + op.resultJson["id"])
            //console.log("DATA: " + op.resultJson)
            console.log("DATA: " + op.resultBytes)
        } else {
            console.log("Operation Error: " + op.errorString)
        }
    }

    function example_find() {
        var todos = eds.collection("todos")
        todos.find({
                    "q": {
                           "name": "John"
                         },
                    "limit":   5,
                    "offset":  0
                   },
                   handleResult)
    }

    function example_findOne() {
        var todos = eds.collection("todos")
        todos.findOne("538cebdce5bde532110096c6",
                      handleResult)
    }

    function example_insert() {
        var todos = eds.collection("todos")
        todos.insert({
                    "name": "John",
                    "age": 31,
                    "title": "plah",
                    "completed": true
                    },handleResult)
    }

    function example_update() {
        var todos = eds.collection("todos")
        todos.update("538cf0be5a3d8b1a1900db53",
                    {
                        "title": "Proper title",
                    },handleResult)
    }

    function example_remove() {
        var todos = eds.collection("todos")
        todos.remove("538cf0be5a3d8b1a1900db53",
                     handleResult)
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

            Text {
                text: "Push Me"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: example_remove()
            }

            data: [
                QEnginioQuery {
                  id: myQuery
                  offset: 0
                },

                QEnginioDataStorage {
                    id: eds
                    backendId: "5379dea0698b3c1dc00cdf57"
                    endpointAddress: "https://api.engin.io"
                }
            ]
        }


    }

}
