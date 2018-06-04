#-------------------------------------------------
#
# Project created by QtCreator 2018-06-01T15:42:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HHT-VCTL
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    getusbdiskwidget.cpp

HEADERS += \
        mainwindow.h \
    getusbdiskwidget.h

FORMS += \
        mainwindow.ui \
    getusbdiskwidget.ui


RC_FILE += app.rc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/zlib1.2.8/lib/ -lzdll  -L$$PWD/3rdparty/quazip/lib -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/zlib1.2.8/lib/ -lzdll

message($$QT_VERSION)

INCLUDEPATH += $$PWD/3rdparty/quazip/include
DEPENDPATH += $$PWD/3rdparty/quazip/include

