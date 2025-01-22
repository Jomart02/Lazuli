#pragma once 

#include "BaseNaviWidget.h"  

class TestPlugin : public BaseNaviWidget {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    explicit TestPlugin(QWidget *parent = nullptr);
    virtual QString getText() override;
};
 
 

