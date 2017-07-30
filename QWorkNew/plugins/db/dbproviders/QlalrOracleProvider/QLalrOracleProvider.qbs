import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "QLalrOracleProvider"
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
        "QLalrOracleProvider.h",
        "QLalrOracleAuthWidget.h",
        "qlalr_oracle_sql_constants.h",
        "QLalrOracleProvider.cpp",
        "QLalrOracleAuthWidget.cpp",
        "QLalrOracleAuthWidget.ui",
        "engine/QLalrOracle.g",
        "engine/QLalrOracleParser.cpp",
        "engine/QLalrOracleParser.h",
        "engine/qlalroracleparsertable.cpp",
        "engine/qlalroracleparsertable_p.h",
        "engine/QLalrOracleAST.cpp",
        "engine/QLalrOracleAST.h",
        "engine/QLalrOracleASTVisitor.cpp",
        "engine/QLalrOracleASTVisitor.h",
        "engine/QLalrOracleASTVisit.cpp",
        "engine/QLalrOracleParserEngine.h",
        "engine/QLalrOracleParserEngine.cpp",
        "engine/QLalrOracleSqlCheckNamesVisitor.h",
        "engine/QLalrOracleSqlCheckNamesVisitor.cpp",
        "engine/QLalrOracleSqlStatementsBindVisitor.h",
        "engine/QLalrOracleSqlStatementsBindVisitor.cpp",
        "engine/QLalrOracleSqlCompletionAssistVisitor.h",
        "engine/QLalrOracleSqlCompletionAssistVisitor.cpp"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/dbproviders" : "bin_release/dbproviders"
    }
}
