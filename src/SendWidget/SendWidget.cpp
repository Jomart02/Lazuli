#include "SendWidget.h"
#include "ui_SendWidget.h"


SendWidget::SendWidget(QWidget *parent) : QWidget(parent) , ui(new Ui::SendWidget){
    ui->setupUi(this);
    initComSettings();
    ui->textEdit->document()->setMaximumBlockCount(100);
    
    connect(ui->pushButtonOnOff,&QPushButton::clicked,this,&SendWidget::startSend);
    connect(ui->pushButtonBind,&QPushButton::clicked,this,&SendWidget::bind);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression ipRegex("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    ui->lineEdit_IP->setValidator( new QRegularExpressionValidator(ipRegex, this));
    QString portRange = "^([0-9]{1,5})$";
    QRegularExpression portRegex(portRange);
    ui->lineEdit_Port->setValidator(new QRegularExpressionValidator(portRegex, this));
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->comboBox_typeSend, &QComboBox::currentIndexChanged, this, &SendWidget::typeChange);

    comPortScanner = new ComPortScanner(this);
    comPortScanner->setInterval(2000); 
    connect(comPortScanner, &ComPortScanner::portsUpdated, this, &SendWidget::updateComPorts);
    comPortScanner->start();
}   

SendWidget::~SendWidget()
{
    comPortScanner->stop();
    comPortScanner->wait();
    delete ui;
}

void SendWidget::setData(QStringList data){
    for(auto text: data){
        ui->textEdit->insertPlainText(text);
    }
}

void SendWidget::setSendParam(SendParam &sendParam){
    ui->lineEdit_Port->setText(QString::number(sendParam.etherParam.port));
    ui->lineEdit_IP->setText(sendParam.etherParam.ip);
    ui->pushButtonOnOff->blockSignals(true);
    ui->pushButtonOnOff->setChecked(sendParam.active);
    ui->pushButtonOnOff->blockSignals(false);

    int index = ui->comboBox_ComPort->findText(sendParam.comParam.comPortName);
    ui->comboBox_ComPort->setCurrentIndex(index >= 0 ? index : 0);

    index = ui->comboBox_ComSpeed->findData(sendParam.comParam.baudRate);
    ui->comboBox_ComSpeed->setCurrentIndex(index >= 0 ? index : ui->comboBox_ComSpeed->findData(QSerialPort::Baud9600));

    index = ui->comboBox_ComDataBits->findData(static_cast<int>(sendParam.comParam.dataBits));
    ui->comboBox_ComDataBits->setCurrentIndex(index >= 0 ? index : ui->comboBox_ComDataBits->findData(QSerialPort::Data8));

    index = ui->comboBox_ComParity->findData(static_cast<int>(sendParam.comParam.parity));
    ui->comboBox_ComParity->setCurrentIndex(index >= 0 ? index : ui->comboBox_ComParity->findData(QSerialPort::NoParity));

    index = ui->comboBox_ComStopBit->findData(static_cast<int>(sendParam.comParam.stopBits));
    ui->comboBox_ComStopBit->setCurrentIndex(index >= 0 ? index : ui->comboBox_ComStopBit->findData(QSerialPort::OneStop));

    index = ui->comboBox_ComFlowControl->findData(static_cast<int>(sendParam.comParam.flowControl));
    ui->comboBox_ComFlowControl->setCurrentIndex(index >= 0 ? index : ui->comboBox_ComFlowControl->findData(QSerialPort::NoFlowControl));

    ui->textEdit->clear();
    typeChange(int(sendParam.curSendType));
}

void SendWidget::bind(){
    QVariant _bindParam;
    if(m_curType == TypeSendInterface::ETHERNET){
        QString ip = ui->lineEdit_IP->text();
        quint16 port = ui->lineEdit_Port->text().toInt();
        EthernetParams ether = {ip,port};
        _bindParam = QVariant::fromValue(ether);
    }else{
        ComPortParams com;
        com.comPortName = ui->comboBox_ComPort->currentText();
        com.baudRate = ui->comboBox_ComSpeed->currentData().toInt();
        com.dataBits = static_cast<QSerialPort::DataBits>(ui->comboBox_ComDataBits->currentData().toInt());
        com.parity = static_cast<QSerialPort::Parity>(ui->comboBox_ComParity->currentData().toInt());
        com.stopBits = static_cast<QSerialPort::StopBits>(ui->comboBox_ComStopBit->currentData().toInt());
        com.flowControl = static_cast<QSerialPort::FlowControl>(ui->comboBox_ComFlowControl->currentData().toInt());
        _bindParam = QVariant::fromValue(com);
    }
    emit bindParam(m_curType,_bindParam);
}

