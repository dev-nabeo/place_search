import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ToolBar {
    id: sbar

    property bool nbuttonVisbile: false

    signal doSearch(string searchtext)
    signal doNearby()
    signal setService(string servicename)

    onNbuttonVisbileChanged: {
        nbutton.enabled = nbuttonVisbile
    }

    RowLayout {
        anchors.fill: parent
        Text {
            id: servcename
            leftPadding: 5
        }
        ToolButton {
            text: qsTr("⋮")
            onClicked: servicemenu.open()
        }
        ToolSeparator {}
        ToolButton {
            id: nbutton
            enabled: nbuttonVisbile
            text: qsTr("近隣検索")
            onClicked: doNearby()
        }
        ToolSeparator {}
        TextField {
            id: sfield
            Layout.fillWidth: true
            placeholderText: qsTr("地名を入力してください...")
            onAccepted: doSearch(sfield.text)
        }
        ToolButton {
            id: sbutton
            text: qsTr("地名検索")
            onClicked: doSearch(sfield.text)
        }
    }
    ButtonGroup {
        id: servicegroup
        onClicked: {
            console.log(button.text);
            servicemenu.close();
            sbar.decideServiceName();
        }
    }
    Menu{
        id: servicemenu
        RadioButton {
            id: service_osm
            text: qsTr("OSM(OpenStreetMap)")
            checked: true
            ButtonGroup.group: servicegroup
        }
        RadioButton {
            id: service_gsi
            text: qsTr("GSI(国土地理院)")
            ButtonGroup.group: servicegroup
        }
        RadioButton {
            id: service_ymr
            text: qsTr("YMR(ヤマレコ)")
            ButtonGroup.group: servicegroup
        }
    }
    Component.onCompleted: {
        sbar.decideServiceName();
    }
    function decideServiceName(){
        if(service_osm.checked){
            servcename.text="OSM"
        }
        else if(service_gsi.checked){
            servcename.text="GSI"
        }
        else{
            servcename.text="YMR"
        }
        setService(servcename.text)
    }
}
