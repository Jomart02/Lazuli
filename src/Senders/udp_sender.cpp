#include "udp_sender.h"

UdpSender::UdpSender(QObject * parent ):Sender(parent){

}


UdpSender::UdpSender(EthernetParams paramEther, QObject * parent)
  : Sender(parent), ha(paramEther.ip), p(paramEther.port) { }

void UdpSender::setAddress(QString &a){
    this->ha.setAddress(a);
}

void UdpSender::setPort(quint16 p){
    this->p = p;
}

void UdpSender::setTarget(QVariant target){
    if(target.canConvert<EthernetParams>()){
        EthernetParams params = target.value<EthernetParams>();
        setAddress(params.ip);
        setPort(params.port);
    }
}

void UdpSender::sendData(const char *buf){
    sendBuf(buf);
}
void UdpSender::setData(QStringList data){
    if(!data.isEmpty()){
        for(auto mes:data){
            sendBuf(mes.toUtf8().data());
        }
        emit dataSent(data);
    }
}


void UdpSender::sendBuf(const char *buf){
    s.writeDatagram(buf,ha,p);
}
