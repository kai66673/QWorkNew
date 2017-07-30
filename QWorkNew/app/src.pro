include (../../common.prf)

QT       += core gui xml

TARGET = QWorkIde
TEMPLATE = app

INCLUDEPATH += ../../libs/qworkbase
INCLUDEPATH += ../../libs/settings
INCLUDEPATH += ../../libs/corelib
INCLUDEPATH += ../../libs/utils

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lQWorkBase
LIBS += -lsettings
LIBS += -lcore
LIBS += -ltexteditor
LIBS += -lqtcutils
LIBS += -laggregate
LIBS += -lProjectExplorer
LIBS += -lssh
LIBS += -lfind
LIBS += -lPythonQt
LIBS += -lPythonQt_QtAll
LIBS += -lcplusplus

RC_FILE = QWorkIde.rc

SOURCES += main.cpp

HEADERS  +=
