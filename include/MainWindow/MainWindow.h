#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "PluginLoader.h"
#include "udp_sender.h"
#include "StyleManager.h"
#include "MapControl.h"
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
        void bindSocket(QString ip, quint16 port);
        void currentPageChange(QTreeWidgetItem *current, QTreeWidgetItem *previous);
        void openCloseMap(bool state);
        void openCloseSendPanel(bool state);
        void openCloseSensorsPanel(bool state);
        void mapClosed();
    private:
        void loadPlugins();
        void loadCustomThemes(); 
    private:

        struct SendParam{
            QString ip;
            quint16 port;
            bool active;
        };

        Ui::MainWindow* ui;
        std::map<QTreeWidgetItem* , BaseNaviWidget*> pageMap;
        QTreeWidgetItem* currentPage = nullptr;
        std::map<BaseNaviWidget*,UdpSender*> senders;
        bool aaa = true;
        MapControl *mapController;

};