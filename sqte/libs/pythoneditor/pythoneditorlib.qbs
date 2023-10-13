import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "pythoneditorlib"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "printsupport" ] }
    Depends { name: "settings" }
    Depends { name: "core" }
    Depends { name: "qtcutils" }
    Depends { name: "texteditor" }
    
    property var commonDefines: ["PYTHONEDITOR_LIBRARY"]
    Properties {
        condition: Qt.core.version.charAt(0) == '5'
        cpp.defines: commonDefines.concat(["HAVE_QT5"])
    }
    cpp.defines: commonDefines
    
    cpp.includePaths: [ '.', '..', '../settings', '../corelib', '../utils', '../texteditorlib' ]
     
    files: [
        "tools/lexical/pythonscanner.h",
        "tools/lexical/pythonformattoken.h",
        "tools/lexical/sourcecodestream.h",
        "tools/pythonhighlighter.h",
        "tools/pythonindenter.h",
        "pythoneditor_global.h",
        "pythoneditor.h",
        "pythoneditorwidget.h",
        "tools/lexical/pythonscanner.cpp",
        "tools/pythonindenter.cpp",
        "tools/pythonhighlighter.cpp",
        "pythoneditorwidget.cpp",
        "pythoneditor.cpp"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug" : "bin_release"
    }
}
