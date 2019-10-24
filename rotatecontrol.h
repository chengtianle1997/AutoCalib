#ifndef ROTATECONTROL_H
#define ROTATECONTROL_H

#include <QWidget>

namespace Ui {
class RotateControl;
}

class RotateControl : public QWidget
{
    Q_OBJECT

public:
    explicit RotateControl(QWidget *parent = nullptr);
    ~RotateControl();

private:
    Ui::RotateControl *ui;
};

#endif // ROTATECONTROL_H
