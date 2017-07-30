include (../../common.prf)

QT += network xml
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}

TEMPLATE = lib
TARGET = richeditorlib

DEFINES += RICHEDITOR_LIBRARY
DEFINES += QTEXT_NOPATCHED

INCLUDEPATH += .
INCLUDEPATH += ../corelib
INCLUDEPATH += ../settings
INCLUDEPATH += ../utils

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsettings
LIBS += -lqtcutils

HEADERS += \
    richeditor_global.h \
    RichEditorWidget.h \
    TablePropertiesDialog.h \
    CellsPropertiesDialog.h \
    richeditor_constants.h

SOURCES += \
    RichEditorWidget.cpp \
    TablePropertiesDialog.cpp \
    CellsPropertiesDialog.cpp

RESOURCES += \
    richeditorlib.qrc

FORMS += \
    TablePropertiesDialog.ui \
    CellsPropertiesDialog.ui
