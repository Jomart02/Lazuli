
#include "SerialSender.h"

SerialSender::SerialSender(const ComPortParams& params, QObject* parent)
     : Sender(parent), serialPort(new QSerialPort(this)), m_params(params) {
     configureSerialPort();
}

SerialSender::~SerialSender() {
     if (serialPort->isOpen()) {
          serialPort->close();
     }
}

void SerialSender::setData(QStringList data) {
     if (!serialPort->isOpen()) {
          if (!serialPort->open(QIODevice::WriteOnly)) {
               QString error ="Failed to open serial port:" + serialPort->errorString() + "\n";
               qWarning() << error;
               emit dataSent(QStringList({error}));
               return;
          }
     }
     for(auto mes:data){
          QByteArray datagram = mes.toUtf8();
          serialPort->write(datagram);
          serialPort->flush();   
     }
     emit dataSent(data);
}

void SerialSender::configureSerialPort() {
     serialPort->setPortName(m_params.comPortName);
     serialPort->setBaudRate(m_params.baudRate);
     serialPort->setDataBits(m_params.dataBits);
     serialPort->setParity(m_params.parity);
     serialPort->setStopBits(m_params.stopBits);
     serialPort->setFlowControl(m_params.flowControl);
}

void SerialSender::setTarget(QVariant target){
     if(target.canConvert<ComPortParams>()){
          m_params = target.value<ComPortParams>();
          configureSerialPort();
     }
}