void SendWidget::typeChange(int index){
    m_curType = (TypeSendInterface)index;

    ui->stackedWidget->setCurrentIndex(index);

}

void SendWidget::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}

void SendWidget::initComSettings(){
    const QList<qint32> baudRates = {
        QSerialPort::Baud1200,
        QSerialPort::Baud2400,
        QSerialPort::Baud4800,
        QSerialPort::Baud9600,
        QSerialPort::Baud19200,
        QSerialPort::Baud38400,
        QSerialPort::Baud57600,
        QSerialPort::Baud115200
    };
    for (qint32 rate : baudRates) {
        ui->comboBox_ComSpeed->addItem(QString::number(rate), rate);
    }
    ui->comboBox_ComSpeed->setCurrentIndex(ui->comboBox_ComSpeed->findData(QSerialPort::Baud9600)); 

    const QMap<QSerialPort::DataBits, QString> dataBitsMap = {
        {QSerialPort::Data5, "5"},
        {QSerialPort::Data6, "6"},
        {QSerialPort::Data7, "7"},
        {QSerialPort::Data8, "8"}
    };
    for (auto it = dataBitsMap.constBegin(); it != dataBitsMap.constEnd(); ++it) {
        ui->comboBox_ComDataBits->addItem(it.value(), static_cast<int>(it.key()));
    }
    ui->comboBox_ComDataBits->setCurrentIndex(ui->comboBox_ComDataBits->findData(QSerialPort::Data8));

    const QMap<QSerialPort::Parity, QString> parityMap = {
        {QSerialPort::NoParity, "None"},
        {QSerialPort::EvenParity, "Even"},
        {QSerialPort::OddParity, "Odd"},
        {QSerialPort::SpaceParity, "Space"},
        {QSerialPort::MarkParity, "Mark"}
    };
    for (auto it = parityMap.constBegin(); it != parityMap.constEnd(); ++it) {
        ui->comboBox_ComParity->addItem(it.value(), static_cast<int>(it.key()));
    }
    ui->comboBox_ComParity->setCurrentIndex(ui->comboBox_ComParity->findData(QSerialPort::NoParity));

    const QMap<QSerialPort::StopBits, QString> stopBitsMap = {
        {QSerialPort::OneStop, "1"},
        {QSerialPort::OneAndHalfStop, "1.5"},
        {QSerialPort::TwoStop, "2"}
    };
    for (auto it = stopBitsMap.constBegin(); it != stopBitsMap.constEnd(); ++it) {
        ui->comboBox_ComStopBit->addItem(it.value(), static_cast<int>(it.key()));
    }
    ui->comboBox_ComStopBit->setCurrentIndex(ui->comboBox_ComStopBit->findData(QSerialPort::OneStop));

    const QMap<QSerialPort::FlowControl, QString> flowControlMap = {
        {QSerialPort::NoFlowControl, "None"},
        {QSerialPort::HardwareControl, "Hardware"},
        {QSerialPort::SoftwareControl, "Software"}
    };
    for (auto it = flowControlMap.constBegin(); it != flowControlMap.constEnd(); ++it) {
        ui->comboBox_ComFlowControl->addItem(it.value(), static_cast<int>(it.key()));
    }
    ui->comboBox_ComFlowControl->setCurrentIndex(ui->comboBox_ComFlowControl->findData(QSerialPort::NoFlowControl));
}

void SendWidget::updateComPorts(const QStringList& ports){
    QString currentPort = ui->comboBox_ComPort->currentText();
    ui->comboBox_ComPort->clear();
    for (const QString& port : ports) {
        ui->comboBox_ComPort->addItem(port);
    }

    int index =  ui->comboBox_ComPort->findText(currentPort);
    if (index != -1) {
         ui->comboBox_ComPort->setCurrentIndex(index);
    }
}