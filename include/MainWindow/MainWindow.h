#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "PluginLoader.h"
#include "udp_sender.h"

namespace Ui
{
    class MainWindow;
}
#define DEFAULT_DATA_DIR QDir::toNativeSeparators(QDir::homePath()) + QDir::separator() +"test" + QDir::separator()

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
    private:
        void loadPlugins();
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
};