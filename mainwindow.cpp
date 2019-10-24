#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Button slot
    connect(ui->rotatecontrol, SIGNAL(clicked()),this,SLOT(OpenRotateControlView()));
    connect(ui->distancecontrol,SIGNAL(clicked()),this,SLOT(OpenDistanceControlView()));
    connect(ui->lasercontrol,SIGNAL(clicked()),this,SLOT(OpenLaserControlView()));
    connect(ui->devicecontrol,SIGNAL(clicked()),this,SLOT(OpenDeviceControlView()));
    connect(ui->refreshcamera,SIGNAL(clicked()),this, SLOT(RefreshCameraList()));
    connect(ui->opencamera,SIGNAL(clicked()),this,SLOT(OpenCamera()));
    //tableview init

}

//View Change
void MainWindow::OpenRotateControlView()
{
    rotatectrl->show();
}
void MainWindow::OpenDistanceControlView()
{
    distancectrl->show();
}
void MainWindow::OpenLaserControlView()
{
    laserctrl->show();
}
void MainWindow::OpenDeviceControlView()
{
    devicectrl->show();
}

//Refresh Camera
void MainWindow::EnumCamera()
{
    bool nRet;
    MV_CC_DEVICE_INFO_LIST stDevList;
    memset(&stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &stDevList);
    cameralist->clear();
    if (MV_OK != nRet)
    {
        printf("Enum Devices fail! nRet [0x%x]\n", nRet);
        return;
    }

    if (stDevList.nDeviceNum > 0)
    {
        for (unsigned int i = 0; i < stDevList.nDeviceNum; i++)
        {

            MV_CC_DEVICE_INFO pstMVDevInfo = *stDevList.pDeviceInfo[i];
            //cameralist->setItem(i,0,new QStandardItem(QString::number(i)));
            cameralist->setItem(i,0,new QStandardItem(QString::fromUtf8(reinterpret_cast<char *>(pstMVDevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber))));
            map<string,int>::iterator iter;
            //检查状态列表内的状态
            iter = CameraCond.find(string(reinterpret_cast<char *>(pstMVDevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber)));
            if(iter == CameraCond.end())
            {
                cameralist->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("关闭")));
            }
            else
            {
                if(CameraCond[string(reinterpret_cast<char *>(pstMVDevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber))]==1)
                {
                    cameralist->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("开启")));
                }
                else
                {
                    cameralist->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("关闭")));
                }
            }
        }
        ui->tableView->setModel(cameralist);
    }
    else
    {
        printf("Find No Devices!\n");
        return;
    }
}
void MainWindow::RefreshCameraList()
{
    EnumCamera();
}
//Open Camera
void MainWindow::OpenCamera()
{
    QItemSelectionModel *selectlist = ui->tableView->selectionModel();
    QModelIndexList SelectedCamera = selectlist->selectedRows();
    //bool isfirst = true;
    foreach (QModelIndex index, SelectedCamera) {
      qDebug()<<index.data().toString()<<endl;
      map<string,int>::iterator iter;
      iter = CameraCond.find(index.data().toString().toStdString());
      //If the Camera is closed
      if(iter==CameraCond.end())
      {
          //Init Params
          camerainitparam[CameraNum].Gain = 15.0;
          camerainitparam[CameraNum].ExposureTime = 2000;
          memcpy(camerainitparam[CameraNum].SerialNum, index.data().toString().toStdString().c_str(), sizeof (camerainitparam[CameraNum].SerialNum));
          //Open the Camera
          CameraInit(&hcamera[CameraNum],camerainitparam[CameraNum]);
          //Record the Num
          CameraNum++;
          //Record the Cond
          CameraCond[index.data().toString().toStdString()] = 1;
      }
      else
      {
          //If the Camera is Opened
          if(CameraCond[index.data().toString().toStdString()]==1)
          {
              //Do nothing but warning

          }
          else
          {
              //Init Params
              camerainitparam[CameraNum].Gain = 15.0;
              camerainitparam[CameraNum].ExposureTime = 2000;
              memcpy(camerainitparam[CameraNum].SerialNum, index.data().toString().toStdString().c_str(), sizeof (camerainitparam[CameraNum].SerialNum));
              //Open the Camera
              CameraInit(&hcamera[CameraNum],camerainitparam[CameraNum]);
              //Record the Num
              CameraNum++;
              //Record the Cond
              CameraCond[index.data().toString().toStdString()] = 1;
          }
      }
      cameralist->setItem(index.row(),1,new QStandardItem(QString::fromLocal8Bit("打开")));
    }

}
//StartGrabing
void MainWindow::StartGrabing()
{


}
//Display Camera
void MainWindow::DisplayCamera(Camera camera)
{
    while(DisplayEnable)
    {
        Mat matimage;
        int ret = GrabImage(&camera,&matimage);
        if(ret)
        {
            qDebug()<<"Grab Image Error!\n";
        }
        else
        {
            DisplayPic(matimage);
        }

    }
}
//Display Picture
void MainWindow::DisplayPic(Mat image)
{
    QGraphicsScene* MainScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(MainScene);
    QPixmap pixmap = QPixmap::fromImage(cvMat_to_QImage(image));
    MainScene->addPixmap(pixmap);
    ui->tableView->show();
}
//Convert Mat to QImage
QImage MainWindow::cvMat_to_QImage(const cv::Mat &mat )
{
  switch ( mat.type() )
  {
     // 8-bit, 4 channel
     case CV_8UC4:
     {
        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32 );
        return image;
     }
     // 8-bit, 3 channel
     case CV_8UC3:
     {
        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 );
        return image.rgbSwapped();
     }
     // 8-bit, 1 channel
     case CV_8UC1:
     {
        static QVector<QRgb>  sColorTable;
        // only create our color table once
        if ( sColorTable.isEmpty() )
        {
           for ( int i = 0; i < 256; ++i )
              sColorTable.push_back( qRgb( i, i, i ) );
        }
        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8 );
        image.setColorTable( sColorTable );
        return image;
     }
     default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        break;
  }
  return QImage();
}

