#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T09:27:55
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = HttpServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += "./joyent-parser/"


SOURCES += main.cpp \
    joyent-parser/http_parser.c \
    httpserver.cpp \
    httpparser.cpp

HEADERS += \
    joyent-parser/http_parser.h \
    httpserver.h \
    httpparser.h
