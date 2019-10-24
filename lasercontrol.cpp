#include "lasercontrol.h"
#include "ui_lasercontrol.h"

lasercontrol::lasercontrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lasercontrol)
{
    ui->setupUi(this);
}

lasercontrol::~lasercontrol()
{
    delete ui;
}
