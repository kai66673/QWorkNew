include (../../../../common.prf)

TARGET = profileeditor
TEMPLATE = lib
CONFIG += plugin

QT += xml

DEFINES += PROFILEEDITOR_LIBRARY

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs
INCLUDEPATH += ../../../../libs/utils
INCLUDEPATH += ../../../../libs/texteditorlib
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

SOURCES += ProfileEditorPlugin.cpp \
    ProfileEditorWidget.cpp \
    ProfileEditor.cpp \
    profilehighlighter.cpp \
    profilecompletionassist.cpp

HEADERS += ProfileEditorPlugin.h\
        profileeditor_global.h \
    ProfileEditorWidget.h \
    ProfileEditor.h \
    profileeditorconstants.h \
    profilehighlighter.h \
    profilecompletionassist.h

RESOURCES += \
    profileeditor.qrc

FORMS +=
