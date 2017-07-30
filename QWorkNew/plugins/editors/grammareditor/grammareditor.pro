include (../../../../common.prf)

TEMPLATE = lib
TARGET = grammareditor
CONFIG += plugin

QT += xml

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs/texteditorlib
INCLUDEPATH += ../../../../libs/texteditorlib/codeassist
INCLUDEPATH += ../../../../libs/pythoneditorlib
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/corelib/CodeModel
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/utils
INCLUDEPATH += ../../../../libs/find
INCLUDEPATH += ../../../../libs/ProjectExplorer
INCLUDEPATH += ../../../../libs/cplusplus
INCLUDEPATH += ../../../../libs/cplusplus/shared
INCLUDEPATH += ../../../../libs/cplusplus/cpptools


DESTDIR = ../../$${TARGET_DIR}/editors
LIBS += -L../../$${TARGET_DIR}

LIBS += -lsettings
LIBS += -lcplusplus
LIBS += -lcore
LIBS += -lqtcutils
LIBS += -ltexteditor
LIBS += -lfind

SOURCES += \
    GrammarEditorPlugin.cpp \
    GrammarEditorWidget.cpp \
    GrammarHighlighter.cpp \
    GrammarLexer.cpp \
    GrammarEditorSettingsWidget.cpp \
    GrammarRulesModel.cpp \
    GrammarRunManager.cpp

HEADERS += \
    GrammarEditorPlugin.h \
    GrammarEditorWidget.h \
    GrammarHighlighter.h \
    GrammarLexer.h \
    GrammarEditorSettingsWidget.h \
    GrammarRulesModel.h \
    GrammarRunManager.h

FORMS += \
    GrammarEditorSettingsWidget.ui

RESOURCES += \
    grammareditor.qrc
