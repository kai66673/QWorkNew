include (../../../../../common.prf)

QT           += sql xml
TEMPLATE      = lib
CONFIG       += plugin

INCLUDEPATH += ../../favoritesbrowser
INCLUDEPATH += ../../../../../libs
INCLUDEPATH += ../../../../../libs/settings
INCLUDEPATH += ../../../../../libs/corelib
INCLUDEPATH += ../../../../../libs/texteditorlib
INCLUDEPATH += ../../../../../libs/utils

TARGET = UrlFavorite

DESTDIR = ../../../$${TARGET_DIR}/favorites
LIBS += -L../../../$${TARGET_DIR}
LIBS += -L../../../$${TARGET_DIR}/plugins

LIBS += -lcore
LIBS += -ltexteditor
LIBS += -lfavorites
LIBS += -lqtcutils

HEADERS += UrlFavoriteFactory.h \
    NewUrlDialog.h

SOURCES += UrlFavoriteFactory.cpp \
    NewUrlDialog.cpp

FORMS += \
    NewUrlDialog.ui

RESOURCES += \
    urlfavorite.qrc

