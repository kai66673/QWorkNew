import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "RdbProvider"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: "dbproviders"
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "xml", "sql" ] }

    Depends { name: 'QWorkBase' }
    Depends { name: 'sqlengine' }
    Depends { name: 'texteditor' }
    
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["HAVE_QT5"] : []
    cpp.dynamicLibraries: ["odbc32"]
    
    cpp.includePaths: [
        'engine',
        '../../../libs/qworkbase',
        '../../../libs/sqlengine',
        '../../../libs/settings',
        '../../../libs/corelib',
        '../../../libs/texteditorlib',
        '../../../libs/texteditorlib/codeassist'
    ]
     
    files: [
        "RdbProvider.h",
        "RdbAuthWidget.h",
        "engine/rdb_sql_constants.h",
        "engine/RdbParserEngine.h",
        "engine/RdbKeywords.h",
        "engine/RdbParser.h",
        "engine/RdbAST.h",
        "engine/RdbASTMatcher.h",
        "engine/RdbSqlStatementsBindVisitor.h",
        "engine/RdbASTVisitor.h",
        "engine/RdbSqlCompletionAssistVisitor.h",
        "engine/RdbSqlCheckNamesVisitor.h",
        "engine/RdbASTfwd.h",
        "RdbProvider.cpp",
        "RdbAuthWidget.cpp",
        "engine/RdbParserEngine.cpp",
        "engine/RdbKeywords.cpp",
        "engine/RdbParser.cpp",
        "engine/RdbAST.cpp",
        "engine/RdbASTClone.cpp",
        "engine/RdbASTMatch0.cpp",
        "engine/RdbASTMatcher.cpp",
        "engine/RdbASTVisit.cpp",
        "engine/RdbASTParse.cpp",
        "engine/RdbSqlStatementsBindVisitor.cpp",
        "engine/RdbASTVisitor.cpp",
        "engine/RdbSqlCompletionAssistVisitor.cpp",
        "engine/RdbSqlCheckNamesVisitor.cpp",
        "RdbAuthWidget.ui"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/dbproviders" : "bin_release/dbproviders"
    }
}
