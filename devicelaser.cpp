#include "devicelaser.h"
#include "ui_devicelaser.h"

DeviceLaser::DeviceLaser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceLaser)
{
    ui->setupUi(this);
}

DeviceLaser::~DeviceLaser()
{
    delete ui;
}
