#include "MainWindow.h"
#include <QMessageBox>
#include <QShortcut>
#include "global_vars.h"
#include <QSettings>
#include "StyleManager.h"
#include "PluginLoader.h"
#include "udp_sender.h"
#include "SerialSender.h"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow),mapController(new MapControl(this)),
translator(new QTranslator(this)),topPanel(new TopPanel(this)){
    ui->setupUi(this);
    setupUI();
    loadPlugins();
    loadTranslate();
    Q_INIT_RESOURCE(translations);
    StyleManager::getInstance()->init(CUSTOM_STYLES);
    
    connect(mapController,&MapControl::mapClose,this,&MainWindow::mapClosed);


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

void MainWindow::setupUI(){

    ui->toolBar->addWidget(topPanel);

    connect(topPanel, &TopPanel::showPagesTriggered, this,&MainWindow::openCloseSensorsPanel);
    connect(topPanel, &TopPanel::showSettingsTriggered, this,&MainWindow::openCloseSendPanel);
    connect(topPanel, &TopPanel::showMapTriggered, this,&MainWindow::openCloseMap);
    connect(topPanel, &TopPanel::exitTriggered, [=](){
       this->deleteLater();
    });
}

MainWindow::~MainWindow(){
    QSettings sett(CONFIG, QSettings::IniFormat);
    sett.setValue("lang", currentLang);
}

void MainWindow::loadTranslate(){
    // Создание меню для выбора языка
    QAction *englishAction = new QAction(QCoreApplication::translate("MainWindow", "en"), this);
    englishAction->setData("en");
    QAction *russianAction = new QAction(QCoreApplication::translate("MainWindow", "ru"), this);
    russianAction->setData("ru");

    ui->languageMenu->addAction(englishAction);
    ui->languageMenu->addAction(russianAction);

    connect(englishAction, &QAction::triggered, this, &MainWindow::onLanguageChanged);
    connect(russianAction, &QAction::triggered, this, &MainWindow::onLanguageChanged);

    QSettings sett(CONFIG, QSettings::IniFormat);
   
    currentLang = sett.value("lang").toString();
    if(!currentLang.isEmpty()){
        changeLanguage(currentLang);
    }
}

void MainWindow::changeLanguage(const QString &language) {
    // Удаляем текущий перевод
    if (qApp->removeTranslator(translator)) {
        qDebug() << "Removed previous translator";
    }
    currentLang = language;
    // Загружаем новый перевод
    if (translator->load(":/translations/" + language)) {
        if (qApp->installTranslator(translator)) {
            qDebug() << "Installed translator for" << language;
            ui->retranslateUi(this);
        } else {
            qDebug() << "Failed to install translator for" << language;
        }
    } else {
        qDebug() << "Failed to load translation file for" << language;
    }
    emit retranslate(language);

    // QEvent languageChangeEvent(QEvent::LanguageChange);
    // QApplication::sendEvent(this, &languageChangeEvent);
}


void MainWindow::onLanguageChanged() {
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        QString language = action->data().toString();
        changeLanguage(language);
    }
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
        
        SendParam data;
        data.curSendType = TypeSendInterface::ETHERNET;
        data.active = false;
        data.etherParam = {"127.0.0.1", 20000};
        data.comParam = {"", QSerialPort::Baud9600, QSerialPort::Data8, QSerialPort::NoParity, 
                        QSerialPort::OneStop, QSerialPort::NoFlowControl};
        
        Sender *sender = new UdpSender(data.etherParam,this);
        senders[plugin] = sender;
        connect(plugin,&BaseNaviWidget::sendData, sender,QOverload<QStringList >::of(&Sender::setData));
        connect(this,&MainWindow::retranslate,plugin,&BaseNaviWidget::setRetranslate);
        
        
        item->setData(0, Qt::UserRole,QVariant::fromValue(data));
        

        ui->sendPanel->setSendParam(data);

        if(currentPage == nullptr){
            currentPage = item;
            connect(sender,&Sender::dataSent, ui->sendPanel,&SendWidget::setData);
            connect(mapController,&MapControl::mapClicked, plugin,&BaseNaviWidget::setPos);
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

void MainWindow::bindSocket(TypeSendInterface typeBind , QVariant _bindParam){
    if(currentPage == nullptr) return;
    BaseNaviWidget* widg = pageMap[currentPage];

    if (!widg || senders.find(widg) == senders.end()) {
        qWarning() << "Invalid widget or sender not found";
        return;
    }
    SendParam param = currentPage->data(0, Qt::UserRole).value<SendParam>();
    bool recreateSender = (param.curSendType != typeBind);
    param.curSendType = typeBind;

    Sender* sender = senders[widg];

    if(typeBind == TypeSendInterface::ETHERNET){
        param.etherParam = _bindParam.value<EthernetParams>();

        if (recreateSender) {
            delete sender;
            senders.erase(widg);
            sender = new UdpSender(param.etherParam, this);
            senders[widg] = sender;
            connect(widg, &BaseNaviWidget::sendData, sender, &Sender::setData);
            connect(sender,&Sender::dataSent, ui->sendPanel,&SendWidget::setData);
        } else {
            sender->setTarget(QVariant::fromValue(param.etherParam));
        }
    
    }else{
        param.comParam = _bindParam.value<ComPortParams>();
        
        if (recreateSender) {
            delete sender;
            senders.erase(widg);
            sender = new SerialSender(param.comParam, this);
            senders[widg] = sender;
            connect(widg, &BaseNaviWidget::sendData, sender, &Sender::setData);
            connect(sender,&Sender::dataSent, ui->sendPanel,&SendWidget::setData);
        } else {
            sender->setTarget(QVariant::fromValue(param.comParam));
        }
    }
    
    currentPage->setData(0, Qt::UserRole,QVariant::fromValue(param));
}


void MainWindow::currentPageChange(QTreeWidgetItem *current, QTreeWidgetItem *previous){
    auto it = pageMap.find(current);
    if (it != pageMap.end()) {
        BaseNaviWidget* widget = it->second;
        if(widget){
            SendParam param = current->data(0, Qt::UserRole).value<SendParam>();
            ui->sendPanel->setSendParam(param);
            ui->stackedWidget->setCurrentWidget(widget);
            if(previous) {
                auto itp = pageMap.find(previous);
                if (itp != pageMap.end()) {
                    Sender* senderprev = senders[itp->second];
                    disconnect(senderprev,&Sender::dataSent, ui->sendPanel,&SendWidget::setData);
                    disconnect(mapController,&MapControl::mapClicked, itp->second,&BaseNaviWidget::setPos);
                }
            }
            Sender* sender = senders[widget];
            connect(sender,&Sender::dataSent, ui->sendPanel,&SendWidget::setData);
            connect(mapController,&MapControl::mapClicked, widget,&BaseNaviWidget::setPos);
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
    topPanel->uncheckMap();
}