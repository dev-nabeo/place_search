import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtPositioning 5.12

Item {
    signal listClicked(var coordinate)
    height: informationarea.height*1.3
    width: parent === null ? 0 : parent.width
    Rectangle{
        anchors.fill: parent
        anchors.margins: 2
        color: "whitesmoke"
        border.color: "lightgray"
        border.width: 1
        radius: 10
        //Row{
        GridLayout {
            id: informationarea
            rows: 2
            columns: 2
            width: parent.width
            flow: GridLayout.TopToBottom
            //spacing: 2
            anchors.verticalCenter: parent.verticalCenter
            Image {
                source: "resources/marker.png"
                Layout.rowSpan: 2
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
            }
            Text {
                text: model.result.name
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: true
            }
            Text {
                text: model.result.addressText==="" ? model.result.detail : model.result.addressText
                padding: 2
                font.pointSize: 7
                wrapMode: Text.Wrap
                maximumLineCount: 2
                elide: Text.ElideRight
                verticalAlignment: Text.AlignTop
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: {
            //console.log("mouse click", model.latitude, model.longitude )
            listClicked( model.result.coordinate )
        }
    }

}
