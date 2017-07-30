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

TARGET = RdbProvider

win32:DEFINES += _WINDOWS

DESTDIR = ../../../$${TARGET_DIR}/dbproviders
LIBS += -L../../../$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsqlengine
LIBS += -ltexteditor
LIBS += -lqtcutils
LIBS *= -lodbc32

HEADERS += RdbProvider.h \
    RdbAuthWidget.h \
    engine/rdb_sql_constants.h \
    engine/RdbParserEngine.h \
    engine/RdbKeywords.h \
    engine/RdbParser.h \
    engine/RdbAST.h \
    engine/RdbASTMatcher.h \
    engine/RdbSqlStatementsBindVisitor.h \
    engine/RdbASTVisitor.h \
    engine/RdbSqlCompletionAssistVisitor.h \
    engine/RdbSqlCheckNamesVisitor.h \
    engine/RdbASTfwd.h \
    engine/RdbSqlScope.h \
    engine/RdbSqlLinkFinder.h

SOURCES += RdbProvider.cpp \
    RdbAuthWidget.cpp \
    engine/RdbParserEngine.cpp \
    engine/RdbKeywords.cpp \
    engine/RdbParser.cpp \
    engine/RdbAST.cpp \
    engine/RdbASTClone.cpp \
    engine/RdbASTMatch0.cpp \
    engine/RdbASTMatcher.cpp \
    engine/RdbASTVisit.cpp \
    engine/RdbASTParse.cpp \
    engine/RdbSqlStatementsBindVisitor.cpp \
    engine/RdbASTVisitor.cpp \
    engine/RdbSqlCompletionAssistVisitor.cpp \
    engine/RdbSqlCheckNamesVisitor.cpp \
    engine/RdbSqlScope.cpp \
    engine/RdbSqlLinkFinder.cpp

FORMS += \
    RdbAuthWidget.ui
