include (../../../common.prf)

QT += sql xml
TEMPLATE = lib
CONFIG += plugin

DEFINES += ASPROJECT_LIBRARY

INCLUDEPATH += ../../../libs
INCLUDEPATH += ../../../libs/settings
INCLUDEPATH += ../../../libs/corelib
INCLUDEPATH += ../../../libs/find
INCLUDEPATH += ../../../libs/utils

TARGET = asproject

DESTDIR = ../../../../../$${BUILDDIR}/QWorkNew/$${QTVERDIR}/$${BINDIR}/plugins
LIBS += -L../../../../../$${BUILDDIR}/QWorkNew/$${QTVERDIR}/$${BINDIR}

LIBS += -lsettings
LIBS += -lcore
LIBS += -lfind
LIBS += -lqtcutils

HEADERS += AsofProjectPlugin.h \
    ProjectData.h \
    NewProjectDialog.h \
    ProjectTreeView.h \
    NewFolderDialog.h \
    ProjectBuildActionsModel.h \
    ConfigureProjectBuildActionsDialog.h \
    AddEditProjectBuildActionDialog.h \
    ProjectBuildActionsManager.h \
    ProjectBuildEnvironmentDialog.h \
    ProjectTypeDialog.h \
    IASofProjectImporter.h \
    asproject_global.h \
    GenericProjectImporter.h \
    AsofProjectImportManager.h \
    SelectProjectImporterDialog.h \
    GenericProjectLocationDialog.h \
    AsofProjectPythonModule.h

SOURCES += AsofProjectPlugin.cpp \
    ProjectData.cpp \
    NewProjectDialog.cpp \
    ProjectTreeView.cpp \
    NewFolderDialog.cpp \
    ProjectBuildActionsModel.cpp \
    ConfigureProjectBuildActionsDialog.cpp \
    AddEditProjectBuildActionDialog.cpp \
    ProjectBuildActionsManager.cpp \
    ProjectBuildEnvironmentDialog.cpp \
    ProjectTypeDialog.cpp \
    GenericProjectImporter.cpp \
    AsofProjectImportManager.cpp \
    SelectProjectImporterDialog.cpp \
    GenericProjectLocationDialog.cpp \
    AsofProjectPythonModule.cpp

RESOURCES += \
    asproject.qrc

FORMS += \
    NewProjectDialog.ui \
    NewFolderDialog.ui \
    ConfigureProjectBuildActionsDialog.ui \
    AddEditProjectBuildActionDialog.ui \
    ProjectBuildEnvironmentDialog.ui \
    ProjectTypeDialog.ui \
    SelectProjectImporterDialog.ui \
    GenericProjectLocationDialog.ui


