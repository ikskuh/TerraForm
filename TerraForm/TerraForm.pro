#-------------------------------------------------
#
# Project created by QtCreator 2019-06-28T10:44:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TerraForm
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(/home/felix/projects/qt-solutions/qtpropertybrowser/src/qtpropertybrowser.pri)

CONFIG += c++17

SOURCES += \
        dragdrop.cpp \
        featureregistry.cpp \
        features/clampfeature.cpp \
        features/polynomialfeature.cpp \
        features/sinewavefeature.cpp \
        main.cpp \
        mainwindow.cpp \
        objectcontroller.cpp \
        terrain.cpp \
        terraindesigner.cpp \
        terrainfeature.cpp \
        terrainfeaturegroup.cpp \
        terrainpreview.cpp \
        terrainrenderer.cpp \
        toollistwidget.cpp

HEADERS += \
        dragdrop.hpp \
        featureregistry.hpp \
        features/clampfeature.hpp \
        features/polynomialfeature.hpp \
        features/sinewavefeature.hpp \
        mainwindow.hpp \
        objectcontroller.hpp \
        terrain.hpp \
        terraindesigner.hpp \
        terrainfeature.hpp \
        terrainfeaturegroup.hpp \
        terrainpreview.hpp \
        terrainrenderer.hpp \
        toollistwidget.hpp

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  icons.qrc
