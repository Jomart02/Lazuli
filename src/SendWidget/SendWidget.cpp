#include "SendWidget.h"

SendWidget::SendWidget(QWidget *parent) : QWidget(parent) , ui(new Ui::SendWidget){
    ui->setupUi(this);
    ui->textEdit->document()->setMaximumBlockCount(100);
    
    connect(ui->pushButtonOnOff,&QPushButton::clicked,this,&SendWidget::startSend);
    connect(ui->pushButtonBind,&QPushButton::clicked,this,&SendWidget::bind);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression ipRegex("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    ui->lineEdit_IP->setValidator( new QRegularExpressionValidator(ipRegex, this));
    QString portRange = "^([0-9]{1,5})$";
    QRegularExpression portRegex(portRange);
    ui->lineEdit_Port->setValidator(new QRegularExpressionValidator(portRegex, this));
}   

SendWidget::~SendWidget()
{
    delete ui;
}

void SendWidget::setData(QStringList data){
    for(auto text: data){
        ui->textEdit->insertPlainText(text);
    }
}

void SendWidget::setSendParam(bool stateSend, QString ip , quint16 port){
    ui->lineEdit_Port->setText(QString::number(port));
    ui->lineEdit_IP->setText(ip);
    ui->pushButtonOnOff->blockSignals(true);
    ui->pushButtonOnOff->setChecked(stateSend);
    ui->pushButtonOnOff->blockSignals(false);
    ui->textEdit->clear();
}

void SendWidget::bind(){
    QString ip = ui->lineEdit_IP->text();
    quint16 port = ui->lineEdit_Port->text().toInt();
    emit bindParam(ip, port);
}

void SendWidget::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}
