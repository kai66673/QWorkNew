import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "qtcutils"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "network" ] }
    
    property var commonDefines: ["QTCREATOR_UTILS_LIB"]
    Properties {
        condition: Qt.core.version.charAt(0) == '5'
        cpp.defines: commonDefines.concat(["HAVE_QT5"])
    }
    cpp.defines: commonDefines
    
    cpp.includePaths: [ '.', '../Qt4Concurrent']
     
    files: [
        "uncommentselection.h",
        "textfileformat.h",
        "stylehelper.h",
        "stringutils.h",
        "settingsutils.h",
        "savefile.h",
        "reloadpromptutils.h",
        "qtcprocess.h",
        "qtcassert.h",
        "persistentsettings.h",
        "networkaccessmanager.h",
        "navigationtreeview.h",
        "linecolumnlabel.h",
        "htmldocextractor.h",
        "fileutils.h",
        "filesearch.h",
        "fancymainwindow.h",
        "faketooltip.h",
        "environment.h",
        "changeset.h",
        "utils_global.h",
        "RecentFilesMenu.h",
        "uncommentselection.cpp",
        "textfileformat.cpp",
        "stylehelper.cpp",
        "stringutils.cpp",
        "savefile.cpp",
        "reloadpromptutils.cpp",
        "qtcprocess.cpp",
        "persistentsettings.cpp",
        "networkaccessmanager.cpp",
        "navigationtreeview.cpp",
        "linecolumnlabel.cpp",
        "htmldocextractor.cpp",
        "fileutils.cpp",
        "filesearch.cpp",
        "fancymainwindow.cpp",
        "faketooltip.cpp",
        "environment.cpp",
        "changeset.cpp",
        "RecentFilesMenu.cpp"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug" : "bin_release"
    }
}