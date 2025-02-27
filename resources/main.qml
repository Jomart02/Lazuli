import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Online Map Example"

    signal mapClickedSignal(double latitude, double longitude)
    signal windowClosed()

    onClosing: {
        console.log("Window is closing...");
        windowClosed(); 
    }
    Plugin {
        id: mapPlugin
        name: "osm" // Используем OSM плагин
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(55.751244, 37.618423) // Москва
        zoomLevel: 12

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            NumberAnimation {
                id: zoomAnimation
                target: map
                property: "zoomLevel"
                duration: 200
                easing.type: Easing.InOutQuad
            }

            property var dragStartPoint: undefined // Точка начала перетаскивания

            onPressed: function(mouse) {
                dragStartPoint = Qt.point(mouse.x, mouse.y)
            }

            onPositionChanged: function(mouse) {
                if (mouse.buttons & Qt.LeftButton && dragStartPoint) {
                    var deltaX = mouse.x - dragStartPoint.x
                    var deltaY = mouse.y - dragStartPoint.y
                    var currentCenter = map.toCoordinate(Qt.point(map.width / 2, map.height / 2))
                    var newCenterPixel = Qt.point(
                        map.fromCoordinate(currentCenter).x - deltaX,
                        map.fromCoordinate(currentCenter).y - deltaY
                    )
                    map.center = map.toCoordinate(newCenterPixel)
                    dragStartPoint = Qt.point(mouse.x, mouse.y)
                }
            }

            onReleased: function() {
                dragStartPoint = undefined
            }

            onClicked: function(mouse) {
                var clickCoordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                console.log("Clicked at latitude:", clickCoordinate.latitude, "longitude:", clickCoordinate.longitude)
                mapClickedSignal(clickCoordinate.latitude, clickCoordinate.longitude)
            }

            onWheel: function(wheelEvent) {
                var targetZoomLevel = wheelEvent.angleDelta.y > 0 ? map.zoomLevel + 1 : map.zoomLevel - 1;
                targetZoomLevel = Math.max(1, Math.min(20, targetZoomLevel));

                zoomAnimation.to = targetZoomLevel;
                zoomAnimation.restart();

                wheelEvent.accepted = true;
            }
        }
    }
}