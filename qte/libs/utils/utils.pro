include (../../common.prf)

QT += network xml sql

TEMPLATE = lib
TARGET = qtcutils

DEFINES += QTCREATOR_UTILS_LIB

win32: LIBS += -luser32 -lshell32

INCLUDEPATH += .
INCLUDEPATH += ../Qt4Concurrent

DESTDIR = $${TARGET_DIR}

win32:SOURCES += $$PWD/consoleprocess_win.cpp
else:SOURCES += $$PWD/consoleprocess_unix.cpp

OTHER_FILES +=

HEADERS += \
    uncommentselection.h \
    textfileformat.h \
    stylehelper.h \
    stringutils.h \
    settingsutils.h \
    savefile.h \
    reloadpromptutils.h \
    qtcprocess.h \
    qtcassert.h \
    persistentsettings.h \
    networkaccessmanager.h \
    navigationtreeview.h \
    linecolumnlabel.h \
    htmldocextractor.h \
    filesearch.h \
    fancymainwindow.h \
    faketooltip.h \
    environment.h \
    changeset.h \
    utils_global.h \
    RecentFilesMenu.h \
    PageFormat.h \
    QwDialog.h \
    QwDockWidget.h \
    QwTableView.h \
    SqlQueryModel.h \
    hostosinfo.h \
    osspecificaspects.h \
    qtcoverride.h \
    IssuesView.h \
    scopedswap.h \
    macroexpander.h \
    outputformat.h \
    outputformatter.h \
    ansiescapecodehandler.h \
    theme/theme.h \
    theme/theme_p.h \
    detailsbutton.h \
    detailswidget.h \
    environmentmodel.h \
    headerviewstretcher.h \
    QtConcurrentTools \
    synchronousprocess.h \
    itemviews.h \
    fancylineedit.h \
    pathchooser.h \
    completinglineedit.h \
    execmenu.h \
    historycompleter.h \
    portlist.h \
    treemodel.h \
    checkablemessagebox.h \
    winutils.h \
    buildablehelperlibrary.h \
    filesystemwatcher.h \
    consoleprocess.h \
    consoleprocess_p.h \
    wizard.h \
    wizardpage.h \
    parameteraction.h \
    fileinprojectfinder.h \
    unixutils.h \
    filewizardpage.h \
    filenamevalidatinglineedit.h \
    projectintropage.h \
    projectnamevalidatinglineedit.h \
    textfieldcheckbox.h \
    textfieldcombobox.h \
    treeviewcombobox.h \
    codegeneration.h \
    newclasswidget.h \
    classnamevalidatinglineedit.h \
    DesignerDockTitle.h \
    styledbar.h \
    qtcfileutils.h \
    StyleManager.h \
    DiagnosticMessage.h \
    MemoryPool.h

SOURCES += \
    uncommentselection.cpp \
    textfileformat.cpp \
    stylehelper.cpp \
    stringutils.cpp \
    savefile.cpp \
    reloadpromptutils.cpp \
    qtcprocess.cpp \
    persistentsettings.cpp \
    networkaccessmanager.cpp \
    navigationtreeview.cpp \
    linecolumnlabel.cpp \
    htmldocextractor.cpp \
    filesearch.cpp \
    fancymainwindow.cpp \
    faketooltip.cpp \
    environment.cpp \
    changeset.cpp \
    RecentFilesMenu.cpp \
    PageFormat.cpp \
    QwDialog.cpp \
    QwTableView.cpp \
    QwDockWidget.cpp \
    SqlQueryModel.cpp \
    hostosinfo.cpp \
    IssuesView.cpp \
    macroexpander.cpp \
    outputformatter.cpp \
    ansiescapecodehandler.cpp \
    theme/theme.cpp \
    qtcassert.cpp \
    detailsbutton.cpp \
    detailswidget.cpp \
    environmentmodel.cpp \
    headerviewstretcher.cpp \
    synchronousprocess.cpp \
    itemviews.cpp \
    fancylineedit.cpp \
    pathchooser.cpp \
    completinglineedit.cpp \
    execmenu.cpp \
    historycompleter.cpp \
    portlist.cpp \
    treemodel.cpp \
    checkablemessagebox.cpp \
    winutils.cpp \
    buildablehelperlibrary.cpp \
    filesystemwatcher.cpp \
    consoleprocess.cpp \
    wizard.cpp \
    wizardpage.cpp \
    parameteraction.cpp \
    fileinprojectfinder.cpp \
    unixutils.cpp \
    filewizardpage.cpp \
    filenamevalidatinglineedit.cpp \
    projectintropage.cpp \
    projectnamevalidatinglineedit.cpp \
    textfieldcheckbox.cpp \
    textfieldcombobox.cpp \
    treeviewcombobox.cpp \
    codegeneration.cpp \
    newclasswidget.cpp \
    classnamevalidatinglineedit.cpp \
    DesignerDockTitle.cpp \
    styledbar.cpp \
    qtcfileutils.cpp \
    StyleManager.cpp \
    DiagnosticMessage.cpp \
    MemoryPool.cpp

FORMS += \
    filewizardpage.ui \
    projectintropage.ui \
    newclasswidget.ui \
    DesignerDockTitle.ui

RESOURCES += \
    utils.qrc

qss.path = $${DESTDIR}/qss
qss.files = qss/*.qss
INSTALLS += qss

DISTFILES += \
    qss/QwTabBar.qss \
    qss/QwTitleBar.qss \
    qss/QwTabCornerToolBar.qss

