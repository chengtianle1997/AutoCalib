#include "distancecontrol.h"
#include "ui_distancecontrol.h"

distancecontrol::distancecontrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::distancecontrol)
{
    ui->setupUi(this);
}

distancecontrol::~distancecontrol()
{
    delete ui;
}
