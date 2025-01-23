#pragma once 

#include "BaseNaviWidget.h"  
#include "ui_TestPlugin.h"

namespace Ui { class TestPlugin; }

class TestPlugin : public BaseNaviWidget {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.BaseNaviWidget/1.0")
    Q_INTERFACES(BaseNaviWidget)
public:
    explicit TestPlugin(QWidget *parent = nullptr);
    ~TestPlugin();
public slots:
    virtual void setText() override;
private:
    Ui::TestPlugin *ui;
};
 