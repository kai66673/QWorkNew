include (../../../../../common.prf)

QT           += sql xml
TEMPLATE      = lib
CONFIG       += plugin

INCLUDEPATH += ./engine
INCLUDEPATH += ../../../../../libs/sqlengine
INCLUDEPATH += ../../../../../libs/settings
INCLUDEPATH += ../../../../../libs/corelib
INCLUDEPATH += ../../../../../libs/utils
INCLUDEPATH += ../../../../../libs/texteditorlib
INCLUDEPATH += ../../../../../libs/texteditorlib/codeassist

TARGET = OracleProvider

win32:DEFINES += _WINDOWS

DESTDIR = ../../../$${TARGET_DIR}/dbproviders
LIBS += -L../../../$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsqlengine
LIBS += -ltexteditor
LIBS += -lqtcutils

HEADERS += OracleProvider.h \
    OracleAuthWidget.h \
    oracle_sql_constants.h \
    engine/OracleParser.h \
    engine/OracleASTVisitor.h \
    engine/OracleASTfwd.h \
    engine/OracleAST.h \
    engine/OracleKeywords.h \
    engine/OracleSqlCheckNamesVisitor.h \
    engine/OracleSqlStatementsBindVisitor.h \
    engine/OracleSqlCompletionAssistVisitor.h \
    engine/OracleParserEngine.h

SOURCES += OracleProvider.cpp \
    OracleAuthWidget.cpp \
    engine/OracleParser.cpp \
    engine/OracleASTVisitor.cpp \
    engine/OracleASTVisit.cpp \
    engine/OracleASTParse.cpp \
    engine/OracleAST.cpp \
    engine/OracleKeywords.cpp \
    engine/OracleSqlCheckNamesVisitor.cpp \
    engine/OracleSqlStatementsBindVisitor.cpp \
    engine/OracleSqlCompletionAssistVisitor.cpp \
    engine/OracleParserEngine.cpp

FORMS += \
    OracleAuthWidget.ui
