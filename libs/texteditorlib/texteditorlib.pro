include (../../common.prf)

QT += network xml
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}

TEMPLATE = lib
TARGET = texteditor

DEFINES += TEXTEDITOR_LIBRARY

INCLUDEPATH += .
INCLUDEPATH += ./tooltip
INCLUDEPATH += ./snippets
INCLUDEPATH += ./codeassist
INCLUDEPATH += ./generichighlighter
INCLUDEPATH += ..
INCLUDEPATH += ../settings
INCLUDEPATH += ../corelib
INCLUDEPATH += ../utils

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lsettings
LIBS += -lcore
LIBS += -lqtcutils
LIBS += -laggregate

HEADERS += \
    texteditor_global.h \
    basetexteditor_p.h \
    basetextdocumentlayout.h \
    basetextdocument.h \
    typingsettings.h \
    storagesettings.h \
    behaviorsettings.h \
    displaysettings.h \
    tabsettings.h \
    extraencodingsettings.h \
    fontsettings.h \
    colorscheme.h \
    syntaxhighlighter.h \
    refactoroverlay.h \
    texteditorconstants.h \
    codecselector.h \
    completionsettings.h \
    icodestylepreferences.h \
    icodestylepreferencesfactory.h \
    codestylepool.h \
    tooltip/tips.h \
    tooltip/tipfactory.h \
    tooltip/tipcontents.h \
    tooltip/reuse.h \
    tooltip/effects.h \
    tooltip/tooltip.h \
    indenter.h \
    autocompleter.h \
    texteditorsettings.h \
    snippets/snippetssettings.h \
    snippets/snippetscollection.h \
    snippets/snippeteditor.h \
    snippets/snippetassistcollector.h \
    snippets/snippet.h \
    snippets/reuse.h \
    snippets/isnippetprovider.h \
    codeassist/quickfixassistprovider.h \
    codeassist/quickfixassistprocessor.h \
    codeassist/igenericproposalmodel.h \
    codeassist/ifunctionhintproposalmodel.h \
    codeassist/iassistproposalwidget.h \
    codeassist/iassistproposalmodel.h \
    codeassist/iassistproposalitem.h \
    codeassist/iassistproposal.h \
    codeassist/iassistprocessor.h \
    codeassist/iassistinterface.h \
    codeassist/genericproposalwidget.h \
    codeassist/genericproposal.h \
    codeassist/functionhintproposalwidget.h \
    codeassist/functionhintproposal.h \
    codeassist/defaultassistinterface.h \
    codeassist/codeassistant.h \
    codeassist/basicproposalitemlistmodel.h \
    codeassist/basicproposalitem.h \
    codeassist/assistenums.h \
    codeassist/runner.h \
    convenience.h \
    texteditoroverlay.h \
    basetextfind.h \
    normalindenter.h \
    quickfix.h \
    basehoverhandler.h \
    helpitem.h \
    refactoringchanges.h \
    ioutlinewidget.h \
    PlainTextDocumentPlugin.h \
    PlainTextEditor.h \
    TextEditorSettingsWidget.h \
    TextFormatModel.h \
    generichighlighter/context.h \
    generichighlighter/definitiondownloader.h \
    generichighlighter/dynamicrule.h \
    generichighlighter/highlightdefinition.h \
    generichighlighter/highlightdefinitionhandler.h \
    generichighlighter/highlightdefinitionmetadata.h \
    generichighlighter/highlighter.h \
    generichighlighter/highlighterexception.h \
    generichighlighter/highlightersettings.h \
    generichighlighter/includerulesinstruction.h \
    generichighlighter/itemdata.h \
    generichighlighter/keywordlist.h \
    generichighlighter/manager.h \
    generichighlighter/progressdata.h \
    generichighlighter/reuse.h \
    generichighlighter/rule.h \
    generichighlighter/specificrules.h \
    texteditor.h \
    IBaseTextDocument.h \
    snippets/SnippetsTableModel.h \
    snippets/plaintextsnippetprovider.h \
    IFixOnSaveHandler.h

