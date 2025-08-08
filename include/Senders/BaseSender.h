#pragma once 
#include <QObject>
#include "global_vars.h"

class Sender : public QObject {
     Q_OBJECT
public:
     explicit Sender(QObject* parent = nullptr) : QObject(parent) {}
     virtual ~Sender() = default;
     virtual void setTarget(QVariant target) = 0;
signals:
     void dataSent(QStringList dataSend);
public slots:
     virtual void setData(QStringList data) = 0;
};