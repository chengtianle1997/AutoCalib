#include "rotatecontrol.h"
#include "ui_rotatecontrol.h"

RotateControl::RotateControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RotateControl)
{
    ui->setupUi(this);
}

RotateControl::~RotateControl()
{
    delete ui;
}
