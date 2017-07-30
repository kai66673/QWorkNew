include (../../../../common.prf)

TARGET = cmakeeditor
TEMPLATE = lib
CONFIG += plugin

QT += xml

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs
INCLUDEPATH += ../../../../libs/utils
INCLUDEPATH += ../../../../libs/texteditorlib
INCLUDEPATH += ../../../../libs/texteditorlib/generichighlighter
INCLUDEPATH += ../../../../libs/texteditorlib/codeassist
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/corelib/CodeModel
INCLUDEPATH += ../../../../libs/settings

DESTDIR = ../../$${TARGET_DIR}/editors
LIBS += -L../../$${TARGET_DIR}

LIBS += -lsettings
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}
LIBS += -lcore
LIBS += -lqtcutils
LIBS += -ltexteditor

DEFINES += CMAKEEDITOR_LIBRARY

SOURCES += CmakeEditorPlugin.cpp \
    CmakeEditor.cpp \
    CmakeEditorWidget.cpp \
    CmakeCompletionAssistProvider.cpp

HEADERS += CmakeEditorPlugin.h\
        cmakeeditor_global.h \
    CmakeEditor.h \
    CmakeEditorWidget.h \
    cmakeeditorconstants.h \
    CmakeCompletionAssistProvider.h
