include (../../../../common.prf)

QT += sql xml
TARGET = dbbrowser
TEMPLATE = lib
CONFIG += plugin

DEFINES += DBBROWSER_LIBRARY

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/texteditorlib
INCLUDEPATH += ../../../../libs/texteditorlib/codeassist
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/sqlengine
INCLUDEPATH += ../../../../libs/utils

DESTDIR = ../../$${TARGET_DIR}/plugins
LIBS += -L../../$${TARGET_DIR}

LIBS += -lsettings
LIBS += -lsqlengine
LIBS += -lcore
LIBS += -ltexteditor
LIBS += -lqtcutils

RESOURCES += dbbrowser.qrc

HEADERS += \
    DbManager.h \
    DbConnectionsHistoryDialog.h \
    DbConnectionsHistory.h \
    QSqlHistoryModel.h \
    QSqlHistoryTableView.h \
    SqlHistoryFilterDialog.h \
    QueryViewSettingsWidget.h \
    ConnectDialog.h \
    QDbStructWidget.h \
    DbStructTreeView.h \
    QRdbQueryModel.h \
    DbQueryModel.h \
    DbFilterDialog.h \
    DbDetailsWidget.h \
    dbbrowser_constants.h \
    dbbrowser_global.h

SOURCES += \
    DbManager.cpp \
    DbConnectionsHistoryDialog.cpp \
    DbConnectionsHistory.cpp \
    QSqlHistoryModel.cpp \
    QSqlHistoryTableView.cpp \
    SqlHistoryFilterDialog.cpp \
    QueryViewSettingsWidget.cpp \
    ConnectDialog.cpp \
    QDbStructWidget.cpp \
    DbStructTreeView.cpp \
    QRdbQueryModel.cpp \
    DbQueryModel.cpp \
    DbFilterDialog.cpp \
    DbDetailsWidget.cpp

FORMS += \
    DbConnectionsHistoryDialog.ui \
    SqlHistoryFilterDialog.ui \
    QueryViewSettingsWidget.ui \
    ConnectDialog.ui \
    DbFilterDialog.ui

qss.path = ../../$${TARGET_DIR}/qss
qss.files = qss/*.qss
INSTALLS += qss

DISTFILES += \
    qss/DbDetailsTabBar.qss
