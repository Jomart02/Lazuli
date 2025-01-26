#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"


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
    private:
        Ui::MainWindow* ui;
};