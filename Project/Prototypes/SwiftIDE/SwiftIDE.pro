CONFIG += release

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

TARGET = SwiftIDE
TEMPLATE = app

HEADERS     += mainwindow.hpp \
    qsfmlcanvas.h \
    sceneeditorview.h
SOURCES     += main.cpp\
               mainwindow.cpp \
    qsfmlcanvas.cpp \
    sceneeditorview.cpp
FORMS       += mainwindow.ui
RESOURCES   += Icons.qrc
INCLUDEPATH += $$[QT_INSTALL_HEADERS]
LIBS        += -L$$[QT_INSTALL_LIBS] -lqscintilla2 -lsfml-system -lsfml-window -lsfml-graphics
