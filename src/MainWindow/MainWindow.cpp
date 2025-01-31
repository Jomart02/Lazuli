#include "MainWindow.h"
#include <QMessageBox>
#include <QShortcut>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    loadPlugins();
    connect(ui->ShowPages, &QAction::toggled, [=](bool checked){
        ui->treeWidget->setVisible(checked);
        QGridLayout *layout = qobject_cast<QGridLayout*>(centralWidget()->layout());
        if (layout) {
            if (checked) {
                layout->setColumnStretch(0, 1); // Восстанавливаем растяжение для treeWidget
                layout->setColumnStretch(1, 6); // Восстанавливаем растяжение для stackedWidget
            } else {
                layout->setColumnStretch(0, 0); // Убираем растяжение для treeWidget
                layout->setColumnStretch(1, 7); // Увеличиваем растяжение для stackedWidget
            }
        }
    });
    connect(ui->sendOptions, &QAction::toggled, [=](bool checked){
        ui->sendPanel->setVisible(checked);
        QGridLayout *layout = qobject_cast<QGridLayout*>(centralWidget()->layout());
        if (layout) {
            if (checked) {
                layout->setRowStretch(0, 6); // Восстанавливаем растяжение для верхней части
                layout->setRowStretch(1, 1); // Восстанавливаем растяжение для sendPanel
            } else {
                layout->setRowStretch(0, 7); // Увеличиваем растяжение для верхней части
                layout->setRowStretch(1, 0); // Убираем растяжение для sendPanel
            }
        }
        
    });

    QShortcut* fullscreenHK = new QShortcut(QKeySequence(Qt::Key_F11), this);
    connect(fullscreenHK, & QShortcut::activated, this, [=](){
        if(isFullScreen()) showNormal();
        else showFullScreen();
    });

    QShortcut* style = new QShortcut(QKeySequence(Qt::Key_F10), this);
    connect(style, & QShortcut::activated, this, [=](){
        if(aaa) setStyleSheet(manS->getStyle(objectName(), "night"));
        else setStyleSheet(manS->getStyle(objectName(), "day"));
        aaa = !aaa;
    });

    connect(ui->sendPanel,&SendWidget::startSend,this,&MainWindow::sendStateChange);
    connect(ui->sendPanel,&SendWidget::bindParam,this,&MainWindow::bindSocket);
    connect(ui->treeWidget, & QTreeWidget::currentItemChanged, this, &MainWindow::currentPageChange);

    manS = new StyleManager(this);
    manS->setQssFile(objectName(),":/style/style");
    manS->addStyle(objectName(), "day", ":/style/day");
    manS->addStyle(objectName(), "night", ":/style/night");
    setStyleSheet(manS->getStyle(objectName(), "day"));
}

MainWindow::~MainWindow(){
    
}

void MainWindow::loadPlugins(){
    PluginLoader pluginLoader;
    PluginLoader::ErrorCodeDir errorLoad = pluginLoader.init();

    QMetaEnum metaEnumLoad = QMetaEnum::fromType<PluginLoader::ErrorCodeDir>();

    if(errorLoad != PluginLoader::ErrorCodeDir::DIRECTORY_ALREADY_EXISTS){
        const char* errorCodeStr = metaEnumLoad.valueToKey(static_cast<int>(errorLoad));
        QTimer::singleShot(1000,[=](){
             QMessageBox::warning(this,tr("Ошибка загрузки"),errorCodeStr);
        });
        return;
    }

    PluginLoader::ErrorsLoad errors;
    QVector<BaseNaviWidget*> plugins = pluginLoader.loadPlugins(errors);
    
    for (BaseNaviWidget* plugin : plugins) {
        ui->stackedWidget->addWidget(plugin);

        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, plugin->name());
        item->setIcon(0, plugin->icon());
        ui->treeWidget->addTopLevelItem(item);

        pageMap[item] = plugin;
        
        UdpSender *sender = new UdpSender("127.0.0.1",20000,this);
        senders[plugin] = sender;
        connect(plugin,&BaseNaviWidget::sendData, sender,QOverload<QStringList>::of(&UdpSender::setData));
        
        ui->sendPanel->setSendParam(false, "127.0.0.1",20000);
        
        SendParam data = {"127.0.0.1",20000,false};
        item->setData(0, Qt::UserRole,QVariant::fromValue(data));

        if(currentPage == nullptr){
            currentPage = item;
            connect(plugin,&BaseNaviWidget::sendData, ui->sendPanel,&SendWidget::setData);
        }
    }
    if(currentPage){
        ui->stackedWidget->setCurrentWidget(pageMap[currentPage]);
    }
}

void MainWindow::sendStateChange(bool state){
    if(currentPage == nullptr) return;
    if(state) pageMap[currentPage]->startSend();
    else pageMap[currentPage]->stopSend();
    
    SendParam param = currentPage->data(0, Qt::UserRole).value<SendParam>();
    param.active = state;
    currentPage->setData(0, Qt::UserRole,QVariant::fromValue(param));
}

void MainWindow::bindSocket(QString ip, quint16 port){
    if(currentPage == nullptr) return;
    BaseNaviWidget* widg = pageMap[currentPage];
    senders[widg]->setTarget(ip, port);
    
    SendParam param = currentPage->data(0, Qt::UserRole).value<SendParam>();
    param.ip = ip;
    param.port = port;
    currentPage->setData(0, Qt::UserRole,QVariant::fromValue(param));
}


void MainWindow::currentPageChange(QTreeWidgetItem *current, QTreeWidgetItem *previous){
    auto it = pageMap.find(current);
    if (it != pageMap.end()) {
        BaseNaviWidget* widget = it->second;
        if(widget){
            SendParam param = current->data(0, Qt::UserRole).value<SendParam>();
            ui->sendPanel->setSendParam(param.active, param.ip, param.port);
            ui->stackedWidget->setCurrentWidget(widget);
            if(previous) {
                auto itp = pageMap.find(previous);
                if (itp != pageMap.end()) {
                    disconnect(itp->second,&BaseNaviWidget::sendData, ui->sendPanel,&SendWidget::setData);
                }
            }
            connect(widget,&BaseNaviWidget::sendData, ui->sendPanel,&SendWidget::setData);
        }
    }
    currentPage = current;
}
