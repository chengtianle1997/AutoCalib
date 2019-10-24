#ifndef LASERCONTROL_H
#define LASERCONTROL_H

#include <QWidget>

namespace Ui {
class lasercontrol;
}

class lasercontrol : public QWidget
{
    Q_OBJECT

public:
    explicit lasercontrol(QWidget *parent = nullptr);
    ~lasercontrol();

private:
    Ui::lasercontrol *ui;
};

#endif // LASERCONTROL_H
