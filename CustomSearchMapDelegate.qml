import QtQuick 2.12
import QtLocation 5.12
import QtPositioning 5.12

MapQuickItem {
    signal itemClicked(string detail)
    coordinate: model.result.coordinate
    sourceItem: Image {
        id: image
        source: "resources/marker.png"
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            bottomPadding: 7
            font.pointSize: 8
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            //text: model.number
            text: model.result.name.slice(0,1)
            color: "#DF5656"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: itemClicked(model.result.detail)
            //onClicked: stackView.showPlaceDatails(model.place,model.distance)
        }

    }
    anchorPoint.x: image.width * 0.5
    anchorPoint.y: image.height
}
