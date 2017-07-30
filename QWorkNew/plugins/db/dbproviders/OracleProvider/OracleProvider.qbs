import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "OracleProvider"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: "dbproviders"
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "xml", "sql" ] }

    Depends { name: 'QWorkBase' }
    Depends { name: 'sqlengine' }
    Depends { name: 'texteditor' }
    
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["HAVE_QT5"] : []
    
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
        "OracleProvider.h",
        "OracleAuthWidget.h",
        "oracle_sql_constants.h",
        "engine/OracleParser.h",
        "engine/OracleASTVisitor.h",
        "engine/OracleASTfwd.h",
        "engine/OracleAST.h",
        "engine/OracleKeywords.h",
        "engine/OracleSqlCheckNamesVisitor.h",
        "engine/OracleSqlStatementsBindVisitor.h",
        "engine/OracleSqlCompletionAssistVisitor.h",
        "engine/OracleParserEngine.h",
        "OracleProvider.cpp",
        "OracleAuthWidget.cpp",
        "engine/OracleParser.cpp",
        "engine/OracleASTVisitor.cpp",
        "engine/OracleASTVisit.cpp",
        "engine/OracleASTParse.cpp",
        "engine/OracleAST.cpp",
        "engine/OracleKeywords.cpp",
        "engine/OracleSqlCheckNamesVisitor.cpp",
        "engine/OracleSqlStatementsBindVisitor.cpp",
        "engine/OracleSqlCompletionAssistVisitor.cpp",
        "engine/OracleParserEngine.cpp",
        "OracleAuthWidget.ui"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/dbproviders" : "bin_release/dbproviders"
    }
}