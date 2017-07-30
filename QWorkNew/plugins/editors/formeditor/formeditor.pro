include (../../../../common.prf)

TEMPLATE = lib
TARGET = formeditor
CONFIG += plugin

QT += xml designer designercomponents-private network

INCLUDEPATH += .
INCLUDEPATH += ./designerintegrationv2
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/corelib/CodeModel
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/utils
INCLUDEPATH += ../../../../libs/ProjectExplorer
INCLUDEPATH += ../../../../libs/ProjectExplorer/jsonwizard
INCLUDEPATH += ../../../../libs/ProjectExplorer/devicesupport
INCLUDEPATH += ../../../../libs/ProjectExplorer/options
INCLUDEPATH += ../../../../libs/ProjectExplorer/qtsupport

DESTDIR = ../../$${TARGET_DIR}/editors
LIBS += -L../../$${TARGET_DIR}

LIBS += -lsettings
LIBS += -lcore
LIBS += -lqtcutils
LIBS += -lricheditorlib
LIBS += -lProjectExplorer

HEADERS += \
    FormEditorPlugin.h \
    designerintegrationv2/formresizer.h \
    designerintegrationv2/sizehandlerect.h \
    designerintegrationv2/widgethost.h \
    designerintegrationv2/widgethostconstants.h \
    DesignerIntegration.h \
    UiTypeDialog.h \
    formtemplatewizardpage.h \
    formclasswizard.h \
    formclasswizardparameters.h \
    formclasswizarddialog.h \
    qtdesignerformclasscodegenerator.h \
    formclasswizardpage.h

SOURCES += \
    FormEditorPlugin.cpp \
    designerintegrationv2/formresizer.cpp \
    designerintegrationv2/sizehandlerect.cpp \
    designerintegrationv2/widgethost.cpp \
    DesignerIntegration.cpp \
    UiTypeDialog.cpp \
    formtemplatewizardpage.cpp \
    formclasswizard.cpp \
    formclasswizardparameters.cpp \
    formclasswizarddialog.cpp \
    qtdesignerformclasscodegenerator.cpp \
    formclasswizardpage.cpp

RESOURCES += \
    formeditor.qrc

FORMS += \
    UiTypeDialog.ui \
    formclasswizardpage.ui
