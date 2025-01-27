#pragma once


#include <QWidget>
#include "ui_SendWidget.h"
#include <QRegularExpression>
#include <QValidator>

namespace Ui { class SendWidget; }

class SendWidget : public QWidget{
    Q_OBJECT
public:
    SendWidget(QWidget *parent);
    ~SendWidget();
    void setSendParam(bool stateSend, QString ip , quint16 port);
signals:
    void startSend(bool start);
    void bindParam(QString ip , quint16 port);
public slots:
    void setData(QStringList data);
    void bind();
private:
    Ui::SendWidget *ui;
};

