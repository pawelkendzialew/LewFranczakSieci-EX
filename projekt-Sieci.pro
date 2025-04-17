QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT += charts
QT += core gui network
SOURCES += \
    ARX.cpp \
    App.cpp \
    BuforDanych.cpp \
    PID.cpp \
    Sygnal.cpp \
    arxwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    networkmanager.cpp \
    test.cpp

HEADERS += \
    ARX.h \
    App.h \
    BuforDanych.h \
    PID.h \
    Sygnal.h \
    arxwindow.h \
    mainwindow.h \
    networkmanager.h \
    test.h

FORMS += \
    arxwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
