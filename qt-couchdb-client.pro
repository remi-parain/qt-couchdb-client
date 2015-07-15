#-------------------------------------------------
#
# Project created by QtCreator 2015-07-15T16:55:38
#
#-------------------------------------------------

QT       += core network
TARGET = qt-couchdb-client
TEMPLATE = lib
CONFIG += staticlib c++11

HEADERS += \
    src/qcouchserver.h \
    src/qcouchquery.h \
    src/qcouchcst.h \
    src/qcouchworker.h \
    src/qcouchutils.h \
    src/qcouchresponse.h \
    src/qcouchdocinfo.h \
    src/qcouchchangetracker.h

SOURCES += \
    src/qcouchquery.cpp \
    src/qcouchserver.cpp \
    src/qcouchworker.cpp \
    src/qcouchutils.cpp \
    src/qcouchdocinfo.cpp \
    src/qcouchchangetracker.cpp

DISTFILES += \
    qt-couchdb-client.pri \
    LICENCE \
    README.md
