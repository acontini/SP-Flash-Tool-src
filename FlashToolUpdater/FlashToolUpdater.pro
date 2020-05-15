#-------------------------------------------------
#
# Project created by QtCreator 2013-03-29T09:38:48
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FlashToolUpdater
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    quazip/unzip.c \
    quazip/quazipnewinfo.cpp \
    quazip/quazipfile.cpp \
    quazip/quazipdir.cpp \
    quazip/quazip.cpp \
    quazip/quaziodevice.cpp \
    quazip/quagzipfile.cpp \
    quazip/quacrc32.cpp \
    quazip/quaadler32.cpp \
    quazip/qioapi.cpp \
    quazip/JlCompress.cpp \
    quazip/zip.c \
    Update.cpp \
    FileOps.cpp \
    CommandLineArguments.cpp

HEADERS += \
    quazip/zlib.h \
    quazip/zip.h \
    quazip/zconf.h \
    quazip/unzip.h \
    quazip/quazipnewinfo.h \
    quazip/quazipfileinfo.h \
    quazip/quazipfile.h \
    quazip/quazipdir.h \
    quazip/quazip_global.h \
    quazip/quazip.h \
    quazip/quaziodevice.h \
    quazip/quagzipfile.h \
    quazip/quacrc32.h \
    quazip/quachecksum32.h \
    quazip/quaadler32.h \
    quazip/JlCompress.h \
    quazip/ioapi.h \
    quazip/crypt.h \
    Update.h \
    FileOps.h \
    CommandLineArguments.h
