import QtQuick 2.3
import Linphone 1.0

Rectangle {
    width: 640
    height: 480
    color: "red"

   // Text {
   //     anchors.centerIn: parent
  //      text: "Hello, World!"
   // }
	  CameraPreview {
    anchors.fill: parent
  }
}