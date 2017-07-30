import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "find"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "network", "xml" ] }
    
    Depends { name: "settings" }
    Depends { name: "qtcutils" }
    Depends { name: "core" }
    Depends { name: "aggregate" }
    
    property var commonDefines: ["FIND_LIBRARY"]
    Properties {
        condition: Qt.core.version.charAt(0) == '5'
        cpp.defines: commonDefines.concat(["HAVE_QT5"])
    }
    cpp.defines: commonDefines
    
    cpp.includePaths: [ '.', '..', '../utils', '../corelib', '../settings']
     
    files: [
        "ifindfilter.h",
        "ifindfilter.cpp",
        "currentdocumentfind.cpp",
        "SearchEngine.cpp",
        "SearchResultTabWidget.cpp",
        "searchresulttreeitemdelegate.cpp",
        "searchresulttreeitems.cpp",
        "searchresulttreemodel.cpp",
        "searchresulttreeview.cpp",
        "searchresultwindow.cpp",
        "basefilefind.cpp",
        "BaseTextSearchParamsWidget.cpp",
        "FindReplaceWidget.cpp",
        "FindInDialog.cpp",
        "SearchReplaceTextDialog.cpp",
        "currentdocumentfind.h",
        "find_global.h",
        "SearchEngine.h",
        "SearchResultTabWidget.h",
        "searchresulttreeitemdelegate.h",
        "searchresulttreeitemroles.h",
        "searchresulttreeitems.h",
        "searchresulttreemodel.h",
        "searchresulttreeview.h",
        "searchresultwindow.h",
        "basefilefind.h",
        "BaseTextSearchParamsWidget.h",
        "FindReplaceWidget.h",
        "FindInDialog.h",
        "SearchReplaceTextDialog.h",
        "FindReplaceWidget.ui",
        "FindInDialog.ui",
        "SearchReplaceTextDialog.ui"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug" : "bin_release"
    }
}
