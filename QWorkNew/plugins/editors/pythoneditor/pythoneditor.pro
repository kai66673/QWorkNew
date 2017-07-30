include (../../../../common.prf)

TEMPLATE = lib
TARGET = pythoneditor
CONFIG += plugin

QT += xml

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs/texteditorlib
INCLUDEPATH += ../../../../libs/texteditorlib/codeassist
INCLUDEPATH += ../../../../libs/pythoneditorlib
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/utils

DESTDIR = ../../$${TARGET_DIR}/editors
LIBS += -L../../$${TARGET_DIR}

LIBS += -lsettings
LIBS += -lcore
LIBS += -lqtcutils
LIBS += -ltexteditor
LIBS += -lpythoneditorlib

HEADERS += \
    PythonEditorPlugin.h \
    PythonCompletiotionAssist.h \
    PythonSnippetProvider.h

SOURCES += \
    PythonEditorPlugin.cpp \
    PythonCompletiotionAssist.cpp \
    PythonSnippetProvider.cpp

FORMS +=

RESOURCES += \
    pythoneditor.qrc

