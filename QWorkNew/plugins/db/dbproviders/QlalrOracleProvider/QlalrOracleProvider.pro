include (../../../../../common.prf)

OTHER_FILES += engine/t.g

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

TARGET = QLalrOracleProvider

win32:DEFINES += _WINDOWS

DESTDIR = ../../../$${TARGET_DIR}/dbproviders
LIBS += -L../../../$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsqlengine
LIBS += -ltexteditor
LIBS += -lqtcutils

HEADERS += QLalrOracleProvider.h \
    QLalrOracleAuthWidget.h \
    qlalr_oracle_sql_constants.h \
    engine/QLalrOracleASTVisitor.h \
    engine/QLalrOracleASTfwd.h \
    engine/QLalrOracleAST.h \
    engine/QLalrOracleKeywords.h \
    engine/QLalrOracleSqlCheckNamesVisitor.h \
    engine/QLalrOracleSqlStatementsBindVisitor.h \
    engine/QLalrOracleSqlCompletionAssistVisitor.h \
    engine/QLalrOracleParserEngine.h \
    engine/qlalroracleparsertable_p.h \
    engine/QLalrOracleParser.h

SOURCES += QLalrOracleProvider.cpp \
    QLalrOracleAuthWidget.cpp \
    engine/QLalrOracleASTVisitor.cpp \
    engine/QLalrOracleASTVisit.cpp \
    engine/QLalrOracleAST.cpp \
    engine/QLalrOracleKeywords.cpp \
    engine/QLalrOracleSqlCheckNamesVisitor.cpp \
    engine/QLalrOracleSqlStatementsBindVisitor.cpp \
    engine/QLalrOracleSqlCompletionAssistVisitor.cpp \
    engine/QLalrOracleParserEngine.cpp \
    engine/QLalrOracleParser.cpp \
    engine/qlalroracleparsertable.cpp

FORMS += \
    QLalrOracleAuthWidget.ui
