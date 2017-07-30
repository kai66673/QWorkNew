include (../../../../common.prf)

QT += sql xml

TEMPLATE = lib
TARGET = sqleditor
CONFIG += plugin

DEFINES += SQLEDITOR_LIBRARY

INCLUDEPATH += ../../../../libs
INCLUDEPATH += ../../../../libs/utils
INCLUDEPATH += ../../../../libs/texteditorlib
INCLUDEPATH += ../../../../libs/texteditorlib/codeassist
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/sqlengine

DESTDIR = ../../$${TARGET_DIR}/editors
LIBS += -L../../$${TARGET_DIR}

LIBS += -lsettings
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}
LIBS += -lsqlengine
LIBS += -lcore
LIBS += -lqtcutils
LIBS += -ltexteditor

HEADERS += \
    SQLEditor.h \
    sqleditorconstants.h \
    SqlEditorPlugin.h \
    SqlSemanticHighlighter.h \
    SqlCheckNames.h \
    SqlSemanticInfo.h \
    SqlHoverHandler.h \
    SqlCompletiotionAssist.h

SOURCES += \
    SQLEditor.cpp \
    SqlEditorPlugin.cpp \
    SqlSemanticHighlighter.cpp \
    SqlCheckNames.cpp \
    SqlSemanticInfo.cpp \
    SqlHoverHandler.cpp \
    SqlCompletiotionAssist.cpp

FORMS +=

RESOURCES += \
    sqleditor.qrc



























