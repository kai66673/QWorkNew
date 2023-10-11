cmake_minimum_required(VERSION 3.5)

project(qtecore VERSION 0.1 LANGUAGES CXX)

set (QTECORE_SOURCES
    textfile.cpp
    styleanimator.cpp
    mimedatabase.cpp
    manhattanstyle.cpp
    infobar.cpp
    ifile.cpp
    ieditor.cpp
    progressview.cpp
    progressbar.cpp
    futureprogress.cpp
    completionassistprovider.cpp
    iassistprovider.cpp
    ifindsupport.cpp
    SessionsDialog.cpp
    SessionManager.cpp
    ISessionClient.cpp
    LogManager.cpp
    LogFilterDialog.cpp
    CommandManager.cpp
    CommandShortcutsDialog.cpp
    CommandsTreeModel.cpp
    CloseApplicationHandler.cpp
    IDocument.cpp
    ISettingsFactory.cpp
    OpenEditorsWindow.cpp
    ITextDocument.cpp
    Core.cpp
    ILink.cpp
    IPythonModule.cpp
    itexteditor.cpp
    fileiconprovider.cpp
    id.cpp
    featureprovider.cpp
    outputwindow.cpp
    variablechooser.cpp
    CodeModel/ICodeModel.cpp
    CodeModel/CodeModelManager.cpp
    CodeModel/abstracteditorsupport.cpp
    IChangeProjectFileWatcher.cpp
    WizardManager.cpp
    iwizardfactory.cpp
    newdialog.cpp
    basefilewizardfactory.cpp
    generatedfile.cpp
    promptoverwritedialog.cpp
    basefilewizard.cpp
    jsexpander.cpp
    corejsextensions.cpp
    PythonQtObjectStorage.cpp
    ModeManager.cpp
    IGuiFactory.cpp
    DocumentToolBar.cpp
    HighlighterFormatsManager.cpp
    HighlighterFormatsConfigureWidget.cpp
    MainToolBar.cpp
    simplemainwindow.cpp
)

set (QTECORE_HEADERS
    textfile.h
    styleanimator.h
    mimedatabase.h
    manhattanstyle.h
    ioutputpane.h
    infobar.h
    ifile.h
    ieditor.h
    icontext.h
    coreconstants.h
    corelib_global.h
    progressview.h
    progressmanager.h
    progressbar.h
    futureprogress.h
    iassistprovider.h
    completionassistprovider.h
    ifindsupport.h
    textfindconstants.h
    SessionsDialog.h
    SessionManager.h
    ISessionClient.h
    IMainWindow.h
    LogManager.h
    LogFilterDialog.h
    CommandManager.h
    CommandShortcutsDialog.h
    CommandsTreeModel.h
    CloseApplicationHandler.h
    IPlugin.h
    IDocument.h
    ISettingsFactory.h
    OpenEditorsWindow.h
    ITextDocument.h
    Core.h
    ILink.h
    IPythonModule.h
    itexteditor.h
    fileiconprovider.h
    id.h
    featureprovider.h
    outputwindow.h
    variablechooser.h
    CodeModel/ICodeModel.h
    CodeModel/CodeModelManager.h
    CodeModel/abstracteditorsupport.h
    IChangeProjectFileWatcher.h
    WizardManager.h
    iwizardfactory.h
    newdialog.h
    basefilewizardfactory.h
    generatedfile.h
    promptoverwritedialog.h
    ifilewizardextension.h
    basefilewizard.h
    jsexpander.h
    corejsextensions.h
    PythonQtObjectStorage.h
    ModeManager.h
    IGuiFactory.h
    DocumentToolBar.h
    HighlighterFormatsManager.h
    HighlighterFormatsConfigureWidget.h
    MainToolBar.h
    simplemainwindow.h
)

set (QTECORE_FORMS
    SessionsDialog.ui
    LogFilterDialog.ui
    CommandShortcutsDialog.ui
    newdialog.ui
    HighlighterFormatsConfigureWidget.ui
)

add_library(${PROJECT_NAME} SHARED
    ${QTECORE_SOURCES}
    ${QTECORE_HEADERS}
    ${QTECORE_FORMS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/CodeModel
    ${CMAKE_CURRENT_LIST_DIR}/../utils
    ${CMAKE_CURRENT_LIST_DIR}/../settings
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    CORELIB_LIBRARY
    QMAKE_AS_LIBRARY
    PROPARSER_THREAD_SAFE
    PROEVALUATOR_THREAD_SAFE
    PROEVALUATOR_CUMULATIVE
    PROEVALUATOR_SETENV
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    qteutils
    qtesettings
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Script
    Qt${QT_VERSION_MAJOR}::Xml
)
