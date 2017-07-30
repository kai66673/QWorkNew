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

TARGET = LinterProvider

DESTDIR = ../../../$${TARGET_DIR}/dbproviders
LIBS += -L../../../$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsqlengine

HEADERS += \
    LinterProvider.h \
    LinterAuthWidget.h

SOURCES += \
    LinterProvider.cpp \
    LinterAuthWidget.cpp

FORMS += \
    LinterAuthWidget.ui
