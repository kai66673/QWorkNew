include (../../../../../common.prf)

QT           += sql xml
TEMPLATE      = lib
CONFIG       += plugin

INCLUDEPATH += ../../../../../libs/sqlengine
INCLUDEPATH += ../../../../../libs/settings
INCLUDEPATH += ../../../../../libs/corelib
INCLUDEPATH += ../../../../../libs/utils
INCLUDEPATH += ../../../../../libs/texteditorlib
INCLUDEPATH += ../../../../../libs/texteditorlib/codeassist

TARGET = Db2Provider

win32:DEFINES += _WINDOWS

DESTDIR = ../../../$${TARGET_DIR}/dbproviders
LIBS += -L../../../$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsqlengine

HEADERS += Db2Provider.h \
    Db2AuthWidget.h \
    db2_sql_constants.h

SOURCES += Db2Provider.cpp \
    Db2AuthWidget.cpp

FORMS += \
    Db2AuthWidget.ui
