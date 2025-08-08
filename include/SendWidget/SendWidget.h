#pragma once


#include <QWidget>
#include <QRegularExpression>
#include <QValidator>
#include "global_vars.h"
#include <QThread>
#include <QSerialPortInfo>

namespace Ui { class SendWidget; }


class ComPortScanner : public QThread {
    Q_OBJECT

public:
    explicit ComPortScanner(QObject* parent = nullptr) : QThread(parent), running(false) {
        interval = 2000;
    }

    ~ComPortScanner() {
        stop();
        wait();
    }

    void setInterval(int msec) {
        interval = msec;
    }

    void stop() {
        running = false;
    }

protected:
    void run() override {
        running = true;
        QStringList previousPorts;

        while (running) {
            QStringList currentPorts;
            for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts()) {
                currentPorts << info.portName();
            }

            if (currentPorts != previousPorts) {
                emit portsUpdated(currentPorts);
                previousPorts = currentPorts; 
            }

            QThread::msleep(interval);
        }
    }

signals:
    void portsUpdated(const QStringList& ports);

private:
    bool running;
    int interval;
};

class SendWidget : public QWidget{
    Q_OBJECT
public:
    SendWidget(QWidget *parent);
    ~SendWidget();



    void setSendParam(SendParam &sendParam);
signals:
    void startSend(bool start);
    void bindParam(TypeSendInterface typeBind , QVariant _bindParam);
public slots:
    void setData(QStringList data);
    void bind();
private slots:
    void typeChange(int index);
    void updateComPorts(const QStringList& ports);
protected:
    void changeEvent(QEvent *event) override;
private:
    void initComSettings();
private:
    Ui::SendWidget *ui;
    TypeSendInterface m_curType = TypeSendInterface::ETHERNET; 
    ComPortScanner* comPortScanner;
};

