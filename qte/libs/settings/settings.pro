include (../../common.prf)

TEMPLATE = lib
TARGET = settings

DEFINES += SETTINGS_LIBRARY

INCLUDEPATH += .

DESTDIR = $${TARGET_DIR}

HEADERS += SettingsManager.h \
    SettingsDialog.h \
    settings_global.h \
    ISettingsClient.h

SOURCES += SettingsManager.cpp \
    SettingsDialog.cpp \
    ISettingsClient.cpp
