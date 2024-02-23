QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH+=D:\commprsed\opencv\opencv\release\install\include
LIBS +=D:\commprsed\opencv\opencv\release\bin\libopencv_core470.dll
LIBS +=D:\commprsed\opencv\opencv\release\bin\libopencv_highgui470.dll
LIBS +=D:\commprsed\opencv\opencv\release\bin\libopencv_imgcodecs470.dll
LIBS +=D:\commprsed\opencv\opencv\release\bin\libopencv_imgproc470.dll
LIBS +=D:\commprsed\opencv\opencv\release\bin\libopencv_calib3d470.dll
SOURCES += \
    contour.cpp \
    contourpoint.cpp \
    hough.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    contourpoint.h \
    countour.h \
    hough.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc \
    resource2.qrc
