include (../../../../common.prf)

TEMPLATE = lib
TARGET = rxmleditor
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
    RxmlEditorPlugin.h

SOURCES += \
    RxmlEditorPlugin.cpp

RESOURCES += \
    rxmleditor.qrc
