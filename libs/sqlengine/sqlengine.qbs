import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "sqlengine"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    
    Depends { name: 'cpp' }
    Depends {
        name : "Qt"
        submodules: [
            "core",
            "widgets",
            "printsupport",
            "sql"
        ]
    }
    Depends {
        condition: Qt.core.version.charAt(0) == '5'
        name: "Qt"
        submodules: ["concurrent"]
    }

    Depends { name: "settings" }
    Depends { name: "core" }
    Depends { name: "QWorkBase" }
    Depends { name: "texteditor" }
    
    property var commonDefines: ["SQLENGINE_BUILD_LIB", "_CRT_SECURE_NO_WARNINGS"]
    Properties {
        condition: Qt.core.version.charAt(0) == '5'
        cpp.defines: commonDefines.concat(["HAVE_QT5"])
    }
    cpp.defines: commonDefines
    
    cpp.includePaths: [ '.',
                        '../qworkbase',
                        '../qworkbase/scripting',
                        '../settings', '../corelib',
                        '../utils', '../texteditorlib',
                        '../texteditorlib/codeassist'
                      ]
     
    files: [
        "AST.h",
        "SqlToken.h",
        "SqlParserEngine.h",
        "SqlLexer.h",
        "sqlengine_global.h",
        "Parser.h",
        "MemoryPool.h",
        "IParserEngine.h",
        "ASTVisitor.h",
        "ASTMatcher.h",
        "ASTfwd.h",
        "TranslationUnit.h",
        "SqlHighlighter.h",
        "Control.h",
        "sql_constants.h",
        "SqlDocument.h",
        "DiagnosticClient.h",
        "DbConnection.h",
        "DbLoader.h",
        "DbUtils.h",
        "SqlDefineBindingsDialog.h",
        "SqlBindHelper.h",
        "DbCatalog.h",
        "DbDetails.h",
        "SqlTextEditorWidget.h",
        "DbDetailsWithSourceWidget.h",
        "DbMetadataProvider.h",
        "sqlchecknamesvisitor.h",
        "SqlParserEngineManager.h",
        "SqlStatement.h",
        "SqlStatementsBindVisitor.h",
        "SqlCompletionAssistVisitor.h",
        "ISemanticParser.h",
        "BaseWalker.h",
        "SqlNameUse.h",
        "AST.cpp",
        "TranslationUnit.cpp",
        "SqlToken.cpp",
        "SqlParserEngine.cpp",
        "SqlLexer.cpp",
        "Parser.cpp",
        "MemoryPool.cpp",
        "IParserEngine.cpp",
        "ASTVisitor.cpp",
        "ASTVisit.cpp",
        "ASTMatcher.cpp",
        "ASTMatch0.cpp",
        "ASTClone.cpp",
        "SqlHighlighter.cpp",
        "Control.cpp",
        "SqlDocument.cpp",
        "ASTParse.cpp",
        "DiagnosticClient.cpp",
        "DbConnection.cpp",
        "DbLoader.cpp",
        "DbUtils.cpp",
        "SqlDefineBindingsDialog.cpp",
        "SqlBindHelper.cpp",
        "DbCatalog.cpp",
        "DbDetails.cpp",
        "SqlTextEditorWidget.cpp",
        "DbDetailsWithSourceWidget.cpp",
        "DbMetadataProvider.cpp",
        "sqlchecknamesvisitor.cpp",
        "SqlParserEngineManager.cpp",
        "SqlStatement.cpp",
        "SqlStatementsBindVisitor.cpp",
        "SqlCompletionAssistVisitor.cpp",
        "ISemanticParser.cpp",
        "BaseWalker.cpp",
        "SqlNameUse.cpp",
        "sqlengine.qrc",
        "SqlDefineBindingsDialog.ui"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug" : "bin_release"
    }
}
