#include "TopPanel.h"
#include "ui_TopPanel.h"
#include <QGraphicsDropShadowEffect>
TopPanel::TopPanel(QWidget *parent) : QWidget(parent) , ui(new Ui::TopPanel){
    ui->setupUi(this);
   
     connect(ui->settingsSend, &QPushButton::toggled,this,&TopPanel::showSettingsTriggered);
     connect(ui->showPages, &QPushButton::toggled,this,&TopPanel::showPagesTriggered);
     connect(ui->Map, &QPushButton::toggled,this,&TopPanel::showMapTriggered);
     connect(ui->Exit, &QPushButton::clicked,this,&TopPanel::exitTriggered);


     QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
     shadowEffect->setBlurRadius(10);
     shadowEffect->setOffset(0, 0);
     shadowEffect->setColor(QColor("#c8c8c8"));
     ui->settingsSend->setGraphicsEffect(shadowEffect);

     QGraphicsDropShadowEffect* shadowEffect1 = new QGraphicsDropShadowEffect(this);
     shadowEffect1->setBlurRadius(10);
     shadowEffect1->setOffset(0, 0);
     shadowEffect1->setColor(QColor("#c8c8c8"));
     ui->showPages->setGraphicsEffect(shadowEffect1);
     QGraphicsDropShadowEffect* shadowEffect2 = new QGraphicsDropShadowEffect(this);
     shadowEffect2->setBlurRadius(10);
     shadowEffect2->setOffset(0, 0);
     shadowEffect2->setColor(QColor("#c8c8c8"));
     ui->Map->setGraphicsEffect(shadowEffect2);
     QGraphicsDropShadowEffect* shadowEffect3 = new QGraphicsDropShadowEffect(this);
     shadowEffect3->setBlurRadius(10);
     shadowEffect3->setOffset(0, 0);
     shadowEffect3->setColor(QColor("#c8c8c8"));
     ui->Exit->setGraphicsEffect(shadowEffect3);
}   

TopPanel::~TopPanel()
{
    delete ui;
}
void TopPanel::uncheckMap(){
     ui->Map->setChecked(false);
}
void TopPanel::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}
