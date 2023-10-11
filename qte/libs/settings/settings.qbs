import qbs.base 1.0
 
Product {
    type: ["dynamiclibrary"]
    name: "settings"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets" ] }
    
    cpp.defines: ['SETTINGS_LIBRARY']
 
    files: [
        "SettingsManager.h",
        "SettingsDialog.h",
        "settings_global.h",
        "SettingsManager.cpp",
        "SettingsDialog.cpp"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug" : "bin_release"
    }
}
