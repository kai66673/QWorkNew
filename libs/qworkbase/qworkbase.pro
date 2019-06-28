#-------------------------------------------------
#
# Project created by QtCreator 2011-01-17T18:52:24
#
#-------------------------------------------------
include (../../common.prf)

QT += network xml gui

TARGET = QWorkBase
TEMPLATE = lib

DEFINES += QWORKBASE_LIBRARY

INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += ../utils
INCLUDEPATH += ../settings
INCLUDEPATH += ../corelib
INCLUDEPATH += ../corelib/CodeModel
INCLUDEPATH += ../ProjectExplorer
INCLUDEPATH += ../ProjectExplorer/devicesupport
INCLUDEPATH += ../ProjectExplorer/options
INCLUDEPATH += ../ProjectExplorer/qtsupport
INCLUDEPATH += ../texteditorlib
INCLUDEPATH += ../find
INCLUDEPATH += ../cplusplus
INCLUDEPATH += ../cplusplus/cpptools
INCLUDEPATH += ../cplusplus/shared
INCLUDEPATH += ./texteditor
INCLUDEPATH += ./texteditor/bookmarks
INCLUDEPATH += ./progressmanager
INCLUDEPATH += ./scripting

include(../$${PYTHONQTVERDIR}/build/python.prf)
INCLUDEPATH += ../$${PYTHONQTVERDIR}/src
INCLUDEPATH += ../$${PYTHONQTVERDIR}/extensions/PythonQt_QtAll

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lPythonQt
LIBS += -lPythonQt_QtAll

LIBS += -lsettings
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}

LIBS += -lcore
LIBS += -lqtcutils
LIBS += -laggregate
LIBS += -lfind
LIBS += -ltexteditor
LIBS += -lcplusplus
LIBS += -lProjectExplorer

SOURCES += QWorkBase.cpp \
    ListSelectorDialog.cpp \
    TextPositionStatusBarWidget.cpp \
    GotoLineDialog.cpp \
    NavigationManager.cpp \
    progressmanager/progressmanager.cpp \
    TasksStatusWidget.cpp \
    texteditor/bookmarks/BookmarkPlugin.cpp \
    texteditor/bookmarks/bookmarkmanager.cpp \
    texteditor/bookmarks/bookmark.cpp \
    texteditor/basetextmark.cpp \
    scripting/QwGuiPythonModule.cpp \
    scripting/PythonScriptManager.cpp \
    scripting/ConfirmRemoveScriptDialog.cpp \
    scripting/AddPythonScriptDialog.cpp \
    DocumentManagerPythonModule.cpp \
    DockCornerSettingsDialog.cpp \
    SnippetsDockWidget.cpp

HEADERS += QWorkBase.h\
    QWorkBase_global.h \
    ListSelectorDialog.h \
    TextPositionStatusBarWidget.h \
    GotoLineDialog.h \
    NavigationManager.h \
    progressmanager/progressmanager_p.h \
    TasksStatusWidget.h \
    texteditor/bookmarks/BookmarkPlugin.h \
    texteditor/bookmarks/bookmarkmanager.h \
    texteditor/bookmarks/bookmark.h \
    texteditor/basetextmark.h \
    scripting/QwGuiPythonModule.h \
    scripting/PythonScriptManager.h \
    scripting/ConfirmRemoveScriptDialog.h \
    scripting/AddPythonScriptDialog.h \
    DocumentManagerPythonModule.h \
    DockCornerSettingsDialog.h \
    SnippetsDockWidget.h

FORMS += \
    ListSelectorDialog.ui \
    TextPositionStatusBarWidget.ui \
    GotoLineDialog.ui \
    scripting/ConfirmRemoveScriptDialog.ui \
    scripting/AddPythonScriptDialog.ui \
    DockCornerSettingsDialog.ui

RESOURCES += \
    QWorkBase.qrc

win32 {
    SOURCES += progressmanager/progressmanager_win.cpp
    LIBS += -lole32
}
else:macx {
    OBJECTIVE_SOURCES += progressmanager/progressmanager_mac.mm
    LIBS += -framework AppKit
}
else:unix {
    SOURCES += progressmanager/progressmanager_x11.cpp
}

OTHER_FILES += \
    scripting/initPythonCodeAssist.py


rope.path = $${DESTDIR}/python/Lib
rope.files = scripting/$${ROPE_DIR}/rope/
INSTALLS += rope

qss.path = $${DESTDIR}/qss
qss.files = qss/*.qss
INSTALLS += qss

DISTFILES += \
    qss/TabWidgetSplitterHolder.qss \
    qss/ContainerTabBar.qss
