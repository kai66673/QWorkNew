include (../../../../common.prf)

TEMPLATE = lib
TARGET = rpmleditor
CONFIG += plugin
QT += xml

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs/sqlengine
INCLUDEPATH += ../../../../libs/richeditorlib
INCLUDEPATH += ../../../../libs/rpmllib
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/utils

DESTDIR = ../../$${TARGET_DIR}/editors
LIBS += -L../../$${TARGET_DIR}

LIBS += -lsettings
LIBS += -lcore
LIBS += -lqtcutils
LIBS += -lricheditorlib
LIBS += -lrpmllib

HEADERS += \
    RpmlEditorPlugin.h

SOURCES += \
    RpmlEditorPlugin.cpp

RESOURCES += \
    rpmleditor.qrc
