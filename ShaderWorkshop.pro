#-------------------------------------------------
#
# Project created by QtCreator 2019-01-22T20:53:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
QT += widgets
CONFIG += c++11
} else {
QMAKE_CXXFLAGS += -std=c++11
}

TARGET = ShaderWorkshop
TEMPLATE = app


SOURCES += main.cpp\
        shaderworkshop.cpp \
    renderer.cpp \
    effect.cpp \
    editorpage.cpp

HEADERS  += shaderworkshop.h \
    renderer.h \
    effect.h \
    editorpage.h

FORMS    += shaderworkshop.ui \
    editorpage.ui
