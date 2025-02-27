
#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QGeoCoordinate>

class MapControl : public QObject {
    Q_OBJECT

public:
    explicit MapControl(QObject *parent = nullptr);
    ~MapControl();


     void showMap();
     void closeMap();
     bool isWindowOpen() const;
signals:
    // Сигнал, отправляемый при клике на карте
     void mapClicked(double latitude, double longitude);
     void mapClose();
private slots:
     // Слот для обработки сигнала клика из QML
     void handleMapClick(double latitude, double longitude);
     void handleWindowClosed();
private:
    QQmlApplicationEngine engine;
    QObject *rootObject; // Корневой объект QML
    bool windowOpenState = false;
};