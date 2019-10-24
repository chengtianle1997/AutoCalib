QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CameraSDK.cpp \
    devicelaser.cpp \
    distancecontrol.cpp \
    lasercontrol.cpp \
    main.cpp \
    mainwindow.cpp \
    rotatecontrol.cpp

HEADERS += \
    CameraSDK.h \
    devicelaser.h \
    distancecontrol.h \
    lasercontrol.h \
    mainwindow.h \
    rotatecontrol.h

FORMS += \
    devicelaser.ui \
    distancecontrol.ui \
    lasercontrol.ui \
    mainwindow.ui \
    rotatecontrol.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/MVS/Libraries/win64/ -lMvCameraControl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/MVS/Libraries/win64/ -lMvCameraControl
#else:unix: LIBS += -L$$PWD/MVS/Libraries/win64/ -lMvCameraControl

INCLUDEPATH += $$PWD/MVS/Includes
DEPENDPATH += $$PWD/MVS/Includes

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/OpenCV/ -lopencv_world343
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/OpenCV/ -lopencv_world343d
#else:unix: LIBS += -L$$PWD/OpenCV/ -lopencv_world343

INCLUDEPATH += $$PWD/OpenCV/include
DEPENDPATH += $$PWD/OpenCV/include
