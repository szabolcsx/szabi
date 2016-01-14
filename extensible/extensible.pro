include ("..\szabi.pri")

TEMPLATE = lib
CONFIG += dll c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    manager.cpp \
    shared_object.cpp \
    version.cpp

DEFINES += \
    _WINDLL

Debug:DESTDIR = ..\Debug
Release:DESTDIR = ..\Release

HEADERS += \
    ../include/szabi/extensible.h \
    ../include/szabi/extensible/extensible_export.h \
    ../include/szabi/extensible/extension.h \
    ../include/szabi/extensible/iextension.h \
    ../include/szabi/extensible/manager.h \
    ../include/szabi/extensible/server.h \
    ../include/szabi/extensible/shared_object.h \
    ../include/szabi/extensible/version.h
