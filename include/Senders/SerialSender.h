#pragma once

#include "BaseSender.h"
#include <QSerialPort>
#include "global_vars.h"

class SerialSender : public Sender {
     Q_OBJECT
public:
     SerialSender(const ComPortParams& params, QObject* parent = nullptr);
     ~SerialSender();
public slots:
     void setData(QStringList data) override;
     virtual void setTarget(QVariant target) override;

private:
     void configureSerialPort();

     QSerialPort* serialPort;
     ComPortParams m_params;
};