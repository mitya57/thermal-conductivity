CONFIG += debug warn_all

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += drawarea.cpp scheme.cpp main.cpp
HEADERS += drawarea.h   struct.h
