#include "MapControl.h"
#include <QtQml>
#include <QtQuick>
#include <QDebug>

MapControl::MapControl(QObject *parent)
    : QObject(parent), rootObject(nullptr) {}

MapControl::~MapControl() {
    if (rootObject) {
          QObject::disconnect(rootObject, SIGNAL(mapClickedSignal(double, double)),
     this, SLOT(handleMapClick(double, double)));
        delete rootObject;
    }
}

void MapControl::showMap() {
     if (!windowOpenState) { // Проверяем, что окно не открыто
         // Загружаем QML-файл
         engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
 
         // Проверяем, успешно ли загружен QML
         if (engine.rootObjects().isEmpty()) {
             qWarning() << "Failed to load QML file!";
             return;
         }
 
         // Получаем корневой объект QML
         rootObject = engine.rootObjects().first();
 
         // Устанавливаем соединение между сигналом из QML и слотом в C++
         QObject::connect(rootObject, SIGNAL(mapClickedSignal(double, double)),
                          this, SLOT(handleMapClick(double, double)));
          QObject::connect(rootObject, SIGNAL(windowClosed()),
                          this, SLOT(handleWindowClosed()));
         // Устанавливаем состояние окна как "открыто"
         windowOpenState = true;
     } else {
         qDebug() << "Window is already open!";
     }
 }
 
 void MapControl::handleWindowClosed() {
     qDebug() << "Window was closed by the user.";
 
     // Вызываем метод closeMap() для очистки ресурсов
     closeMap();
     emit mapClose();
 }
 

 // Метод для закрытия окна
 void MapControl::closeMap() {
     if (windowOpenState) { // Проверяем, что окно открыто
         if (rootObject) {
          QObject::disconnect(rootObject, SIGNAL(mapClickedSignal(double, double)),
          this, SLOT(handleMapClick(double, double)));   
               // Закрываем окно
             QQuickWindow *window = qobject_cast<QQuickWindow *>(rootObject);
             if (window) {
                 window->close();
             }
             
             // Очищаем ссылку на корневой объект
             rootObject->deleteLater();
 
             // Устанавливаем состояние окна как "закрыто"
             windowOpenState = false;
         }
     } else {
         qDebug() << "Window is already closed!";
     }
 }
 
 // Геттер для проверки состояния окна
 bool MapControl::isWindowOpen() const {
     return windowOpenState;
 }

void MapControl::handleMapClick(double latitude, double longitude) {
    qDebug() << "Map clicked at:" << latitude << "," << longitude;
    emit mapClicked(latitude, longitude);
}