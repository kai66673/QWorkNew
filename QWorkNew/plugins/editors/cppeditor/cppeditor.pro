include (../../../../common.prf)

TEMPLATE = lib
TARGET = cppeditor
CONFIG += plugin

QT += xml network

DEFINES += CPPEDITOR_LIBRARY

INCLUDEPATH += .
INCLUDEPATH += ../../../../libs
INCLUDEPATH += ../../../../libs/utils
INCLUDEPATH += ../../../../libs/cplusplus
INCLUDEPATH += ../../../../libs/cplusplus/shared
INCLUDEPATH += ../../../../libs/cplusplus/cpptools
INCLUDEPATH += ../../../../libs/texteditorlib
INCLUDEPATH += ../../../../libs/texteditorlib/codeassist
INCLUDEPATH += ../../../../libs/corelib
INCLUDEPATH += ../../../../libs/corelib/CodeModel
INCLUDEPATH += ../../../../libs/ProjectExplorer
INCLUDEPATH += ../../../../libs/ProjectExplorer/devicesupport
INCLUDEPATH += ../../../../libs/ProjectExplorer/options
INCLUDEPATH += ../../../../libs/ProjectExplorer/qtsupport
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/find
INCLUDEPATH += ../../../../libs/find/locator

DESTDIR = ../../$${TARGET_DIR}/editors
LIBS += -L../../$${TARGET_DIR}

LIBS += -lcplusplus
LIBS += -lsettings
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += concurrent
}
LIBS += -lcore
LIBS += -lfind
LIBS += -lqtcutils
LIBS += -ltexteditor
LIBS += -lProjectExplorer

HEADERS += \
    cppeditor.h \
    cpphighlighter.h \
    cppeditorenums.h \
    cppeditor_global.h \
    cppquickfix.h \
    cppoutline.h \
    cppinsertdecldef.h \
    cppcompleteswitch.h \
    cppinsertqtpropertymembers.h \
    CppEditorPlugin.h \
    CppEditorSettingsWidget.h \
    cppquickfixassistant.h \
    cppsnippetprovider.h

SOURCES += \
    cppeditor.cpp \
    cpphighlighter.cpp \
    cppquickfix.cpp \
    cppquickfixes.cpp \
    cppoutline.cpp \
    cppinsertdecldef.cpp \
    cppcompleteswitch.cpp \
    cppinsertqtpropertymembers.cpp \
    CppEditorPlugin.cpp \
    CppEditorSettingsWidget.cpp \
    cppquickfixassistant.cpp \
    cppsnippetprovider.cpp

RESOURCES += cppeditor.qrc
OTHER_FILES += CppEditor.mimetypes.xml

FORMS += \
    CppEditorSettingsWidget.ui




































