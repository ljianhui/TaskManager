#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T14:17:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TaskManager
TEMPLATE = app

INCLUDEPATH += core


SOURCES += main.cpp\
        mainwindow.cpp \
    baseinfoform.cpp \
    processinfoform.cpp \
    resourcesform.cpp \
    filesysteminfoform.cpp \
    logform.cpp \
    core/baseinfo.cpp \
    core/cpu.cpp \
    core/cpuinfo.cpp \
    core/filehelper.cpp \
    core/filesystem.cpp \
    core/filesysteminfo.cpp \
    core/log.cpp \
    core/memoryinfo.cpp \
    core/networkinfo.cpp \
    core/process.cpp \
    core/processinfo.cpp \
    core/processmanager.cpp \
    core/systeminfo.cpp \
    core/utils.cpp \
    prioritysettingdialog.cpp \
    tableviewhelper.cpp \
    publicreadwritelock.cpp \
    logworker.cpp \
    systeminfoprovider.cpp

HEADERS  += mainwindow.h \
    baseinfoform.h \
    processinfoform.h \
    resourcesform.h \
    filesysteminfoform.h \
    logform.h \
    core/baseinfo.h \
    core/cpu.h \
    core/cpuinfo.h \
    core/filehelper.h \
    core/filesystem.h \
    core/filesysteminfo.h \
    core/log.h \
    core/memoryinfo.h \
    core/networkinfo.h \
    core/process.h \
    core/processinfo.h \
    core/processmanager.h \
    core/singleton.h \
    core/systeminfo.h \
    core/utils.h \
    prioritysettingdialog.h \
    tableviewhelper.h \
    publicreadwritelock.h \
    logworker.h \
    systeminfoprovider.h

FORMS    += mainwindow.ui \
    baseinfoform.ui \
    processinfoform.ui \
    resourcesform.ui \
    filesysteminfoform.ui \
    logform.ui \
    prioritysettingdialog.ui

RESOURCES += \
    img.qrc
