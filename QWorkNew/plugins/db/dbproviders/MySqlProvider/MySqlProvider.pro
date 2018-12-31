include (../../../../../common.prf)

QT           += sql xml
TEMPLATE      = lib
CONFIG       += plugin

INCLUDEPATH += ../../../../../libs/settings
INCLUDEPATH += ../../../../../libs/sqlengine
INCLUDEPATH += ../../../../../libs/corelib
INCLUDEPATH += ../../../../../libs/utils
INCLUDEPATH += ../../../../../libs/texteditorlib
INCLUDEPATH += ../../../../../libs/texteditorlib/codeassist

TARGET = MySqlProvider

win32:DEFINES += _WINDOWS

DESTDIR = ../../../$${TARGET_DIR}/dbproviders
LIBS += -L../../../$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsqlengine

FORMS += \
    mysqlauthwidget.ui

HEADERS += \
    mysqlauthwidget.h \
    mysqlprovider.h

SOURCES += \
    mysqlauthwidget.cpp \
    mysqlprovider.cpp

