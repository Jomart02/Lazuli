#include "MainWindow.h"
#include <QMessageBox>
#include <QShortcut>
#include "global_vars.h"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow),mapController(new MapControl(this)){
    ui->setupUi(this);
    loadPlugins();
    StyleManager::getInstance()->init(CUSTOM_STYLES);

    connect(mapController,&MapControl::mapClose,this,&MainWindow::mapClosed);

    connect(ui->ShowPages, &QAction::toggled, this,&MainWindow::openCloseSensorsPanel);
    connect(ui->sendOptions, &QAction::toggled, this,&MainWindow::openCloseSendPanel);
    connect(ui->map, &QAction::toggled, this,&MainWindow::openCloseMap);
    connect(ui->Exit, &QAction::toggled, [=](){
       this->close();
    });

    QShortcut* fullscreenHK = new QShortcut(QKeySequence(Qt::Key_F11), this);
    connect(fullscreenHK, & QShortcut::activated, this, [=](){
        if(isFullScreen()) showNormal();
        else showFullScreen();
    });

    QShortcut* style = new QShortcut(QKeySequence(Qt::Key_F10), this);
    connect(style, & QShortcut::activated, this, [=](){
        if(aaa) setStyleSheet(StyleManager::getInstance()->getStyle(objectName(), "night"));
        else setStyleSheet(StyleManager::getInstance()->getStyle(objectName(), "day"));
        aaa = !aaa;
    });

    connect(ui->menuCnbkb,&QMenu::triggered, [=](QAction *action){
        QString theme = action->property("style").toString();
        qDebug() << theme;
        setStyleSheet(StyleManager::getInstance()->getStyle(objectName(), theme));
    });

    connect(ui->sendPanel,&SendWidget::startSend,this,&MainWindow::sendStateChange);
    connect(ui->sendPanel,&SendWidget::bindParam,this,&MainWindow::bindSocket);
    connect(ui->treeWidget, & QTreeWidget::currentItemChanged, this, &MainWindow::currentPageChange);


    StyleManager::getInstance()->setQssFile(objectName(),":/style/style");
    StyleManager::getInstance()->addStyle(objectName(), "day", ":/style/day");
    StyleManager::getInstance()->addStyle(objectName(), "night", ":/style/night");
    if(StyleManager::getInstance()->isCustomStyles(CUSTOM_STYLES)) loadCustomThemes();
    setStyleSheet(StyleManager::getInstance()->getStyle(objectName(), "day"));
}

MainWindow::~MainWindow(){
    
}

void MainWindow::loadPlugins(){
    PluginLoader pluginLoader;
    EnumCode::ErrorCodeDir errorLoad = pluginLoader.init();

    QMetaEnum metaEnumLoad = QMetaEnum::fromType<EnumCode::ErrorCodeDir>();

    if(errorLoad != EnumCode::ErrorCodeDir::DIRECTORY_ALREADY_EXISTS){
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

void MainWindow::loadCustomThemes(){
    auto styles =  StyleManager::getInstance()->getCustomStyles(CUSTOM_STYLES);
    if(!styles.empty()){
        for(auto style : styles){
            QAction * styleAction = new QAction(ui->menuCnbkb);
            styleAction->setText(style.first);
            styleAction->setProperty("style" ,style.first );
            ui->menuCnbkb->addAction(styleAction);
            StyleManager::getInstance()->addStyle(objectName(), style.first, style.second);
        }
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


void MainWindow::openCloseMap(bool state){
    if (state) {
        mapController->showMap();
    } else {
        mapController->closeMap();
    }
}
void MainWindow::openCloseSendPanel(bool state){
    ui->sendPanel->setVisible(state);
    QGridLayout *layout = qobject_cast<QGridLayout*>(centralWidget()->layout());
    if (layout) {
        if (state) {
            layout->setRowStretch(0, 6); // Восстанавливаем растяжение для верхней части
            layout->setRowStretch(1, 1); // Восстанавливаем растяжение для sendPanel
        } else {
            layout->setRowStretch(0, 7); // Увеличиваем растяжение для верхней части
            layout->setRowStretch(1, 0); // Убираем растяжение для sendPanel
        }
    }
}
void MainWindow::openCloseSensorsPanel(bool state){
    ui->treeWidget->setVisible(state);
    QGridLayout *layout = qobject_cast<QGridLayout*>(centralWidget()->layout());
    if (layout) {
        if (state) {
            layout->setColumnStretch(0, 1); // Восстанавливаем растяжение для treeWidget
            layout->setColumnStretch(1, 6); // Восстанавливаем растяжение для stackedWidget
        } else {
            layout->setColumnStretch(0, 0); // Убираем растяжение для treeWidget
            layout->setColumnStretch(1, 7); // Увеличиваем растяжение для stackedWidget
        }
    }
}
void MainWindow::mapClosed(){
    ui->map->setChecked(false);
}