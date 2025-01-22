#include "TestPlugin.h"
#include <QDebug>

TestPlugin::TestPlugin(QWidget *parent) : BaseNaviWidget(parent) {}


QString TestPlugin::getText(){
    return "Plugin 1";
}