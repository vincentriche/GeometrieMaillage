#-------------------------------------------------
#
# Project created by QtCreator 2013-09-12T03:27:41
#
#-------------------------------------------------

QT       += core gui opengl

LIBS += -L C:\Qt\5.9.1\mingw53_32\lib\libQt5OpenGL.a -lopengl32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sierpinski
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplay.cpp \
	gasket.cpp \
    triangulation.cpp \
    utilities.cpp

HEADERS  += mainwindow.h \
    gldisplay.h \
	gasket.h \
    utilities.h \
    triangulation.h

FORMS    += mainwindow.ui
