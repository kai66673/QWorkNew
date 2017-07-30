include (../../../../common.prf)

QT           += xml network
TEMPLATE      = lib
CONFIG       += plugin

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/corelib/CodeModel
INCLUDEPATH += ../../../../libs/ProjectExplorer
INCLUDEPATH += ../../../../libs/ProjectExplorer/devicesupport
INCLUDEPATH += ../../../../libs/ProjectExplorer/options
INCLUDEPATH += ../../../../libs/ProjectExplorer/qtsupport
INCLUDEPATH += ../../../../libs
INCLUDEPATH += ../../../../libs/utils
INCLUDEPATH += ../../../../libs/find
INCLUDEPATH += ../../../../libs/texteditorlib
INCLUDEPATH += ../../../../libs/Qt4Concurrent
INCLUDEPATH += ../../../../libs/cplusplus
INCLUDEPATH += ../../../../libs/cplusplus/shared
INCLUDEPATH += ../../../../libs/cplusplus/cpptools

TARGET = QMakeProjectProvider

win32:DEFINES += _WINDOWS
DEFINES += QMAKEPROJECTMANAGER_LIBRARY

DESTDIR = ../../$${TARGET_DIR}/projectproviders
LIBS += -L../../$${TARGET_DIR}

LIBS += -lqtcutils
LIBS += -lcore
LIBS += -ltexteditor
LIBS += -lcplusplus
LIBS += -lProjectExplorer

RESOURCES += \
    qmakeprojectmanager.qrc

HEADERS += \
    qmakeprojectmanager.h \
    qmakeprojectconstants.h \
    qmakeproject.h \
    qmakenodes.h \
    qmakeprojectmanager_global.h \
    qmakestep.h \
    qmakebuildconfiguration.h \
    qmakebuildinfo.h \
    qmakeprojectimporter.h \
    qmakerunconfigurationfactory.h \
    qmakeparser.h \
    qmakekitinformation.h \
    makestep.h \
    qmakekitconfigwidget.h \
    qmakeprojectconfigwidget.h \
    desktopqmakerunconfiguration.h \
    findqmakeprofiles.h \
    wizards/qtwizard.h \
    wizards/qtprojectparameters.h \
    wizards/modulespage.h \
    qtmodulesinfo.h \
    wizards/subdirsprojectwizard.h \
    wizards/subdirsprojectwizarddialog.h \
    wizards/emptyprojectwizard.h \
    wizards/emptyprojectwizarddialog.h \
    wizards/guiappwizard.h \
    wizards/guiappwizarddialog.h \
    wizards/filespage.h \
    wizards/consoleappwizard.h \
    wizards/consoleappwizarddialog.h \
    wizards/librarywizard.h \
    wizards/librarywizarddialog.h \
    wizards/libraryparameters.h

SOURCES += \
    qmakeprojectmanager.cpp \
    qmakeproject.cpp \
    qmakenodes.cpp \
    qmakestep.cpp \
    qmakebuildconfiguration.cpp \
    qmakeprojectimporter.cpp \
    qmakerunconfigurationfactory.cpp \
    qmakeparser.cpp \
    qmakekitinformation.cpp \
    makestep.cpp \
    qmakekitconfigwidget.cpp \
    qmakeprojectconfigwidget.cpp \
    desktopqmakerunconfiguration.cpp \
    findqmakeprofiles.cpp \
    wizards/qtwizard.cpp \
    wizards/qtprojectparameters.cpp \
    wizards/modulespage.cpp \
    qtmodulesinfo.cpp \
    wizards/subdirsprojectwizard.cpp \
    wizards/subdirsprojectwizarddialog.cpp \
    wizards/emptyprojectwizard.cpp \
    wizards/emptyprojectwizarddialog.cpp \
    wizards/guiappwizard.cpp \
    wizards/guiappwizarddialog.cpp \
    wizards/filespage.cpp \
    wizards/consoleappwizard.cpp \
    wizards/consoleappwizarddialog.cpp \
    wizards/librarywizard.cpp \
    wizards/librarywizarddialog.cpp \
    wizards/libraryparameters.cpp

FORMS += \
    qmakestep.ui \
    makestep.ui \
    qmakeprojectconfigwidget.ui

DEFINES *= \
    QMAKE_AS_LIBRARY \
    PROPARSER_THREAD_SAFE \
    PROEVALUATOR_THREAD_SAFE \
    PROEVALUATOR_CUMULATIVE \
    PROEVALUATOR_SETENV
