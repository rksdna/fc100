import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true

    Rectangle {
           id: root
           anchors.fill: parent
           color: "gray"
           Text {
               id: label
               anchors.centerIn: parent
               text: device.objectName
           }
       }
}

