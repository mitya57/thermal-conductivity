CONFIG += debug warn_all

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += drawarea.cpp window.cpp scheme.cpp msr.cpp       main.cpp
HEADERS += drawarea.h   window.h   struct.h   conditions1.h conditions2.h
