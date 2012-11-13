AK_VS_PATH = $${PWD}../../../
ECHELON_PATH = $${AK_VS_PATH}/../



CONFIG(debug, debug|release):   EXE_FOLDER = $${AK_VS_PATH}"bin"
else:                           EXE_FOLDER = $${AK_VS_PATH}"bin_stable"

DESTDIR = $$EXE_FOLDER
TARGET= libservice

TRANSLATIONS += translation/libservice_ru.ts \
                translation/libservice_en.ts


QMAKE_PRE_LINK = lupdate libservice.pro & \
                 lrelease libservice.pro & \
                 copy /Y translation\\libservice_ru.qm %CD%\\..\\..\\bin_stable\\translation & \
                 copy /Y translation\\libservice_en.qm %CD%\\..\\..\\bin_stable\\translation

TEMPLATE = lib

QT +=       core \
            network \
            gui \
            svg

#DEFINES += DEBUG_ON

HEADERS += \
    include/htmlfunction.h \
    include/block_shem_report.h \
    include/matrix_report.h \
    include/libservice.h \
    include/common_libservice.h \
    include/ioltmanager.h \
    include/oltmanager.h \
    include/olt_xmlmanager.h \
    include/servicespace.h \
    include/oltdebugmanager.h \
    include/groupfiles.h \
    include/metrix.h

SOURCES += \
    src/htmlfunction.cpp \
    src/block_shem_report.cpp \
    src/matrix_report.cpp \
    src/libservice.cpp \
    src/graf.cpp \
    src/dinamik.cpp \
    src/trassi.cpp \
    src/infoandfuncobj.cpp \
    src/signature.cpp \
    src/ioltmanager.cpp \
    src/oltmanager.cpp \
    src/olt_xmlmanager.cpp \
    src/oltmanager_links.cpp \
    src/oltmanager_files.cpp \
    src/servicespace.cpp \
    src/oltdebugmanager.cpp \
    src/groupfiles.cpp \
    src/metrix.cpp

LIBS += -L$${EXE_FOLDER} \
    -L$${ECHELON_PATH}@depencies/qgetopt/optionManager/release/ \
    -llibdb_interface

INCLUDEPATH += \
    $${AK_VS_PATH}@libraries/libdb_interface/include \
    $${AK_VS_PATH}@modules/optionManager/include \
    ./include/


contains( CONFIG, FOR_GPROF_BUILD ){
    QMAKE_CXX += -pg
    QMAKE_CC += -pg
    QMAKE_LFLAGS += -pg
}
