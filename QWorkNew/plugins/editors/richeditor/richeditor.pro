include (../../../../common.prf)

TEMPLATE = lib
TARGET = richeditor
CONFIG += plugin

QT += xml

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs/richeditorlib
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/utils

DESTDIR = ../../$${TARGET_DIR}/editors
LIBS += -L../../$${TARGET_DIR}

DEFINES += QTEXT_NOPATCHED

LIBS += -lsettings
LIBS += -lcore
LIBS += -lqtcutils
LIBS += -lricheditorlib

HEADERS += \
    RichEditorPlugin.h

SOURCES += \
    RichEditorPlugin.cpp

RESOURCES += \
    richeditor.qrc
