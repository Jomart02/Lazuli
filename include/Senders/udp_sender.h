#pragma once
#include <QHostAddress>
#include <QUdpSocket>
#include "BaseSender.h"

// Sends UDP packets
class UdpSender : public Sender
{
    Q_OBJECT
public:
    UdpSender(QObject * parent = 0);
    UdpSender(EthernetParams paramEther,QObject * parent = 0);

    /// @brief Send <buf>
    /// @param buf char arr to send
    void sendData(const char *buf);

    void setAddress(QString &address);
    void setPort(quint16 port);
    virtual void setTarget(QVariant target) override;
public slots:
    virtual void setData(QStringList data) override;
private:

    void sendBuf(const char *buf);
    QHostAddress ha;
    quint16 p;
    QUdpSocket s;
};

