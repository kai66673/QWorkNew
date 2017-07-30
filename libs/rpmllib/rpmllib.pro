include (../../common.prf)

QT += network xml sql gui-private core-private
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}

TEMPLATE = lib
TARGET = rpmllib

DEFINES += RPML_LIBRARY

INCLUDEPATH += .
INCLUDEPATH += ../utils
INCLUDEPATH += ../corelib
INCLUDEPATH += ../settings
INCLUDEPATH += ../richeditorlib

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lqtcutils
LIBS += -lcore
LIBS += -lsettings
LIBS += -lricheditorlib

HEADERS += \
    rpmllib_global.h \
    RpmlProcessor.h \
    ReportData.h \
    PagesTextEdit.h \
    ReportEditorWidget.h \
    ReportPreferencesDialog.h \
    ReportInputParameters.h \
    ReportSelectInputParameterDialog.h \
    ReportSelectDataSourceColumnDialog.h \
    DataSource.h \
    DataSourceConnectParametersDialog.h \
    DbPostgresDataSourceProvider.h \
    DbPostgresDataSourceConnectWidget.h \
    DbSqliteDataSourceProvider.h \
    DbSqliteDataSourceConnectWidget.h \
    DbDataSourceQueryEditDialog.h \
    SqlHighlighter.h \
    InputParametersDialog.h \
    DataSourceAssistor.h \
    ReportDataTableDialog.h \
    textprinter.h \
    OdfWriter.h \
    DbLinterDataSourceConnectWidget.h \
    DbLinterDataSourceProvider.h

SOURCES += \
    RpmlProcessor.cpp \
    ReportData.cpp \
    PagesTextEdit.cpp \
    ReportEditorWidget.cpp \
    ReportPreferencesDialog.cpp \
    ReportInputParameters.cpp \
    ReportSelectInputParameterDialog.cpp \
    ReportSelectDataSourceColumnDialog.cpp \
    DataSource.cpp \
    DataSourceConnectParametersDialog.cpp \
    DbPostgresDataSourceProvider.cpp \
    DbPostgresDataSourceConnectWidget.cpp \
    DbSqliteDataSourceProvider.cpp \
    DbSqliteDataSourceConnectWidget.cpp \
    DbDataSourceQueryEditDialog.cpp \
    SqlHighlighter.cpp \
    InputParametersDialog.cpp \
    DataSourceAssistor.cpp \
    ReportDataTableDialog.cpp \
    textprinter.cpp \
    OdfWriter.cpp \
    DbLinterDataSourceConnectWidget.cpp \
    DbLinterDataSourceProvider.cpp

RESOURCES += \
    rpmllib.qrc

FORMS += \
    ReportPreferencesDialog.ui \
    ReportSelectInputParameterDialog.ui \
    ReportSelectDataSourceColumnDialog.ui \
    DataSourceConnectParametersDialog.ui \
    DbPostgresDataSourceConnectWidget.ui \
    DbSqliteDataSourceConnectWidget.ui \
    ReportDataTableDialog.ui \
    DbLinterDataSourceConnectWidget.ui
