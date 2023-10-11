import qbs.base 1.0
 
Product {
    type: ["dynamiclibrary"]
    name: "aggregate"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core" ] }
    
    cpp.defines: ['AGGREGATION_LIBRARY']
 
    files: [
        "aggregate.cpp",
        "aggregation_global.h",
        "aggregate.h"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug" : "bin_release"
    }
}