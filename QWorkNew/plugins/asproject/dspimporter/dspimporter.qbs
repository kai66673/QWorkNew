import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "DspImporter"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: "aspdbimporters"
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "xml", "sql" ] }

    Depends { name: 'QWorkBase' }
    Depends { name: 'asproject' }
    Depends { name: 'qtcutils' }
    
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["HAVE_QT5"] : []

    cpp.includePaths: [
        '.',
        '..',
        '../../../libs/qworkbase',
        '../../../libs/corelib',
        '../../../libs/settings',
        '../../../libs'
    ]
     
    files: [
        "DspConfigDialog.h",
        "DspImporterPlugin.cpp",
        "DspImporterPlugin.h",
        "DspConfigDialog.cpp",
        "DspConfigDialog.ui"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/aspdbimporters" : "bin_release/aspdbimporters"
    }
}