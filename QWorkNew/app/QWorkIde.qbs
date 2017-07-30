import qbs.base 1.0

Product {
    type: ["application"]
    name: "QWorkIde"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    
    consoleApplication: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? true : false
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets" ] }
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["HAVE_QT5"] : []

    Depends { name: "QWorkBase" }
    Depends { name: "settings" }
    Depends { name: "core" }
    Depends { name: "texteditor" }
        
    cpp.includePaths: [
        '.',
        '../libs/qworkbase',
        '../libs/settings',
        '../libs/corelib'
    ]
     
    files: [
        "main.cpp",
        "QWorkIde.rc"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug" : "bin_release"
    }
}
