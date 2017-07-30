include (../../../../common.prf)

QT += sql xml

TEMPLATE = lib
TARGET = kareleditor
CONFIG += plugin

DEFINES += SQLEDITOR_LIBRARY

INCLUDEPATH += ../../../../libs
INCLUDEPATH += ../../../../libs/utils
INCLUDEPATH += ../../../../libs/texteditorlib
INCLUDEPATH += ../../../../libs/texteditorlib/codeassist
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/find

DESTDIR = ../../$${TARGET_DIR}/editors
LIBS += -L../../$${TARGET_DIR}

LIBS += -lsettings
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}
LIBS += -lcore
LIBS += -lqtcutils
LIBS += -ltexteditor
LIBS += -lfind

HEADERS += \
    KarelEditorPlugin.h \
    KarelEditor.h \
    karel_constants.h \
    KarelToken.h \
    TranslationUnit.h \
    Control.h \
    KarelKeywords.h \
    KarelLexer.h \
    KarelHighlighter.h \
    KarelDocument.h \
    DiagnosticClient.h \
    KarelAST.h \
    KarelASTfwd.h \
    KarelASTVisitor.h \
    KarelParser.h \
    KarelSemanticInfo.h \
    KarelSemanticHighlighter.h \
    KarelCheckSymbols.h \
    KarelHoverHandler.h \
    KarelBind.h \
    KarelType.h \
    KarelIdentifierConstants.h \
    KarelSnippetProvider.h \
    KarelCompletionAssist.h \
    KarelEditorSettingsWidget.h \
    KarelDocumentCaseFixHandler.h \
    KarelSimpleLexer.h \
    KarelTypeResolver.h \
    KarelOverviewModel.h \
    KarelSymbolIcons.h \
    KarelReplaceUsagesDialog.h \
    KarelAutoCompleter.h \
    KarelIndenter.h

SOURCES += \
    KarelEditorPlugin.cpp \
    KarelEditor.cpp \
    KarelToken.cpp \
    TranslationUnit.cpp \
    Control.cpp \
    KarelKeywords.cpp \
    KarelLexer.cpp \
    KarelHighlighter.cpp \
    KarelDocument.cpp \
    DiagnosticClient.cpp \
    KarelAST.cpp \
    KarelASTVisitor.cpp \
    KarelParser.cpp \
    KarelSemanticInfo.cpp \
    KarelSemanticHighlighter.cpp \
    KarelCheckSymbols.cpp \
    KarelHoverHandler.cpp \
    KarelBind.cpp \
    KarelType.cpp \
    KarelSnippetProvider.cpp \
    KarelCompletionAssist.cpp \
    KarelEditorSettingsWidget.cpp \
    KarelDocumentCaseFixHandler.cpp \
    KarelSimpleLexer.cpp \
    KarelTypeResolver.cpp \
    KarelOverviewModel.cpp \
    KarelSymbolIcons.cpp \
    KarelReplaceUsagesDialog.cpp \
    KarelAutoCompleter.cpp \
    KarelIndenter.cpp

RESOURCES += \
    kareleditor.qrc

FORMS += \
    KarelEditorSettingsWidget.ui \
    KarelReplaceUsagesDialog.ui
