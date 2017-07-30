import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "SqliteProvider"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: "dbproviders"
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "xml", "sql" ] }

    Depends { name: 'QWorkBase' }
    Depends { name: 'sqlengine' }
    
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["HAVE_QT5"] : []
    
    cpp.includePaths: [
        '../../../libs/qworkbase',
        '../../../libs/sqlengine',
        '../../../libs/settings',
        '../../../libs/corelib',
        '../../../libs/texteditorlib',
        '../../../libs/texteditorlib/codeassist'
    ]
     
    files: [
        "SqliteProvider.h",
        "SqliteAuthWidget.h",
        "SqliteProvider.cpp",
        "SqliteAuthWidget.cpp",
        "SqliteAuthWidget.ui"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/dbproviders" : "bin_release/dbproviders"
    }
}