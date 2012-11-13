AK_VS_PATH = $${PWD}../../../
CONFIG += console
CONFIG(debug, debug|release):TARGET = ../../../bin/modules/universal_loader
else:TARGET = ../../../bin_stable/modules/universal_loader
CONFIG(debug, debug|release):EXE_FOLDER = $${AK_VS_PATH}"bin"
else:EXE_FOLDER = $${AK_VS_PATH}"bin_stable"
CONFIG(release, debug|release):DEFINES += NO_DEBUG
TEMPLATE = app

TRANSLATIONS += universal_loader_ru.ts \
                universal_loader_en.ts

# QT        += svg
QT = core \
    sql \
    gui \
    svg
HEADERS += args_proc.h
SOURCES += main.cpp \
    args_proc.cpp
FORMS += 
RESOURCES += 
INCLUDEPATH = $${AK_VS_PATH}@libraries/libdb_interface/include \
    $${AK_VS_PATH}@libraries/libservice/include

LIBS +=  $${EXE_FOLDER}/libdb_interface.dll \
         $${EXE_FOLDER}/libservice.dll


QMAKE_PRE_LINK = lupdate universal_loader.pro & \
                 lrelease universal_loader.pro & \
                 copy /Y universal_loader_ru.qm %CD%\\..\\..\\bin_stable\\translation & \
                 copy /Y universal_loader_en.qm %CD%\\..\\..\\bin_stable\\translation


#contains( CONFIG, FOR_GPROF_BUILD )
#{
  #  QMAKE_CXX += -pg
   # QMAKE_CC += -pg
   # QMAKE_LFLAGS += -pg
#}
