include (../../../../../common.prf)

QT           += xml sql
TEMPLATE      = lib
CONFIG       += plugin

INCLUDEPATH += ../../../../../libs/settings
INCLUDEPATH += ../../../../../libs/sqlengine
INCLUDEPATH += ../../../../../libs/corelib
INCLUDEPATH += ../../../../../libs/utils
INCLUDEPATH += ../../../../../libs/texteditorlib
INCLUDEPATH += ../../../../../libs/texteditorlib/codeassist

TARGET = CassandraProvider

win32:DEFINES += _WINDOWS

DESTDIR = ../../../$${TARGET_DIR}/dbproviders
LIBS += -L../../../$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsqlengine

FORMS += \
    CassandraAuthWidget.ui

HEADERS += \
    CassandraAuthWidget.h

SOURCES += \
    CassandraAuthWidget.cpp
