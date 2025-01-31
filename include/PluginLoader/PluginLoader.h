#pragma once 

#include <QObject>
#include <QString>
#include <QVector>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include "BaseNaviWidget.h"
#include <QMetaEnum>
#include "global_vars.h"
class PluginLoader : public QObject {
    Q_OBJECT
public:

    enum class ErrorCodePlugins{
        PLUGIN_IS_NOT_A_BASENAVIWIDGET,
        FAILED_TO_LOAD_PLUGIN
    };
    Q_ENUM(ErrorCodePlugins)

    struct ErrorDataPlugins{
        ErrorCodePlugins errorCode;
        QString filename;
        QString error;
    };

    using ErrorsLoad = QVector<ErrorDataPlugins>;

    explicit PluginLoader(QObject *parent = nullptr);
    QVector<BaseNaviWidget*> loadPlugins(ErrorsLoad &errors,const QString &path = PLAGIN_DATA_DIR); // Загрузка плагинов из указанной папки
    EnumCode::ErrorCodeDir init();
};