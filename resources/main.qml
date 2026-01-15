import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Map"

    signal mapClickedSignal(double latitude, double longitude)
    signal windowClosed()

    onClosing: {
        console.log("Window is closing...");
        windowClosed();
    }

    Plugin {
        id: mapPlugin
        name: "osm" // Используем OSM плагин

        PluginParameter {
            name:"osm.mapping.custom.host"
            value: "https://tile.openstreetmap.org/"
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        activeMapType: map.supportedMapTypes[map.supportedMapTypes.length - 1]
        center: QtPositioning.coordinate(55.751244, 37.618423) // Москва
        zoomLevel: 12

        // Маркер на карте
        property var markerCoordinate: null // Текущая позиция маркера

        // Функция для форматирования координат
        function formatCoordinate(latitude, longitude) {
            // Форматирование широты
            var latAbs = Math.abs(latitude);
            var latDeg = Math.floor(latAbs);
            var latMin = (latAbs - latDeg) * 60;
            var latHemisphere = latitude >= 0 ? "N" : "S";
            var latString = latDeg.toString().padStart(2, "0") + "° " +
                           latMin.toFixed(3).padStart(6, "0") + "' " + latHemisphere;

            // Форматирование долготы
            var lonAbs = Math.abs(longitude);
            var lonDeg = Math.floor(lonAbs);
            var lonMin = (lonAbs - lonDeg) * 60;
            var lonHemisphere = longitude >= 0 ? "E" : "W";
            var lonString = lonDeg.toString().padStart(3, "0") + "° " +
                           lonMin.toFixed(3).padStart(6, "0") + "' " + lonHemisphere;

            return { latitude: latString, longitude: lonString };
        }

        MapQuickItem {
            id: marker
            anchorPoint.x: image.width / 2
            anchorPoint.y: image.height / 2
            coordinate: map.markerCoordinate // Привязываем к свойству markerCoordinate
            sourceItem: Item {
                width: image.width
                height: image.height

                Image {
                    id: image
                    source: "qrc:/cur.svg" // Путь к изображению маркера
                    width: 18
                    height: 18
                }

                // Всплывающее меню (ToolTip)
                ToolTip {
                    id: markerTooltip
                    text: map.markerCoordinate ? 
                        "Широта: " + map.formatCoordinate(map.markerCoordinate.latitude, map.markerCoordinate.longitude).latitude + "\n" +
                        "Долгота: " + map.formatCoordinate(map.markerCoordinate.latitude, map.markerCoordinate.longitude).longitude : 
                        "Координаты: Не задано"
                    visible: markerMouseArea.containsMouse
                    delay: 200
                    timeout: 5000 // Время отображения в миллисекундах
                    background: Rectangle {
                        border.color: "#555"
                        color: "#f0f0f0"
                        radius: 3
                    }
                }

                // MouseArea для обработки наведения на маркер
                MouseArea {
                    id: markerMouseArea
                    anchors.fill: parent
                    hoverEnabled: true // Включаем отслеживание наведения
                    acceptedButtons: Qt.NoButton // Не обрабатываем клики, только наведение
                }
            }
            visible: map.markerCoordinate !== null // Показываем только если координата задана
        }

        // Отображение координат курсора
        Rectangle {
            id: cursorCoordinates
            anchors {
                top: parent.top
                left: parent.left
                margins: 10
            }
            color: "#ffffff"
            opacity: 0.5
            radius: 5
            width: coordinatesColumn.implicitWidth + 10 // Учитываем padding
            height: coordinatesColumn.implicitHeight + 10 // Учитываем padding

            Column {
                id: coordinatesColumn
                anchors.centerIn: parent
                spacing: 5

                Label {
                    id: latitudeLabel
                    text: "Широта: Неизвестно"
                    font.pixelSize: 14
                    color: "black"
                }
                Label {
                    id: longitudeLabel
                    text: "Долгота: Неизвестно"
                    font.pixelSize: 14
                    color: "black"
                }
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true // Включаем отслеживание наведения для координат
            NumberAnimation {
                id: zoomAnimation
                target: map
                property: "zoomLevel"
                duration: 200
                easing.type: Easing.InOutQuad
            }

            property var dragStartPoint: undefined // Точка начала перетаскивания
            property bool isDragging: false 

            onPressed: function(mouse) {
                dragStartPoint = Qt.point(mouse.x, mouse.y)
                isDragging = false
            }

            onPositionChanged: function(mouse) {
                if (mouse.buttons & Qt.LeftButton && dragStartPoint) {
                    var deltaX = mouse.x - dragStartPoint.x
                    var deltaY = mouse.y - dragStartPoint.y

                    if (deltaX > 5 || deltaY > 5) {
                        isDragging = true 
                    }

                    var currentCenter = map.toCoordinate(Qt.point(map.width / 2, map.height / 2))
                    var newCenterPixel = Qt.point(
                        map.fromCoordinate(currentCenter).x - deltaX,
                        map.fromCoordinate(currentCenter).y - deltaY
                    )
                    map.center = map.toCoordinate(newCenterPixel)
                    dragStartPoint = Qt.point(mouse.x, mouse.y)
                }

                // Обновляем координаты курсора
                var cursorCoord = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                var formatted = map.formatCoordinate(cursorCoord.latitude, cursorCoord.longitude)
                latitudeLabel.text = "Широта: " + formatted.latitude
                longitudeLabel.text = "Долгота: " + formatted.longitude
            }

            onReleased: function() {
                dragStartPoint = undefined
            }

            onClicked: function(mouse) {
                if (isDragging) {
                    return
                }
                var clickCoordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                console.log("Clicked at latitude:", clickCoordinate.latitude, "longitude:", clickCoordinate.longitude)

                // Обновляем позицию маркера
                map.markerCoordinate = clickCoordinate

                // Отправляем сигнал о клике
                mapClickedSignal(clickCoordinate.latitude, clickCoordinate.longitude)
            }

            onWheel: function(wheelEvent) {
                var targetZoomLevel = wheelEvent.angleDelta.y > 0 ? map.zoomLevel + 1 : map.zoomLevel - 1;
                targetZoomLevel = Math.max(1, Math.min(20, targetZoomLevel));
                zoomAnimation.to = targetZoomLevel;
                zoomAnimation.restart();
                wheelEvent.accepted = true;
            }

            onExited: function() {
                latitudeLabel.text = "Широта: Неизвестно"
                longitudeLabel.text = "Долгота: Неизвестно"
            }
        }
    }
}
