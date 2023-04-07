#-------------------------------------------------
#
# Project created by QtCreator 2020-03-21T14:44:03
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fitts
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

CONFIG += c++11

SOURCES += \
        main.cpp \
    view/fittsview.cpp \
    controller/fittscontroller.cpp \
    model/fittsmodel.cpp \
    view/graphicwidget.cpp

HEADERS += \
    view/fittsview.h \
    controller/fittscontroller.h \
    model/fittsmodel.h \
    view/graphicwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH  += $$PWD/view/
INCLUDEPATH  += $$PWD/controller/
INCLUDEPATH  += $$PWD/model/

RESOURCES += \
    ressources.qrc

DISTFILES += \
    style.qss \
    style_old.qss