#pragma once

#include <QWidget>
#include <QtPlugin>

#ifdef BASENAVIWIDGET_LIBRARY
#define BASENAVIWIDGET_EXPORT Q_DECL_EXPORT
#else
#define BASENAVIWIDGET_EXPORT Q_DECL_IMPORT
#endif

class BASENAVIWIDGET_EXPORT BaseNaviWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseNaviWidget(QWidget *parent = nullptr) : QWidget(parent){

    }
    ~BaseNaviWidget(){

    }
public slots:
    virtual void setText() = 0;

private:

};

// Объявляем интерфейс для плагинов
Q_DECLARE_INTERFACE(BaseNaviWidget, "com.example.BaseNaviWidget/1.0")
