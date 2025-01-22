#pragma once 

#include <QObject>
#include <QString>
#include <QVector>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include "BaseNaviWidget.h"

class PluginLoader : public QObject {
    Q_OBJECT
public:
    explicit PluginLoader(QObject *parent = nullptr);
    QVector<BaseNaviWidget*> loadPlugins(const QString &path); // Загрузка плагинов из указанной папки
};