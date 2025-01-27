#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    PluginLoader pluginLoader;
    QVector<BaseNaviWidget*> plugins = pluginLoader.loadPlugins(DEFAULT_DATA_DIR);

    for (BaseNaviWidget* plugin : plugins) {

        ui->stackedWidget->addWidget(plugin);
    }
    connect(ui->ShowPages, &QAction::toggled, [=](bool checked){
        ui->treeWidget->setVisible(checked);
    });
}

MainWindow::~MainWindow(){
    
}