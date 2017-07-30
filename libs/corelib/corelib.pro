include (../../common.prf)

QT += xml network script
TEMPLATE = lib
TARGET = core

DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += ./CodeModel
INCLUDEPATH += ../utils
INCLUDEPATH += ../settings
INCLUDEPATH += ../Qt4Concurrent

DEFINES += CORELIB_LIBRARY

INCLUDEPATH += shared

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lqtcutils
LIBS += -lsettings

HEADERS += \
    textfile.h \
    styleanimator.h \
    mimedatabase.h \
    manhattanstyle.h \
    ioutputpane.h \
    infobar.h \
    ifile.h \
    ieditor.h \
    icontext.h \
    coreconstants.h \
    corelib_global.h \
    progressview.h \
    progressmanager.h \
    progressbar.h \
    futureprogress.h \
    iassistprovider.h \
    completionassistprovider.h \
    ifindsupport.h \
    textfindconstants.h \
    SessionsDialog.h \
    SessionManager.h \
    ISessionClient.h \
    IMainWindow.h \
    LogManager.h \
    LogFilterDialog.h \
    CommandManager.h \
    CommandShortcutsDialog.h \
    CommandsTreeModel.h \
    CloseApplicationHandler.h \
    IPlugin.h \
    IDocument.h \
    ISettingsFactory.h \
    OpenEditorsWindow.h \
    ITextDocument.h \
    Core.h \
    ILink.h \
    IPythonModule.h \
    itexteditor.h \
    fileiconprovider.h \
    id.h \
    featureprovider.h \
    outputwindow.h \
    variablechooser.h \
    CodeModel/ICodeModel.h \
    CodeModel/CodeModelManager.h \
    CodeModel/abstracteditorsupport.h \
    IChangeProjectFileWatcher.h \
    WizardManager.h \
    iwizardfactory.h \
    newdialog.h \
    basefilewizardfactory.h \
    generatedfile.h \
    promptoverwritedialog.h \
    ifilewizardextension.h \
    basefilewizard.h \
    jsexpander.h \
    corejsextensions.h \
    PythonQtObjectStorage.h \
    ModeManager.h \
    IGuiFactory.h \
    DocumentToolBar.h \
    HighlighterFormatsManager.h \
    HighlighterFormatsConfigureWidget.h \
    MainToolBar.h

SOURCES += \
    textfile.cpp \
    styleanimator.cpp \
    mimedatabase.cpp \
    manhattanstyle.cpp \
    infobar.cpp \
    ifile.cpp \
    ieditor.cpp \
    progressview.cpp \
    progressbar.cpp \
    futureprogress.cpp \
    completionassistprovider.cpp \
    iassistprovider.cpp \
    ifindsupport.cpp \
    SessionsDialog.cpp \
    SessionManager.cpp \
    ISessionClient.cpp \
    LogManager.cpp \
    LogFilterDialog.cpp \
    CommandManager.cpp \
    CommandShortcutsDialog.cpp \
    CommandsTreeModel.cpp \
    CloseApplicationHandler.cpp \
    IDocument.cpp \
    ISettingsFactory.cpp \
    OpenEditorsWindow.cpp \
    ITextDocument.cpp \
    Core.cpp \
    ILink.cpp \
    IPythonModule.cpp \
    itexteditor.cpp \
    fileiconprovider.cpp \
    id.cpp \
    featureprovider.cpp \
    outputwindow.cpp \
    variablechooser.cpp \
    CodeModel/ICodeModel.cpp \
    CodeModel/CodeModelManager.cpp \
    CodeModel/abstracteditorsupport.cpp \
    IChangeProjectFileWatcher.cpp \
    WizardManager.cpp \
    iwizardfactory.cpp \
    newdialog.cpp \
    basefilewizardfactory.cpp \
    generatedfile.cpp \
    promptoverwritedialog.cpp \
    basefilewizard.cpp \
    jsexpander.cpp \
    corejsextensions.cpp \
    PythonQtObjectStorage.cpp \
    ModeManager.cpp \
    IGuiFactory.cpp \
    DocumentToolBar.cpp \
    HighlighterFormatsManager.cpp \
    HighlighterFormatsConfigureWidget.cpp \
    MainToolBar.cpp

RESOURCES += \
    corelib.qrc

FORMS += \
    SessionsDialog.ui \
    LogFilterDialog.ui \
    CommandShortcutsDialog.ui \
    newdialog.ui \
    HighlighterFormatsConfigureWidget.ui

# Some way to override the architecture used in Abi:
!isEmpty($$(QTC_CPU)) {
    DEFINES += QTC_CPU=$$(QTC_CPU)
} else {
    DEFINES += QTC_CPU=X86Architecture
}

DEFINES *= \
    QMAKE_AS_LIBRARY \
    PROPARSER_THREAD_SAFE \
    PROEVALUATOR_THREAD_SAFE \
    PROEVALUATOR_CUMULATIVE \
    PROEVALUATOR_SETENV

DISTFILES += \
    qss/DocumentToolBar.qss \
    qss/MainToolBar.qss

qss.path = $${DESTDIR}/qss
qss.files = qss/*.qss
INSTALLS += qss
