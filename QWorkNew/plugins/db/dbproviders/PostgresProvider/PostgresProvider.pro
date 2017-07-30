include (../../../../../common.prf)

QT           += sql xml
TEMPLATE      = lib
CONFIG       += plugin

INCLUDEPATH += .
INCLUDEPATH += ../../../../../libs/settings
INCLUDEPATH += ../../../../../libs/sqlengine
INCLUDEPATH += ../../../../../libs/corelib
INCLUDEPATH += ../../../../../libs/utils
INCLUDEPATH += ../../../../../libs/texteditorlib
INCLUDEPATH += ../../../../../libs/texteditorlib/codeassist

TARGET = PostgresProvider

DESTDIR = ../../../$${TARGET_DIR}/dbproviders
LIBS += -L../../../$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsqlengine
LIBS += -ltexteditor
LIBS += -lqtcutils

HEADERS += \
    PostgresProvider.h \
    PostgresAuthWidget.h \
    postgres_sql_constants.h \
    engine/PostgresParserEngine.h \
    engine/PostgresParser.tab.hh \
    engine/stack.hh \
    engine/PostgresDriver.h \
    engine/PostgresSemanticParser.h \
    engine/PostgresAST.h \
    engine/PostgresASTfwd.h \
    engine/PostgresASTVisitor.h \
    engine/PostgresSqlStatementsBindVisitor.h \
    engine/PostgresSqlCheckNamesVisitor.h \
    engine/PostgresSqlCompletionAssistVisitor.h \
    engine/PostgresASTNoVisitor.h \
    engine/PostgresSqlLinkFinder.h

SOURCES += \
    PostgresProvider.cpp \
    PostgresAuthWidget.cpp \
    engine/PostgresParserEngine.cpp \
    engine/PostgresParser.tab.cc \
    engine/PostgresDriver.cpp \
    engine/PostgresScanner.cpp \
    engine/PostgresSemanticParser.cpp \
    engine/PostgresAST.cpp \
    engine/PostgresASTVisitor.cpp \
    engine/PostgresSqlStatementsBindVisitor.cpp \
    engine/PostgresSqlCheckNamesVisitor.cpp \
    engine/PostgresSqlCompletionAssistVisitor.cpp \
    engine/PostgresSqlLinkFinder.cpp

FORMS += \
    PostgresAuthWidget.ui

OTHER_FILES += \
    engine/PostgresParser.yy \
    engine/Origanal_Postgres_gram.y

RESOURCES += \
    PostgresProvider.qrc
