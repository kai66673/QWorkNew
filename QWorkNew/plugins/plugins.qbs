import qbs

Project {
    name: "QWorkBasePlugins"
    references: [
        "aliases/aliases.qbs",
        "asproject/asproject.qbs",
        "asproject/dspimporter/dspimporter.qbs",
        "database/database.qbs",
        "database/Db2Provider/Db2Provider.qbs",
        "database/OracleProvider/OracleProvider.qbs",
        "database/QlalrOracleProvider/QLalrOracleProvider.qbs",
        "database/OracleRdbProvider/OracleRdbProvider.qbs",
        "database/SqliteProvider/SqliteProvider.qbs",
        "sqleditor/sqleditor.qbs",
        "cppeditor/cppeditor.qbs",
        "pythoneditor/pythoneditor.qbs"
    ]
}
