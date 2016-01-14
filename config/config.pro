include ("..\szabi.pri")

TEMPLATE = lib
CONFIG += static c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    config.cpp

Debug:DESTDIR = ..\Debug
Release:DESTDIR = ..\Release
