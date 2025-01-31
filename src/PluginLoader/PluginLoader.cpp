#include "PluginLoader.h"


PluginLoader::PluginLoader(QObject *parent) : QObject(parent) {}

EnumCode::ErrorCodeDir PluginLoader::init(){
    QDir dir(PLAGIN_DATA_DIR);

    QMetaEnum metaEnum = QMetaEnum::fromType<EnumCode::ErrorCodeDir>();
    
    EnumCode::ErrorCodeDir errorCode;
    if(!dir.exists()) {
        if(!dir.mkpath(".")) {
            const char* errorCodeStr = metaEnum.valueToKey(static_cast<int>(EnumCode::ErrorCodeDir::FAILED_TO_CREATE));
            qWarning() << errorCodeStr << PLAGIN_DATA_DIR;
            return EnumCode::ErrorCodeDir::FAILED_TO_CREATE;
        } else {
            errorCode = EnumCode::ErrorCodeDir::CREATED_DIRECTORY;
        }
    } else {
        errorCode = EnumCode::ErrorCodeDir::DIRECTORY_ALREADY_EXISTS;
    }

    const char* errorCodeStr = metaEnum.valueToKey(static_cast<int>(errorCode));
    qDebug() << errorCode << PLAGIN_DATA_DIR;
    return errorCode;
}


QVector<BaseNaviWidget*> PluginLoader::loadPlugins(ErrorsLoad &errors,const QString &path) {
    QVector<BaseNaviWidget*> plugins;
    QDir pluginsDir(path);
    QMetaEnum metaEnum = QMetaEnum::fromType<ErrorCodePlugins>();

    for (const QString &fileName : pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();

        if (plugin) {
            BaseNaviWidget *naviWidget = qobject_cast<BaseNaviWidget*>(plugin);
            if (naviWidget) {
                plugins.append(naviWidget);
            } else {
                const char* errorCodeStr = metaEnum.valueToKey(static_cast<int>(ErrorCodePlugins::PLUGIN_IS_NOT_A_BASENAVIWIDGET));
                qWarning() << errorCodeStr << fileName;
                errors.push_back({ErrorCodePlugins::PLUGIN_IS_NOT_A_BASENAVIWIDGET,fileName,""});
            }
        } else {
            const char* errorCodeStr = metaEnum.valueToKey(static_cast<int>(ErrorCodePlugins::FAILED_TO_LOAD_PLUGIN));
            qWarning() << errorCodeStr << fileName << "Error:" << pluginLoader.errorString();
            errors.push_back({ErrorCodePlugins::PLUGIN_IS_NOT_A_BASENAVIWIDGET,fileName,pluginLoader.errorString()});
        }
    }

    return plugins;
}