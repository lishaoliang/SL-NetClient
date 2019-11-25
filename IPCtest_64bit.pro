#-------------------------------------------------
#
# Project created by QtCreator 2019-08-02T13:23:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IPCtest_x64
TEMPLATE = app
DEFINES += __L_WIN64__
RC_FILE = resource/icon.rc
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


LIBS += -L$$PWD/lib_x64/ -ll_sdk
LIBS += -L$$PWD/lib_x64/ -lavcodec
LIBS += -L$$PWD/lib_x64/ -lavdevice
LIBS += -L$$PWD/lib_x64/ -lavfilter
LIBS += -L$$PWD/lib_x64/ -lavformat
LIBS += -L$$PWD/lib_x64/ -lavutil
LIBS += -L$$PWD/lib_x64/ -lpostproc
LIBS += -L$$PWD/lib_x64/ -lswresample
LIBS += -L$$PWD/lib_x64/ -lswscale

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS += cn.ts\
        en.ts
#QMAKE_CXXFLAGS += -source-charset:utf-8
INCLUDEPATH += third_lib \
        third_lib/inc \
        inc/proto \
        third_lib/ffmpeg

SOURCES += \
    src/allsettingdialog.cpp \
    src/basicinformationdialog.cpp \
    src/Globalmessagedialog.cpp \
    src/loginwidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mythread.cpp \
    src/netsettingdialog.cpp \
    src/ntpsettingdialog.cpp \
    src/osdsettingdialog.cpp \
    src/painterwidget.cpp \
    src/upgradethread.cpp

HEADERS += \
    src/allsettingdialog.h \
    src/basicinformationdialog.h \
    src/Globalmessagedialog.h \
    src/loginwidget.h \
    src/mainwindow.h \
    src/mythread.h \
    src/netsettingdialog.h \
    src/ntpsettingdialog.h \
    src/osdsettingdialog.h \
    src/painterwidget.h \
    src/upgradethread.h

FORMS += \
    src/allsettingdialog.ui \
    src/basicinformationdialog.ui \
    src/Globalmessagedialog.ui \
    src/loginwidget.ui \
    src/mainwindow.ui \
    src/netsettingdialog.ui \
    src/ntpsettingdialog.ui \
    src/osdsettingdialog.ui

DISTFILES += \
    resource/icon.rc

RESOURCES += \
    resource/resource.qrc