//Camera Driver
int MainWindow::CameraInit(Camera *camera, CameraInitParam& camerainitparam)
{
    int ret;

    ret = camera->GetDevList(camerainitparam);
    if (ret)
    {
        return ret;
    }


    //printf("Please choose a camera:");
    //scanf("%d", &camerainitparam.devNum);
    ret = camera->OpenCamera(camerainitparam);
    if (ret)
    {
        return ret;
    }

    if (!camerainitparam.ROIWidth)
    {
        camerainitparam.ROIWidth = camerainitparam.in_w;
    }
    if (!camerainitparam.ROIHeight)
    {
        camerainitparam.ROIHeight = camerainitparam.in_h;
    }

    //Get Device info
    camera->GetDevInfo(camerainitparam.devNum, camerainitparam.DevInfo);
    if (ret)
    {
        printf("Get DeviceInfo Failed");
    }

    //Set prop
    //Set Exposureauto
    ret = camera->SetExposureAuto(camerainitparam.ExposureAuto);
    if (ret)
    {
        return ret;
    }
    if (!camerainitparam.ExposureAuto)
    {
        //Set ExposureTime
        ret = camera->SetExposureTime(camerainitparam.ExposureTime);
        if (ret)
        {
            return ret;
        }
    }


    //Set AcquisitionFrameRate
    ret = camera->SetAcquisitionFrameRate(camerainitparam.AcquisitionFrameRate);
    if (ret)
    {
        return ret;
    }


    //Set GainAuto[GainSelector]
    ret = camera->SetGainAuto(camerainitparam.GainAuto);
    if (ret)
    {
        return ret;
    }
    if (!camerainitparam.GainAuto)
    {
        //Set Gain
        ret = camera->SetGain(camerainitparam.Gain);
        if (ret)
        {
            return ret;
        }
    }

    //omit the useless params
    /*
    //Set Width
    ret = camera.SetWidth(camerainitparam.ROIWidth);
    if (ret)
    {
        return ret;
    }
    //Set Height
    ret = camera.SetHeight(camerainitparam.ROIHeight);
    if (ret)
    {
        return ret;
    }
    //Set OffsetX
    ret = camera.SetOffsetX(camerainitparam.ROIOffsetX);
    if (ret)
    {
        return ret;
    }
    //Set OffsetY
    ret = camera.SetOffsetY(camerainitparam.ROIOffsetY);
    if (ret)
    {
        return ret;
    }
    //Set FrameSpecInfoSelector
    ret = camera.SetFrameSpecInfoSelector(camerainitparam.FrameSpecInfoSelector);
    if (ret)
    {
        return ret;
    }
    //Set FrameSpecInfo
    ret = camera.SetFrameSpecInfo(camerainitparam.FrameSpecInfo);
    if (ret)
    {
        return ret;
    }
    //Set TriggerSelector
    ret = camera.SetTriggerSelector(camerainitparam.TriggerSelector);
    if (ret)
    {
        return ret;
    }
    //Set TriggerMode
    ret = camera.SetTriggerMode(camerainitparam.TriggerMode);
    if (ret)
    {
        return ret;
    }
    //Set TriggerSource
    ret = camera.SetTriggerSource(camerainitparam.TriggerSource);
    if (ret)
    {
        return ret;
    }
    //Set TriggerActivation
    ret = camera.SetTriggerActivation(camerainitparam.TriggerActivation);
    if (ret)
    {
        return ret;
    }
    //Set TriggerDelay
    ret = camera.SetTriggerDelay(camerainitparam.TriggerDelay);
    if (ret)
    {
        return ret;
    }
    //Set TriggerCacheEnable
    ret = camera.SetTriggerCacheEnable(camerainitparam.TriggerCacheEnable);
    if (ret)
    {
        return ret;
    }
    */

    //Start Grabing
    ret = camera->StartGrabbing();
    if (ret)
    {
        return ret;
    }

    ImageWidth = camerainitparam.in_w;
    ImageHeight = camerainitparam.in_h;
    SizeofPixels = camerainitparam.in_w * camerainitparam.in_h;
    return 0;
}

int MainWindow::CameraClean(Camera *camera)
{
    int ret = 0;
    ret = camera->StopGrabbing();
    if (ret)
    {
        return ret;
    }
    ret = camera->CloseDevice();
    if (ret)
    {
        return ret;
    }

    return 1;
}

int MainWindow::GrabImage(Camera *camera, Mat* matimage)
{
    MV_FRAME_OUT stOutFrame;
    memset(&stOutFrame, 0, sizeof(MV_FRAME_OUT));

    int ret = 0;
    ret = camera->GetImage(stOutFrame);
    if (ret == MV_OK)
    {
        Mat matImage{
            cvSize(ImageWidth,ImageHeight),
            CV_8UC1,
            (stOutFrame).pBufAddr
        };
        *matimage = matImage.clone();
    }
    else
    {
        printf("Grab Image Error!\n");
        return -1;
    }
    return 0;
}

MainWindow::~MainWindow()
{
    delete rotatectrl;
    delete laserctrl;
    delete devicectrl;
    delete rotatectrl;
    delete ui;
}

