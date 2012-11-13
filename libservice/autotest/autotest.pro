#-------------------------------------------------
#
# Project created by QtCreator 2009-09-03T13:05:24
#
#-------------------------------------------------
AK_VS_PATH = $${PWD}../../../../
QT       -= gui

TARGET = autotest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG(debug, debug|release):TARGET = ../../../../bin/autotest
else:TARGET = ../../../../bin_stable/autotest

CONFIG(debug, debug|release):EXE_FOLDER = $${AK_VS_PATH}"bin"
else:EXE_FOLDER = $${AK_VS_PATH}"bin_stable"

TEMPLATE = app
LIBS += -L$$EXE_FOLDER \
    -llibservice \
    -llibdb_interface \
    -L$${AK_VS_PATH}@depencies/posix-libraries

SOURCES += main.cpp
INCLUDEPATH += $${AK_VS_PATH}@libraries/libdb_interface
INCLUDEPATH += $${AK_VS_PATH}@libraries/libservice
