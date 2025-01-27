#include "udp_sender.h"

UdpSender::UdpSender(QObject * parent ):QObject(parent){

}


UdpSender::UdpSender(const QString &address, quint16 port, QObject * parent)
  : QObject(parent), ha(address), p(port) { }

void UdpSender::setAddress(QString &a){
    this->ha.setAddress(a);
}

void UdpSender::setPort(quint16 p){
    this->p = p;
}

void UdpSender::setTarget(QString &a, quint16 p){
    setAddress(a);
    setPort(p);
}

void UdpSender::sendData(const char *buf){
    sendBuf(buf);
}
void UdpSender::setData(QStringList data){
    if(!data.isEmpty()){
        for(auto mes:data){
            sendBuf(mes.toUtf8().data());
        }
    }
}


void UdpSender::sendBuf(const char *buf){
    s.writeDatagram(buf,ha,p);
}
