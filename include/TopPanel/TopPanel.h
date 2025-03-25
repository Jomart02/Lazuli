#pragma once
#include <QWidget>

namespace Ui { class TopPanel; }

class TopPanel : public QWidget{
    Q_OBJECT
public:
     TopPanel(QWidget *parent);
    ~TopPanel();
   
signals:
     void showMapTriggered(bool state);
     void showSettingsTriggered(bool state);
     void showPagesTriggered(bool state);
     void exitTriggered();

public slots:
     void uncheckMap();
protected:
    void changeEvent(QEvent *event) override;
private:
    Ui::TopPanel *ui;
};

