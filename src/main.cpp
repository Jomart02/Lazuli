#include <QApplication>
#include "PluginLoader.h"
#define DEFAULT_DATA_DIR QDir::toNativeSeparators(QDir::homePath()) + QDir::separator() +"test" + QDir::separator()

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);   
    PluginLoader pluginLoader;
    QVector<BaseNaviWidget*> plugins = pluginLoader.loadPlugins(DEFAULT_DATA_DIR);

    for (BaseNaviWidget* plugin : plugins) {
        plugin->show();
    }

    return app.exec(); 
}