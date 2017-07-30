import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "sqleditor"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: "editors"
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "sql", "printsupport" ] }
    Depends {
        condition: Qt.core.version.charAt(0) == '5'
        name: "Qt"
        submodules: ["concurrent"]
    }

    Depends { name: 'QWorkBase' }
    Depends { name: 'settings' }
    Depends { name: 'sqlengine' }
    Depends { name: 'core' }
    Depends { name: 'qtcutils' }
    Depends { name: 'texteditor' }
    
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["HAVE_QT5"] : []

    cpp.includePaths: [
        '.',
        '../../libs',
        '../../libs/qworkbase',
        '../../libs/utils',
        '../../libs/corelib',
        '../../libs/settings',
        '../../libs/sqlengine',
        '../../libs/texteditorlib',
        '../../libs/texteditorlib/codeassist'
    ]
     
    files: [
        "SQLEditor.h",
        "sqleditorconstants.h",
        "SqlEditorPlugin.h",
        "SqlEditorSupport.h",
        "SqlSemanticHighlighter.h",
        "SqlCheckNames.h",
        "SqlSemanticInfo.h",
        "SqlHoverHandler.h",
        "SqlCompletiotionAssist.h",
        "SQLEditor.cpp",
        "SqlEditorPlugin.cpp",
        "SqlEditorSupport.cpp",
        "SqlSemanticHighlighter.cpp",
        "SqlCheckNames.cpp",
        "SqlSemanticInfo.cpp",
        "SqlHoverHandler.cpp",
        "SqlCompletiotionAssist.cpp",
        "sqleditor.qrc"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/editors" : "bin_release/editors"
    }
}