include (../../common.prf)

QT += sql xml

TEMPLATE = lib
TARGET = sqlengine
DEPENDPATH += .

INCLUDEPATH += .
INCLUDEPATH += ../texteditorlib
INCLUDEPATH += ../texteditorlib/codeassist
INCLUDEPATH += ../settings
INCLUDEPATH += ../corelib
INCLUDEPATH += ../utils

DEFINES += SQLENGINE_BUILD_LIB
DEFINES += _CRT_SECURE_NO_WARNINGS

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lcore
LIBS += -lsettings
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}
LIBS += -ltexteditor
LIBS += -lqtcutils

HEADERS += \
    AST.h \
    SqlToken.h \
    SqlParserEngine.h \
    SqlLexer.h \
    sqlengine_global.h \
    Parser.h \
    IParserEngine.h \
    ASTVisitor.h \
    ASTMatcher.h \
    ASTfwd.h \
    TranslationUnit.h \
    SqlHighlighter.h \
    Control.h \
    sql_constants.h \
    SqlDocument.h \
    DiagnosticClient.h \
    DbConnection.h \
    DbLoader.h \
    DbUtils.h \
    SqlDefineBindingsDialog.h \
    SqlBindHelper.h \
    DbCatalog.h \
    DbDetails.h \
    SqlTextEditorWidget.h \
    DbDetailsWithSourceWidget.h \
    DbMetadataProvider.h \
    sqlchecknamesvisitor.h \
    SqlParserEngineManager.h \
    SqlStatement.h \
    SqlStatementsBindVisitor.h \
    SqlCompletionAssistVisitor.h \
    ISemanticParser.h \
    BaseWalker.h \
    SqlNameUse.h \
    DetailsLink.h \
    SqlLinkFinder.h \
    SqlScope.h \
    DbConnectionAuthInfo.h

SOURCES += \
    AST.cpp \
    TranslationUnit.cpp \
    SqlToken.cpp \
    SqlParserEngine.cpp \
    SqlLexer.cpp \
    Parser.cpp \
    IParserEngine.cpp \
    ASTVisitor.cpp \
    ASTVisit.cpp \
    ASTMatcher.cpp \
    ASTMatch0.cpp \
    ASTClone.cpp \
    SqlHighlighter.cpp \
    Control.cpp \
    SqlDocument.cpp \
    ASTParse.cpp \
    DiagnosticClient.cpp \
    DbConnection.cpp \
    DbLoader.cpp \
    DbUtils.cpp \
    SqlDefineBindingsDialog.cpp \
    SqlBindHelper.cpp \
    DbCatalog.cpp \
    DbDetails.cpp \
    SqlTextEditorWidget.cpp \
    DbDetailsWithSourceWidget.cpp \
    DbMetadataProvider.cpp \
    sqlchecknamesvisitor.cpp \
    SqlParserEngineManager.cpp \
    SqlStatement.cpp \
    SqlStatementsBindVisitor.cpp \
    SqlCompletionAssistVisitor.cpp \
    ISemanticParser.cpp \
    BaseWalker.cpp \
    SqlNameUse.cpp \
    DetailsLink.cpp \
    SqlLinkFinder.cpp \
    SqlScope.cpp \
    DbConnectionAuthInfo.cpp

OTHER_FILES += \
    sqlengine.pro.user

RESOURCES += \
    sqlengine.qrc

FORMS += \
    SqlDefineBindingsDialog.ui

