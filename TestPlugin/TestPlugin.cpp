#include "TestPlugin.h"
#include <QDebug>

TestPlugin::TestPlugin(QWidget *parent) : BaseNaviWidget(parent), ui(new Ui::TestPlugin) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this,&TestPlugin::setText);
}

TestPlugin::~TestPlugin(){
    delete ui;
}

void TestPlugin::setText(){
    ui->textEdit->clear();
    ui->textEdit->setText("Plugin test");
}
