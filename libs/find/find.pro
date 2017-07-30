include (../../common.prf)

QT += network xml
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}

TARGET = find
TEMPLATE = lib

DEFINES += FIND_LIBRARY

INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += ./locator
INCLUDEPATH += ../utils
INCLUDEPATH += ../Qt4Concurrent
INCLUDEPATH += ../corelib
INCLUDEPATH += ../settings

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lcore
LIBS += -lqtcutils
LIBS += -lsettings
LIBS += -laggregate

SOURCES += ifindfilter.cpp \
    currentdocumentfind.cpp \
    SearchEngine.cpp \
    SearchResultTabWidget.cpp \
    searchresulttreeitemdelegate.cpp \
    searchresulttreeitems.cpp \
    searchresulttreemodel.cpp \
    searchresulttreeview.cpp \
    searchresultwindow.cpp \
    basefilefind.cpp \
    BaseTextSearchParamsWidget.cpp \
    FindReplaceWidget.cpp \
    FindInDialog.cpp \
    SearchReplaceTextDialog.cpp \
    locator/ilocatorfilter.cpp \
    locator/locatorwidget.cpp \
    locator/LocatorManager.cpp \
    locator/locatorsearchutils.cpp \
    locator/LocatorSearchModePlugin.cpp \
    locator/filesystemfilter.cpp \
    locator/directoryfilter.cpp \
    locator/basefilefilter.cpp \
    locator/locatorfiltersfilter.cpp \
    locator/opendocumentsfilter.cpp

HEADERS += ifindfilter.h \
    currentdocumentfind.h \
    find_global.h \
    SearchEngine.h \
    SearchResultTabWidget.h \
    searchresulttreeitemdelegate.h \
    searchresulttreeitemroles.h \
    searchresulttreeitems.h \
    searchresulttreemodel.h \
    searchresulttreeview.h \
    searchresultwindow.h \
    basefilefind.h \
    BaseTextSearchParamsWidget.h \
    FindReplaceWidget.h \
    FindInDialog.h \
    SearchReplaceTextDialog.h \
    locator/ilocatorfilter.h \
    locator/locatorwidget.h \
    locator/LocatorManager.h \
    locator/locatorsearchutils.h \
    locator/LocatorSearchModePlugin.h \
    locator/filesystemfilter.h \
    locator/directoryfilter.h \
    locator/basefilefilter.h \
    locator/locatorfiltersfilter.h \
    locator/opendocumentsfilter.h

FORMS += \
    FindReplaceWidget.ui \
    FindInDialog.ui \
    SearchReplaceTextDialog.ui \
    locator/filesystemfilter.ui \
    locator/directoryfilter.ui

RESOURCES += \
    find.qrc

qss.path = $${DESTDIR}/qss
qss.files = qss/*.qss
INSTALLS += qss

DISTFILES += \
    qss/SearchTabWidget.qss
