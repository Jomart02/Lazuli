#include "PluginLoader.h"


PluginLoader::PluginLoader(QObject *parent) : QObject(parent) {}

QVector<BaseNaviWidget*> PluginLoader::loadPlugins(const QString &path) {
    QVector<BaseNaviWidget*> plugins;
    QDir pluginsDir(path);
    
    for (const QString &fileName : pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();

        if (plugin) {
            BaseNaviWidget *naviWidget = qobject_cast<BaseNaviWidget*>(plugin);
            if (naviWidget) {
                plugins.append(naviWidget);
            } else {
                qWarning() << "Plugin is not a BaseNaviWidget:" << fileName;
            }
        } else {
            qWarning() << "Failed to load plugin:" << fileName << "Error:" << pluginLoader.errorString();
        }
    }

    return plugins;
}