# QtCloudServices SDK (Qt Version)

This is QtCloudServices SDK for Qt. What is QtCloudServices? See below:

* The Qt Cloud Services home page is at https://www.qtc.io
* The Developer Documentation page is at https://developer.qtc.io

# System Requirements
* Qt 5.3 or newer
* OpenSSL library
  * Mac OS X: OpenSSL library should be preinstalled.
  * Linux: Most distributions have preinstalled OpenSSL library. If yours doesn't, seach for `libssl` in package repository.
  * Windows: Get the installer from http://slproweb.com/products/Win32OpenSSL.html (light version is enough, copy DLLs to windows system directory when asked).
* Perl 5.10 or newer
  * Mac and Linux: Perl should be preinstalled.
  * Windows: http://www.perl.org/get.html

# Build & Install
    * `qmake && make install`
    * Produces shared library and installs it as a globally available Qt5 module

# Usage
* In C++ applications 
    * Use QtCloudServices module by adding `QT += cloudservices` to application `.pro` file
    * Include QtCloudServices headers with `<QtCloudServices/...>` (for example: `#include <QtCloudServices/QEnginioDataStorage>`) 
* In QML applications 
    * Import QtCloudServices components with `import QtCloudServices 1.0`
  
## Installing

## Quick Start

This SDK provides connectivity support to QtCloudServices for both Qt/C++ and Qt/QML.

* Enginio Data Storage
   * Primitive database operations (find, findOne, insert, update, remove)
   * Model based views (QAbstractItemModel) - Flat list / Hierarchial views.
   * Object Manager - Automatically track object properties.
* Managed Web Sockets. This can be used for example to augment Enginio Data Storage Model .

### Enginio Data Storage

#### Qt/C++

Include QEnginioDataStorage

```cpp
#include <QtCloudServices/QEnginioDataStorage>

QEnginioDataStorage eds("https://api.engin.io", "123456789abcdef123456789")

// optional authorization
//  eds.setSecret(<security token>)
// or
//  eds.setUsername(<username>)
//  eds.setPassword(<password>)
```

##### QEnginioDataStorage.collection
```cpp
    eds.collection(name)
    
    // for example
    QEnginioCollection todos;
    todos = eds.collection("todos");
```

Assign collection instance. Arguments:

* name - A name of the collection

##### QEnginioDataStorage.collection.find

```cpp
    collection.find(query, resultHandler)

    // for example
    
    // result handle can be lambda/function/functor/etc..
    std::function<void(QEnginioOperation)> resultHandle 

    QEnginioCollection todos;
    todos = eds.collection("todos");

    todos.find(QEnginioQuery([query filter json/aux constructors])
               .limit(5)
               .offset(0)
               .sort(...)
               ,resultHandle);
```
Query for objects in a collection. Arguments:

* query : Query definition
* resultHandler - Callback for handling results/errors

##### QEnginioDataStorage.collection.findOne

```cpp
    collection.find(objectId, resultHandler)

    // for example
    QEnginioCollection todos;
    todos = eds.collection("todos");
    todos.findOne("538cebdce5bde532110096c6",
                  handleResult);
```
Find an object in collection. Arguments:

* objectId - Object Identifier
* resultHandler - Callback for handling results/errors

##### QEnginioDataStorage.collection.insert

```cpp
    collection.insert(object, resultHandler)

    // for example
    QEnginioCollection todos;
    todos = eds.collection("todos");

    QJsonObject object;
    object["name"] = "John Doe"
    object["age"] = 31;
    object["completed"] = true;
    todos.insert(object,
                 handleResult);
```
Insert an object into a collection. Arguments:

* object - Object
* resultHandler - Callback for handling results/errors

##### QEnginioDataStorage.collection.update

```cpp
    collection.update(objectId, object, resultHandler)

    // for example
    QEnginioCollection todos;
    todos = eds.collection("todos");

    QJsonObject object;
    object["title"] = "Proper title"
    todos.update("538cf0be5a3d8b1a1900db53", object,
                 handleResult)
```
Update an existing object. Arguments:

