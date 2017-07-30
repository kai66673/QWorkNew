import qbs.base 1.0

Product {
    type: ["dynamiclibrary"]
    name: "database"
    targetName: (qbs.enableDebugCode && qbs.targetOS.contains("windows")) ? (name + 'd') : name
    destinationDirectory: "plugins"
    
    Depends { name: 'cpp' }
    Depends { name: "Qt"; submodules: [ "core", "widgets", "xml", "sql" ] }

    Depends { name: 'QWorkBase' }
    Depends { name: 'settings' }
    Depends { name: 'sqlengine' }
    Depends { name: 'core' }
    
    cpp.defines: Qt.core.version.charAt(0) == '5' ? ["DATABASE_LIBRARY", "HAVE_QT5"] : ["DATABASE_LIBRARY"]

    property var pythonQtInclude:  Qt.core.version.charAt(0) == '5' ? '../../../PyInclude/PythonQt5' : '../../../PyInclude/PythonQt4'
    
    cpp.includePaths: [ '.',
                        '../../libs/qworkbase',
                        '../../libs/qworkbase/scripting',
                        '../../libs/settings',
                        '../../libs/texteditorlib',
                        '../../libs/texteditorlib/codeassist',
                        '../../libs/corelib',
                        '../../libs/sqlengine',
                        '../../../PyInclude/Python', pythonQtInclude ]
     
    files: [
        "database_global.h",
        "DbManager.h",
        "DbConnectionsHistoryDialog.h",
        "DbConnectionsHistory.h",
        "QSqlHistoryModel.h",
        "database_constants.h",
        "QSqlHistoryTableView.h",
        "SqlHistoryFilterDialog.h",
        "SqlEditorSettingsWidget.h",
        "SqlTextFormatModel.h",
        "SqlSemanticTextFormatModel.h",
        "QueryViewSettingsWidget.h",
        "ConnectDialog.h",
        "QDbStructWidget.h",
        "DbStructTreeView.h",
        "QRdbQueryModel.h",
        "QRdbTableView.h",
        "QXHeaderView.h",
        "QXSortFilterProxyModel.h",
        "DbQueryModel.h",
        "DbFilterDialog.h",
        "DbDetailsWidget.h",
        "DbManager.cpp",
        "DbConnectionsHistoryDialog.cpp",
        "DbConnectionsHistory.cpp",
        "QSqlHistoryModel.cpp",
        "QSqlHistoryTableView.cpp",
        "SqlHistoryFilterDialog.cpp",
        "SqlEditorSettingsWidget.cpp",
        "SqlTextFormatModel.cpp",
        "SqlSemanticTextFormatModel.cpp",
        "QueryViewSettingsWidget.cpp",
        "ConnectDialog.cpp",
        "QDbStructWidget.cpp",
        "DbStructTreeView.cpp",
        "QRdbQueryModel.cpp",
        "QRdbTableView.cpp",
        "QXSortFilterProxyModel.cpp",
        "QXHeaderView.cpp",
        "DbQueryModel.cpp",
        "DbFilterDialog.cpp",
        "DbDetailsWidget.cpp",
        "DbConnectionsHistoryDialog.ui",
        "SqlHistoryFilterDialog.ui",
        "SqlEditorSettingsWidget.ui",
        "QueryViewSettingsWidget.ui",
        "ConnectDialog.ui",
        "DbFilterDialog.ui",
        "database.qrc"
    ]
    
    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: qbs.enableDebugCode ? "bin_debug/plugins" : "bin_release/plugins"
    }
}
