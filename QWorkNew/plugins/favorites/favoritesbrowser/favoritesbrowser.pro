include (../../../../common.prf)

QT += xml
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/utils

TARGET = favorites

DEFINES += FAVORITES_LIBRARY

DESTDIR = ../../$${TARGET_DIR}/plugins
LIBS += -L../../$${TARGET_DIR}

LIBS += -lsettings
LIBS += -lcore
LIBS += -lqtcutils

HEADERS += FavoritesPlugin.h \
    IFavorite.h \
    FavoriteModel.h \
    FavoriteFactoriesManager.h \
    FavoritesTreeView.h \
    SelectFavoriteFactoryDialog.h \
    favorites_global.h

SOURCES += FavoritesPlugin.cpp \
    IFavorite.cpp \
    FavoriteModel.cpp \
    FavoriteFactoriesManager.cpp \
    FavoritesTreeView.cpp \
    SelectFavoriteFactoryDialog.cpp

RESOURCES += \
    favoritesbrowser.qrc

FORMS += \
    SelectFavoriteFactoryDialog.ui
