#pragma once

#include <QDir>
#include <QSerialPort>

#define DEFAULT_DATA_DIR QDir::toNativeSeparators(QDir::homePath()) + QDir::separator() +"Lazuli" + QDir::separator() 
#define PLAGIN_DATA_DIR DEFAULT_DATA_DIR + "Plugins"
#define CUSTOM_STYLES DEFAULT_DATA_DIR + "Styles"
#define CONFIG DEFAULT_DATA_DIR + "Lazuli.ini"
namespace EnumCode {
    Q_NAMESPACE

    enum class ErrorCodeDir{
        CREATED_DIRECTORY,
        FAILED_TO_CREATE,
        DIRECTORY_ALREADY_EXISTS
    };  
    Q_ENUM_NS(ErrorCodeDir)

};

enum class TypeSendInterface{
    ETHERNET = 0,
    COM_PORT
};  


// Параметры для Ethernet (udp)
struct EthernetParams {
    QString ip;
    quint16 port = 0;
};

// Параметры для COM-порта
struct ComPortParams {
    QString comPortName; // Имя COM-порта 
    qint32 baudRate = QSerialPort::Baud9600; // Скорость (по умолчанию 9600)
    QSerialPort::DataBits dataBits = QSerialPort::Data8; // Биты данных
    QSerialPort::Parity parity = QSerialPort::NoParity; // Четность
    QSerialPort::StopBits stopBits = QSerialPort::OneStop; // Стоп-биты
    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl; // Управление потоком
};


struct SendParam{
    TypeSendInterface curSendType = TypeSendInterface::ETHERNET;
    bool active;
    EthernetParams etherParam;
    ComPortParams comParam;
};