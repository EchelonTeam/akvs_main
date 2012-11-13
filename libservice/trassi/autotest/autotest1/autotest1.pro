#-------------------------------------------------
#
# Project created by QtCreator 2009-09-09T16:28:38
#
#-------------------------------------------------





AK_VS_PATH = $${PWD}/../../../

CONFIG(debug, debug|release):EXE_FOLDER = $${AK_VS_PATH}"bin"
else:EXE_FOLDER = $${AK_VS_PATH}"bin_stable"
CONFIG(release, debug|release):DEFINES += NO_DEBUG

DESTDIR = $$EXE_FOLDER

QT -= gui \
        xml


message($${AK_VS_PATH})
TARGET = autotest2
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
LIBS += -L$$EXE_FOLDER \
    -llibdb_interface \
    -llibservice
    SOURCES += main.cpp

INCLUDEPATH = $${AK_VS_PATH}@libraries/libdb_interface
INCLUDEPATH += $${AK_VS_PATH}@libraries/libservice
