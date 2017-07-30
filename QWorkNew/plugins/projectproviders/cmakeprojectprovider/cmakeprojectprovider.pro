include (../../../../common.prf)

QT           += xml network
TEMPLATE      = lib
CONFIG       += plugin

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs
INCLUDEPATH += ../../../../libs/utils
INCLUDEPATH += ../../../../libs/find
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/corelib/CodeModel
INCLUDEPATH += ../../../../libs/ProjectExplorer
INCLUDEPATH += ../../../../libs/ProjectExplorer/options
INCLUDEPATH += ../../../../libs/ProjectExplorer/devicesupport
INCLUDEPATH += ../../../../libs/ProjectExplorer/qtsupport
INCLUDEPATH += ../../../../libs/texteditorlib
INCLUDEPATH += ../../../../libs/texteditorlib/codeassist
INCLUDEPATH += ../../../../libs/cplusplus
INCLUDEPATH += ../../../../libs/cplusplus/shared
INCLUDEPATH += ../../../../libs/cplusplus/cpptools

TARGET = CMakeProjectProvider

win32:DEFINES += _WINDOWS

DESTDIR = ../../$${TARGET_DIR}/projectproviders
LIBS += -L../../$${TARGET_DIR}

LIBS += -lqtcutils
LIBS += -lcore
LIBS += -lsettings
LIBS += -ltexteditor
LIBS += -lcplusplus
LIBS += -lProjectExplorer

HEADERS += \
    cmakeprojectconstants.h \
    cmakeproject.h \
    cmakeopenprojectwizard.h \
    cmakerunconfiguration.h \
    cmakebuildinfo.h \
    generatorinfo.h \
    cmakebuildconfiguration.h \
    cmakeprojectnodes.h \
    cmakeprojectmanager.h \
    makestep.h \
    cmakeparser.h \
    cmakesettingspage.h \
    cmaketool.h

SOURCES += \
    cmakeproject.cpp \
    cmakeopenprojectwizard.cpp \
    cmakerunconfiguration.cpp \
    generatorinfo.cpp \
    cmakebuildconfiguration.cpp \
    cmakeprojectnodes.cpp \
    cmakeprojectmanager.cpp \
    makestep.cpp \
    cmakeparser.cpp \
    cmakesettingspage.cpp \
    cmaketool.cpp

RESOURCES += \
    cmakeprojectprovider.qrc
