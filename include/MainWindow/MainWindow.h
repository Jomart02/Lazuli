#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "PluginLoader.h"
#include "BaseSender.h"
#include "MapControl.h"
#include <QTranslator>
#include "TopPanel.h"
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();
    private slots:
        void sendStateChange(bool state);
        void bindSocket(TypeSendInterface typeBind , QVariant _bindParam);
        void currentPageChange(QTreeWidgetItem *current, QTreeWidgetItem *previous);
        void openCloseMap(bool state);
        void openCloseSendPanel(bool state);
        void openCloseSensorsPanel(bool state);
        void mapClosed();
        void onLanguageChanged();
    signals:
        void retranslate(QString retrPref);
    private:
        void setupUI();
        void loadPlugins();
        void loadCustomThemes(); 
        void loadTranslate();
        void changeLanguage(const QString &language);
    private:

        Ui::MainWindow* ui;
        std::map<QTreeWidgetItem* , BaseNaviWidget*> pageMap;
        QTreeWidgetItem* currentPage = nullptr;
        std::map<BaseNaviWidget*,Sender*> senders;
        bool aaa = true;
        MapControl *mapController;
        QTranslator *translator;
        QString currentLang;
        TopPanel *topPanel;
};