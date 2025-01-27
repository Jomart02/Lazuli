#pragma once

#include <QObject>
#include <QString>
#include <QHostAddress>
#include <QUdpSocket>

// Sends UDP packets
class UdpSender : public QObject
{
    Q_OBJECT
public:
    // address - target IP address, brooadcasts supported
    // port - destination port
    // parent - qt parent

    UdpSender(QObject * parent = 0);

    UdpSender(const QString &address, quint16 port,
        QObject * parent = 0);

    /// @brief Send <buf>
    /// @param buf char arr to send
    void sendData(const char *buf);

    void setAddress(QString &address);
    void setPort(quint16 port);
    void setTarget(QString &address, quint16 port);
public slots:
    void setData(QStringList data);
private:

    void sendBuf(const char *buf);
    QHostAddress ha;
    quint16 p;
    QUdpSocket s;
};

