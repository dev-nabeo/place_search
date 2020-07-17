import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtLocation 5.12
import QtPositioning 5.12
import customSearchModel 1.0

ApplicationWindow {
    id: appWin
    visible: true
    width: 640
    height: 480
    title: qsTr("検索機能検証")

    header: SearchBar{
        id: sbar
        onDoNearby: {
            console.log("onDoNearby");
            custommodel.method = "nearby";
            custommodel.searchCoordinate=mapGis.center;
            custommodel.update();
        }
        onDoSearch: {
            console.log("onDoSearch", searchtext);
            custommodel.method = "search";
            custommodel.searchTerm=searchtext;
            custommodel.update();
        }
        onSetService: {
            console.log("onSetService", servicename)
            custommodel.service=servicename;
            if(servicename == "OSM"){
                mapGis.datacopyrighttext =
                        "<a href=\"https://osm.org/copyright\">OpenStreetMap contributors, ODbL 1.0.</a>";
            }
            else if(servicename == "GSI"){
                mapGis.datacopyrighttext =
                        "<font size=1>CSISシンプルジオコーディング実験（<a href=\"https://www.gsi.go.jp/MAP/CD-ROM/chi25000/chimei.htm\">数値地図25000地名・公共施設</a>）利用</font>";
            }
            else{
                mapGis.datacopyrighttext =
                        "<a href=\"https://www.yamareco.com/\">Yamareco Inc.</a>";
            }
        }
    }

    MapGis{
        id: mapGis
        property variant searchCoordinate: mapGis.center
        anchors.fill: parent
        onZoomLevelChanged: {
            if( zoomLevel > 14){
                sbar.nbuttonVisbile = true;
                custommodel.range = 3;
            }
            else{
                sbar.nbuttonVisbile = false;
                custommodel.range = 1;
            }
        }
        MapItemView{
            id: mapitemview
            model: custommodel
            delegate: CustomSearchMapDelegate{
                onItemClicked: {
                    console.log("detail", detail);
                    if(detail.length !== 0){
                        detailtip.text = detail;
                        detailtip.open();
                    }
                }
                ToolTip{
                    id: detailtip
                    timeout: 3000
                }
            }
        }
    }

    Drawer {
        id: resultdrawer
        width: 0.5 * parent.width
        y: header.height
        height: parent.height - header.height
        opacity: 0.9
        ListView{
            id: resultview
            anchors.fill: parent
            model: CustomSearchModel{
                id: custommodel
                limit: 50
                //range: 2
                onStatusChanged: {
                    console.log("status=", status, "count=", count);
                    switch(status){
                    case CustomSearchModel.Error:
                        errmessage.text = errorString;
                        errdialog.open();
                        break;
                    case CustomSearchModel.Ready:
                        if(count !== 0){
                            resultdrawer.open();
                        }
                        else{
                            errmessage.text = "NODATA";
                            errdialog.open();
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            delegate: CustomSearchDelegate{
                onListClicked: {
                    resultdrawer.close();
                    mapGis.center=coordinate;
                    mapGis.zoomLevel=15;
                }
            }
            footer: Button {
                id: clearbutton
                text: qsTr("Clear")
                width: parent.width
                visible: resultview.count!=0 ? true : false
                onClicked: {
                    console.log("clear")
                    custommodel.reset();
                    resultdrawer.close();
                }
            }
        }
    }
    Dialog {
        id: errdialog
        title: qsTr("エラー")
        modal: true
        Label {
            id: errmessage
            width: parent.width
            wrapMode: Text.Wrap
            //text: "エラー内容"
        }
        anchors.centerIn: parent
        width: parent.width*2/3
        standardButtons: Dialog.Ok
    }
}
