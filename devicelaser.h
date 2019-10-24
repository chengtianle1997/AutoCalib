#ifndef DEVICELASER_H
#define DEVICELASER_H

#include <QWidget>

namespace Ui {
class DeviceLaser;
}

class DeviceLaser : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceLaser(QWidget *parent = nullptr);
    ~DeviceLaser();

private:
    Ui::DeviceLaser *ui;
};

#endif // DEVICELASER_H
