include (../../common.prf)

TEMPLATE = lib
TARGET = cplusplus

DEPENDPATH += .

QT += xml network
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}

DEFINES += CPLUSPLUS_BUILD_LIB
DEFINES += LANGUAGEUTILS_BUILD_DIR
DEFINES += CPPTOOLS_LIBRARY
win32-msvc*:DEFINES += _SCL_SECURE_NO_WARNINGS

INCLUDEPATH += .
INCLUDEPATH += ./shared
INCLUDEPATH += ./cpptools
INCLUDEPATH += ../Qt4Concurrent
INCLUDEPATH += ../texteditorlib
INCLUDEPATH += ../texteditorlib/codeassist
INCLUDEPATH += ../corelib
INCLUDEPATH += ../corelib/CodeModel
INCLUDEPATH += ../ProjectExplorer
INCLUDEPATH += ../ProjectExplorer/devicesupport
INCLUDEPATH += ../ProjectExplorer/options
INCLUDEPATH += ../ProjectExplorer/qtsupport
INCLUDEPATH += ../settings
INCLUDEPATH += ../utils
INCLUDEPATH += ../find
INCLUDEPATH += ../find/locator

LIBS += -lsettings
LIBS += -lcore
LIBS += -lfind
LIBS += -lqtcutils
LIBS += -ltexteditor
LIBS += -lProjectExplorer

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

HEADERS += \
    languageutils/fakemetaobject.h \
    languageutils/componentversion.h \
    languageutils/languageutils_global.h \
    shared/TypeVisitor.h \
    shared/TypeMatcher.h \
    shared/Type.h \
    shared/TranslationUnit.h \
    shared/Token.h \
    shared/Templates.h \
    shared/SymbolVisitor.h \
    shared/Symbols.h \
    shared/Symbol.h \
    shared/Scope.h \
    shared/QtContextKeywords.h \
    shared/Parser.h \
    shared/ObjectiveCTypeQualifiers.h \
    shared/NameVisitor.h \
    shared/Names.h \
    shared/Name.h \
    shared/LiteralTable.h \
    shared/Literals.h \
    shared/Lexer.h \
    shared/FullySpecifiedType.h \
    shared/DiagnosticClient.h \
    shared/CPlusPlusForwardDeclarations.h \
    shared/CPlusPlus.h \
    shared/CoreTypes.h \
    shared/Control.h \
    shared/Bind.h \
    shared/ASTVisitor.h \
    shared/ASTPatternBuilder.h \
    shared/ASTMatcher.h \
    shared/ASTfwd.h \
    shared/AST.h \
    findcdbbreakpoint.h \
    TypeHierarchyBuilder.h \
    cppdoxygen.h \
    cpptools/cppcodeformatter.h \
    cpptools/cppcodestylepreferences.h \
    cpptools/cppcodestylepreferencesfactory.h \
    cpptools/cppcodestylesettings.h \
    cpptools/cppcompletionassist.h \
    cpptools/cppfindreferences.h \
    cpptools/cppmodelmanager.h \
    cpptools/cppqtstyleindenter.h \
    cpptools/cpprefactoringchanges.h \
    cpptools/cpptools_global.h \
    cpptools/cpptoolsconstants.h \
    cpptools/cpptoolseditorsupport.h \
    cpptools/cpptoolsreuse.h \
    cpptools/cpptoolssettings.h \
    cpptools/insertionpointlocator.h \
    cpptools/searchsymbols.h \
    cpptools/CppReplaceUsagesDialog.h \
    cpptools/cppautocompleter.h \
    cpptools/cppelementevaluator.h \
    cpptools/cpphoverhandler.h \
    cpptools/cppchecksymbols.h \
    cpptools/cpplocalsymbols.h \
    cpptools/cppsemanticinfo.h \
    shared/GrammarBind.h \
    cpptools/symbolfinder.h \
    cpptools/functionutils.h \
    cpptools/cppvirtualfunctionassistprovider.h \
    cpptools/cppvirtualfunctionproposalitem.h \
    cpptools/VirtualFunctionHelper.h \
    cpptools/builtinindexingsupport.h \
    cpptools/cppindexingsupport.h \
    cpptools/indexitem.h \
    cpptools/stringtable.h \
    cpptools/CppHelperDialog.h \
    cpptools/CppHelperItem.h \
    cpptools/cppprojectfile.h \
    cpptools/cppprojects.h \
    cpptools/cpplocatordata.h \
    cpptools/cpplocatorfilter.h \
    cpptools/cppclassesfilter.h \
    cpptools/cppfunctionsfilter.h \
    cpptools/CppToolsUtils.h \
    cpptools/ProposeNameResolver.h \
    cpptools/CppLibrariesManager.h