SOURCES += \
    basetexteditor.cpp \
    basetextdocumentlayout.cpp \
    basetextdocument.cpp \
    typingsettings.cpp \
    behaviorsettings.cpp \
    storagesettings.cpp \
    tabsettings.cpp \
    displaysettings.cpp \
    fontsettings.cpp \
    extraencodingsettings.cpp \
    syntaxhighlighter.cpp \
    colorscheme.cpp \
    refactoroverlay.cpp \
    codecselector.cpp \
    completionsettings.cpp \
    icodestylepreferences.cpp \
    codestylepool.cpp \
    icodestylepreferencesfactory.cpp \
    tooltip/tooltip.cpp \
    tooltip/tips.cpp \
    tooltip/tipfactory.cpp \
    tooltip/tipcontents.cpp \
    indenter.cpp \
    autocompleter.cpp \
    texteditorsettings.cpp \
    snippets/snippetssettings.cpp \
    snippets/snippetscollection.cpp \
    snippets/snippeteditor.cpp \
    snippets/snippetassistcollector.cpp \
    snippets/snippet.cpp \
    snippets/isnippetprovider.cpp \
    codeassist/runner.cpp \
    codeassist/quickfixassistprovider.cpp \
    codeassist/quickfixassistprocessor.cpp \
    codeassist/igenericproposalmodel.cpp \
    codeassist/ifunctionhintproposalmodel.cpp \
    codeassist/iassistproposalwidget.cpp \
    codeassist/iassistproposalmodel.cpp \
    codeassist/iassistproposalitem.cpp \
    codeassist/iassistproposal.cpp \
    codeassist/iassistprocessor.cpp \
    codeassist/iassistinterface.cpp \
    codeassist/genericproposalwidget.cpp \
    codeassist/genericproposal.cpp \
    codeassist/functionhintproposalwidget.cpp \
    codeassist/functionhintproposal.cpp \
    codeassist/defaultassistinterface.cpp \
    codeassist/codeassistant.cpp \
    codeassist/basicproposalitemlistmodel.cpp \
    codeassist/basicproposalitem.cpp \
    convenience.cpp \
    texteditoroverlay.cpp \
    basetextfind.cpp \
    normalindenter.cpp \
    quickfix.cpp \
    basehoverhandler.cpp \
    helpitem.cpp \
    refactoringchanges.cpp \
    PlainTextDocumentPlugin.cpp \
    PlainTextEditor.cpp \
    TextEditorSettingsWidget.cpp \
    TextFormatModel.cpp \
    generichighlighter/context.cpp \
    generichighlighter/definitiondownloader.cpp \
    generichighlighter/dynamicrule.cpp \
    generichighlighter/highlightdefinition.cpp \
    generichighlighter/highlightdefinitionhandler.cpp \
    generichighlighter/highlightdefinitionmetadata.cpp \
    generichighlighter/highlighter.cpp \
    generichighlighter/highlightersettings.cpp \
    generichighlighter/includerulesinstruction.cpp \
    generichighlighter/itemdata.cpp \
    generichighlighter/keywordlist.cpp \
    generichighlighter/manager.cpp \
    generichighlighter/progressdata.cpp \
    generichighlighter/rule.cpp \
    generichighlighter/specificrules.cpp \
    IBaseTextDocument.cpp \
    codeassist/keywordscompletionassist.cpp \
    snippets/SnippetsTableModel.cpp \
    snippets/plaintextsnippetprovider.cpp

FORMS += \
    TextEditorSettingsWidget.ui

RESOURCES += \
    texteditorlib.qrc

generic_highlighter_xml.path = $${DESTDIR}/generic-highlighter
generic_highlighter_xml.files = generichighlighter/config_xml/*.xml
INSTALLS += generic_highlighter_xml
