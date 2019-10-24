#ifndef DISTANCECONTROL_H
#define DISTANCECONTROL_H

#include <QWidget>

namespace Ui {
class distancecontrol;
}

class distancecontrol : public QWidget
{
    Q_OBJECT

public:
    explicit distancecontrol(QWidget *parent = nullptr);
    ~distancecontrol();

private:
    Ui::distancecontrol *ui;
};

#endif // DISTANCECONTROL_H
