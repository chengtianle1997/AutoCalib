#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPoint>
#include <QImage>
#include <QDebug>
#include <QStandardItemModel>
#include "lasercontrol.h"
#include "rotatecontrol.h"
#include "distancecontrol.h"
#include "devicelaser.h"
#include "CameraSDK.h"
#include "map"

#define MAX_CAMERA_ONCE 50

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Record the camera already opened
    int CameraNum = 0;
    //Camera pointer for device detect
    Camera *camera;
    //Cameras to calib
    Camera hcamera[MAX_CAMERA_ONCE];
    CameraInitParam camerainitparam[MAX_CAMERA_ONCE];
    //CameraList Model
    QStandardItemModel* cameralist = new QStandardItemModel();
    //CameraCond Map  0:Closed  1：Opened
    map<string,int> CameraCond;
    void StartGrabing();
    void DisplayCamera(Camera camera);
    void DisplayPic(Mat image);
    QImage cvMat_to_QImage(const cv::Mat &mat);
    //DisplayExit
    bool DisplayEnable = false;

private slots:
    void OpenRotateControlView();
    void OpenDistanceControlView();
    void OpenLaserControlView();
    void OpenDeviceControlView();
    void RefreshCameraList();
    void OpenCamera();

private:
    Ui::MainWindow *ui;
    RotateControl *rotatectrl = new RotateControl;
    lasercontrol *laserctrl = new lasercontrol;
    distancecontrol *distancectrl = new distancecontrol;
    DeviceLaser *devicectrl = new DeviceLaser;

    //CameraDriver
    int SizeofPixels = 0;
    int ImageWidth = 0;
    int ImageHeight = 0;

    int CameraInit(Camera *camera, CameraInitParam& camerainitparam);
    int CameraClean(Camera *camera);
    int GrabImage(Camera *camera, Mat* matimage);
    void EnumCamera();


};


#endif // MAINWINDOW_H
