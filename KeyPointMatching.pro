TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


QMAKE_CXXFLAGS += -std=c++14 -pthread
QMAKE_LINK += -pthread

CONFIG += link_pkgconfig
PKGCONFIG += opencv


SOURCES += main.cpp

#for opencv
INCLUDEPATH+= /usr/local/include/opencv2
#INCLUDEPATH+= /media/nhquan/Data/VP9/opencv/opencv/include
#LIBS += -L/media/nhquan/Data/VP9/opencv/opencv/build/lib
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio -lopencv_video