SOURCES += \
    shared/QtContextKeywords.cpp \
    languageutils/fakemetaobject.cpp \
    languageutils/componentversion.cpp \
    shared/TypeMatcher.cpp \
    shared/Type.cpp \
    shared/TranslationUnit.cpp \
    shared/Token.cpp \
    shared/Templates.cpp \
    shared/SymbolVisitor.cpp \
    shared/Symbols.cpp \
    shared/Symbol.cpp \
    shared/Scope.cpp \
    shared/Parser.cpp \
    shared/ObjectiveCTypeQualifiers.cpp \
    shared/ObjectiveCAtKeywords.cpp \
    shared/NameVisitor.cpp \
    shared/Names.cpp \
    shared/Name.cpp \
    shared/LiteralTable.cpp \
    shared/Literals.cpp \
    shared/Lexer.cpp \
    shared/Keywords.cpp \
    shared/FullySpecifiedType.cpp \
    shared/DiagnosticClient.cpp \
    shared/CoreTypes.cpp \
    shared/Control.cpp \
    shared/Bind.cpp \
    shared/ASTVisitor.cpp \
    shared/ASTVisit.cpp \
    shared/ASTPatternBuilder.cpp \
    shared/ASTMatcher.cpp \
    shared/ASTMatch0.cpp \
    shared/ASTClone.cpp \
    shared/AST.cpp \
    shared/TypeVisitor.cpp \
    TypeHierarchyBuilder.cpp \
    findcdbbreakpoint.cpp \
    cppdoxygen.cpp \
    cpptools/cppcodeformatter.cpp \
    cpptools/cppcodestylepreferences.cpp \
    cpptools/cppcodestylepreferencesfactory.cpp \
    cpptools/cppcodestylesettings.cpp \
    cpptools/cppcompletionassist.cpp \
    cpptools/cppfindreferences.cpp \
    cpptools/cppmodelmanager.cpp \
    cpptools/cppqtstyleindenter.cpp \
    cpptools/cpprefactoringchanges.cpp \
    cpptools/cpptoolseditorsupport.cpp \
    cpptools/cpptoolsreuse.cpp \
    cpptools/cpptoolssettings.cpp \
    cpptools/insertionpointlocator.cpp \
    cpptools/searchsymbols.cpp \
    cpptools/CppReplaceUsagesDialog.cpp \
    cpptools/cppautocompleter.cpp \
    cpptools/cppelementevaluator.cpp \
    cpptools/cpphoverhandler.cpp \
    cpptools/cppchecksymbols.cpp \
    cpptools/cpplocalsymbols.cpp \
    cpptools/cppsemanticinfo.cpp \
    shared/GrammarBind.cpp \
    cpptools/symbolfinder.cpp \
    cpptools/functionutils.cpp \
    cpptools/cppvirtualfunctionassistprovider.cpp \
    cpptools/cppvirtualfunctionproposalitem.cpp \
    cpptools/VirtualFunctionHelper.cpp \
    cpptools/builtinindexingsupport.cpp \
    cpptools/cppindexingsupport.cpp \
    cpptools/indexitem.cpp \
    cpptools/stringtable.cpp \
    cpptools/CppHelperDialog.cpp \
    cpptools/CppHelperItem.cpp \
    cpptools/cppprojectfile.cpp \
    cpptools/cppprojects.cpp \
    cpptools/cpplocatordata.cpp \
    cpptools/cpplocatorfilter.cpp \
    cpptools/cppclassesfilter.cpp \
    cpptools/cppfunctionsfilter.cpp \
    cpptools/CppToolsUtils.cpp \
    cpptools/ProposeNameResolver.cpp \
    cpptools/CppLibrariesManager.cpp

HEADERS += \
    Icons.h \
    ExpressionUnderCursor.h \
    BackwardsScanner.h \
    MatchingText.h \
    OverviewModel.h

SOURCES += \
    Icons.cpp \
    ExpressionUnderCursor.cpp \
    BackwardsScanner.cpp \
    MatchingText.cpp \
    OverviewModel.cpp

HEADERS += \
    SimpleLexer.h \
    CppDocument.h \
    CppRewriter.h \
    Overview.h \
    NamePrettyPrinter.h \
    TypeOfExpression.h \
    TypePrettyPrinter.h \
    ResolveExpression.h \
    LookupItem.h \
    LookupContext.h \
    ASTParent.h \
    ASTPath.h \
    SnapshotSymbolVisitor.h \
    SymbolNameVisitor.h \
    DeprecatedGenTemplateInstance.h \
    FindUsages.h \
    DependencyTable.h \
    PreprocessorClient.h \
    PreprocessorEnvironment.h \
    Macro.h \
    FastPreprocessor.h \
    pp.h \
    pp-cctype.h \
    pp-engine.h \
    pp-macro-expander.h \
    pp-scanner.h \
    ModelManagerInterface.h

SOURCES += \
    SimpleLexer.cpp \
    CppDocument.cpp \
    CppRewriter.cpp \
    Overview.cpp \
    NamePrettyPrinter.cpp \
    TypeOfExpression.cpp \
    TypePrettyPrinter.cpp \
    ResolveExpression.cpp \
    LookupItem.cpp \
    LookupContext.cpp \
    ASTParent.cpp \
    ASTPath.cpp \
    SnapshotSymbolVisitor.cpp \
    SymbolNameVisitor.cpp \
    DeprecatedGenTemplateInstance.cpp \
    FindUsages.cpp \
    DependencyTable.cpp \
    PreprocessorClient.cpp \
    PreprocessorEnvironment.cpp \
    FastPreprocessor.cpp \
    Macro.cpp \
    pp-engine.cpp \
    pp-macro-expander.cpp \
    pp-scanner.cpp \
    ModelManagerInterface.cpp

RESOURCES += cplusplus.qrc

SUBDIRS += \
    cpptools/cpptools.pro

OTHER_FILES += \
    cpptools/cpptools.pri \
    cpptools/cpptools_dependencies.pri \
    cpptools/CppTools.pluginspec.in

FORMS += \
    cpptools/completionsettingspage.ui \
    cpptools/CppReplaceUsagesDialog.ui \
    cpptools/CppHelperDialog.ui