* objectId - ObjectId for object to be updated
* object - Object with field that are to be reassigned.
* resultHandler - Callback for handling results/errors

##### QEnginioDataStorage.collection.remove

```cpp
    collection.remove(objectId, resultHandler)
    
    // for example
    QEnginioCollection todos;
    todos = eds.collection("todos");
    todos.remove("538cf0be5a3d8b1a1900db53",
                 handleResult);
```
Remove an object from a collection. Arguments:

* objectId - ObjectId for object to be removed
* resultHandler - Callback for handling results/errors


#### Qt/QML

Import QtCloudServices module.

```qml
import QtQuick 2.2
import QtCloudServices 1.0
```

Create QnginioDataStorage instance in some suitable data area in QML, take note to id 'eds' this is used
later to reference this data storage instance. 

```qml
Item {
   ...
   Rectangle {
       data: [
           QEnginioDataStorage {
               id: eds
               backendId: "123456789abcdef123456789"     // backend id
               endpointAddress: "https://api.engin.io"   // instance addres

               // optional authorization
               //  secret: <security token>
               // or
               //  username: <username>
               //  password: <password>

           }
       ]
   }
   ...
```

##### QEnginioDataStorage.collection
```qml
        var collection = eds.collection(name)
        
        // for example
        var todos = eds.collection("todos")
```

Assign collection instance. Arguments:

* name - A name of the collection

##### QEnginioDataStorage.collection.find

```qml
    collection.find(query, resultHandler)

    // for example
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

    var todos = eds.collection("todos")
    todos.find({
                "q": {
                       "name": "John"
                     },
                "limit":   5,
                "offset":  0
               },
               handleResult)
```
Query for objects in a collection. Arguments:

* query : Query definition
  * q      - Query definition see [Enginio documentation](https://developer.qtcloudservices.com/eds/key-concepts/files)
  * limit  - Result set limit
  * offset - Result set offset
  * ...
* resultHandler - Callback for handling results/errors

##### QEnginioDataStorage.collection.findOne

```qml
    collection.find(objectId, resultHandler)

    // for example
    var todos = eds.collection("todos")
    todos.findOne("538cebdce5bde532110096c6",
                  handleResult)
```
Find an object in collection. Arguments:

* objectId - Object Identifier
* resultHandler - Callback for handling results/errors

##### QEnginioDataStorage.collection.insert

```qml
    collection.insert(object, resultHandler)

    // for example
    var todos = eds.collection("todos")
    todos.insert({
                "name": "John Doe",
                "age": 31,
                "completed": true
                },handleResult)
```
Insert an object into a collection. Arguments:

* object - Object
* resultHandler - Callback for handling results/errors

##### QEnginioDataStorage.collection.update

```qml
    collection.update(objectId, object, resultHandler)

    // for example
    var todos = eds.collection("todos")
    todos.update("538cf0be5a3d8b1a1900db53",
                {
                    "title": "Proper title",  
                },handleResult)
```
Update an existing object. Arguments:

* objectId - ObjectId for object to be updated
* object - Object with field that are to be reassigned.
* resultHandler - Callback for handling results/errors

##### QEnginioDataStorage.collection.remove

```qml
    collection.remove(objectId, resultHandler)
    
    // for example
    var todos = eds.collection("todos")
    todos.remove("538cf0be5a3d8b1a1900db53",
                handleResult)
```
Remove an object from a collection. Arguments:

* objectId - ObjectId for object to be removed
* resultHandler - Callback for handling results/errors

### Managed Web Sockets

TODO

## SDK API References

See the product specific SDK API References:

* [Enginio Data Storage](https://github.com/jotahtin/qtc-sdk-qt/wiki/Enginio-Data-Storage-SDK-API)
* [Managed WebSocket](https://github.com/jotahtin/qtc-sdk-qt/wiki/Managed-WebSocket-SDK-API)
