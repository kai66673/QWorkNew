include (../../common.prf)

QT += xml network script designer designercomponents-private
TEMPLATE = lib
TARGET = ProjectExplorer

DEFINES += PROJECTEXPLORER_LIBRARY
DEFINES += QTSUPPORT_LIBRARY
DEFINES += QMAKE_LIBRARY
DEFINES += RESOURCE_LIBRARY
DEFINES += CPP_ENABLED

DEFINES += NOMINMAX

INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += ./options
INCLUDEPATH += ./qtsupport
INCLUDEPATH += ./qtsupport/proparser
INCLUDEPATH += ./devicesupport
INCLUDEPATH += ../utils
INCLUDEPATH += ../ssh
INCLUDEPATH += ../settings
INCLUDEPATH += ../corelib
INCLUDEPATH += ../corelib/CodeModel
INCLUDEPATH += ../Qt4Concurrent

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lqtcutils
LIBS += -lsettings
LIBS += -lcore
LIBS += -lssh

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

HEADERS += \
    devicesupport/desktopdevice.h \
    devicesupport/desktopdeviceconfigurationwidget.h \
    devicesupport/desktopdevicefactory.h \
    devicesupport/desktopdeviceprocess.h \
    devicesupport/desktopprocesssignaloperation.h \
    devicesupport/deviceapplicationrunner.h \
    devicesupport/devicecheckbuildstep.h \
    devicesupport/devicefactoryselectiondialog.h \
    devicesupport/devicemanager.h \
    devicesupport/devicemanagermodel.h \
    devicesupport/deviceprocess.h \
    devicesupport/deviceprocessesdialog.h \
    devicesupport/deviceprocesslist.h \
    devicesupport/devicesettingspage.h \
    devicesupport/devicesettingswidget.h \
    devicesupport/devicetestdialog.h \
    devicesupport/devicetypekitchooser.h \
    devicesupport/deviceusedportsgatherer.h \
    devicesupport/idevice.h \
    devicesupport/idevicefactory.h \
    devicesupport/idevicewidget.h \
    devicesupport/localprocesslist.h \
    devicesupport/sshdeviceprocess.h \
    devicesupport/sshdeviceprocesslist.h \
    options/ioptionspage.h \
    options/kitoptionspage.h \
    options/projectexplorersettingspage.h \
    options/settingsdialog.h \
    options/toolchainconfigwidget.h \
    options/toolchainoptionspage.h \
    qtsupport/proparser/ioutils.h \
    qtsupport/proparser/profileevaluator.h \
    qtsupport/proparser/proitems.h \
    qtsupport/proparser/prowriter.h \
    qtsupport/proparser/qmake_global.h \
    qtsupport/proparser/qmakeevaluator.h \
    qtsupport/proparser/qmakeevaluator_p.h \
    qtsupport/proparser/qmakeglobals.h \
    qtsupport/proparser/qmakeparser.h \
    qtsupport/proparser/qmakevfs.h \
    qtsupport/baseqtversion.h \
    qtsupport/customexecutableconfigurationwidget.h \
    qtsupport/customexecutablerunconfiguration.h \
    qtsupport/debugginghelperbuildtask.h \
    qtsupport/desktopqtversion.h \
    qtsupport/desktopqtversionfactory.h \
    qtsupport/profilereader.h \
    qtsupport/qmldumptool.h \
    qtsupport/qtconfigwidget.h \
    qtsupport/qtkitconfigwidget.h \
    qtsupport/qtkitinformation.h \
    qtsupport/qtoptionspage.h \
    qtsupport/qtoutputformatter.h \
    qtsupport/qtparser.h \
    qtsupport/qtsupport_global.h \
    qtsupport/qtsupportconstants.h \
    qtsupport/qtversionfactory.h \
    qtsupport/qtversionmanager.h \
    qtsupport/simulatorqtversion.h \
    qtsupport/simulatorqtversionfactory.h \
    qtsupport/uicodemodelsupport.h \
    qtsupport/winceqtversion.h \
    qtsupport/winceqtversionfactory.h \
    qtsupport/cpp/formclasswizardpage.h \
    qtsupport/cpp/formclasswizarddialog.h \
    qtsupport/cpp/formclasswizard.h \
    qtsupport/cpp/formclasswizardparameters.h \
    resourceeditor/resource_global.h \
    resourceeditor/resourceeditorconstants.h \
    resourceeditor/resourcefile_p.h \
    resourceeditor/resourcenode.h \
    abi.h \
    abiwidget.h \
    abstractmsvctoolchain.h \
    abstractprocessstep.h \
    ansifilterparser.h \
    applicationlauncher.h \
    appoutputpane.h \
    buildconfiguration.h \
    buildconfigurationmodel.h \
    buildenvironmentwidget.h \
    buildinfo.h \
    buildmanager.h \
    buildprogress.h \
    buildsettingspropertiespage.h \
    buildstep.h \
    buildsteplist.h \
    buildstepspage.h \
    buildtargetinfo.h \
    clangparser.h \
    compileoutputwindow.h \
    ConfigureProjectsDialog.h \
    customparser.h \
    customparserconfigdialog.h \
    customtoolchain.h \
    dependenciespanel.h \
    deployablefile.h \
    deployconfiguration.h \
    deployconfigurationmodel.h \
    deploymentdata.h \
    doubletabwidget.h \
    environmentaspect.h \
    environmentaspectwidget.h \
    environmentitemswidget.h \
    environmentwidget.h \
    gccparser.h \
    gcctoolchain.h \
    gcctoolchainfactories.h \
    gnumakeparser.h \
    headerpath.h \
    importwidget.h \
    ioutputparser.h \
    ipotentialkit.h \
    iprojectprovider.h \
    itaskhandler.h \
    kit.h \
    kitchooser.h \
    kitconfigwidget.h \
    kitfeatureprovider.h \
    kitinformation.h \
    kitinformationconfigwidget.h \
    kitmanager.h \
    kitmanagerconfigwidget.h \
    kitmodel.h \
    ldparser.h \
    linuxiccparser.h \
    localapplicationrunconfiguration.h \
    localapplicationruncontrol.h \
    localenvironmentaspect.h \
    msvcparser.h \
    msvctoolchain.h \
    namedwidget.h \
    nodesvisitor.h \
    osparser.h \
    panelswidget.h \
    processparameters.h \
    processstep.h \
    project.h \
    projectconfiguration.h \
    projectexplorer_export.h \
    projectexplorerconstants.h \
    projectexplorersettings.h \
    projectimporter.h \
    projectmacroexpander.h \
    projectmanager.h \
    projectmodels.h \
    projectnodes.h \
    projectpanelfactory.h \
    projecttreewidget.h \
    projectwindow.h \
    propertiespanel.h \
    removefiledialog.h \
    runconfiguration.h \
    runconfigurationmodel.h \
    runsettingspropertiespage.h \
    selectablefilesmodel.h \
    SelectProjectsDialog.h \
    settingsaccessor.h \
    showineditortaskhandler.h \
    showoutputtaskhandler.h \
    target.h \
    targetselector.h \
    targetsettingspanel.h \
    targetsettingswidget.h \
    targetsetuppage.h \
    targetsetupwidget.h \
    task.h \
    taskhub.h \
    taskmodel.h \
    taskwindow.h \
    toolchain.h \
    toolchainmanager.h \
    unconfiguredprojectpanel.h \
    wincetoolchain.h \
    windebuginterface.h \
    jsonwizard/jsonfieldpage.h \
    jsonwizard/jsonfilepage.h \
    jsonwizard/jsonkitspage.h \
    jsonwizard/jsonprojectpage.h \
    jsonwizard/jsonsummarypage.h \
    jsonwizard/jsonwizard.h \
    jsonwizard/jsonwizardfactory.h \
    jsonwizard/jsonwizardfilegenerator.h \
    jsonwizard/jsonwizardgeneratorfactory.h \
    jsonwizard/jsonwizardpagefactory.h \
    jsonwizard/jsonwizardpagefactory_p.h \
    projectwizardpage.h \
    customwizard/customwizard.h \
    customwizard/customwizardpage.h \
    customwizard/customwizardparameters.h \
    customwizard/customwizardpreprocessor.h \
    customwizard/customwizardscriptgenerator.h \
    addnewmodel.h \
    baseprojectwizarddialog.h \
    cpptoolsjsextension.h \
    qtsupport/codegenerator.h \
    qtsupport/codegensettings.h \
    qtsupport/codegensettingspage.h \
    cppfilesettingspage.h \
    projectfilewizardextension.h \
    qtsupport/formtemplatewizardpage.h \
    qtsupport/QtDesignerFormClassCodeGenerator.h \
    ProjectManagerPythonModule.h \
    miniprojecttargetselector.h

SOURCES += \
    devicesupport/desktopdevice.cpp \
    devicesupport/desktopdeviceconfigurationwidget.cpp \
    devicesupport/desktopdevicefactory.cpp \
    devicesupport/desktopdeviceprocess.cpp \
    devicesupport/desktopprocesssignaloperation.cpp \
    devicesupport/deviceapplicationrunner.cpp \
    devicesupport/devicecheckbuildstep.cpp \
    devicesupport/devicefactoryselectiondialog.cpp \
    devicesupport/devicemanager.cpp \
    devicesupport/devicemanagermodel.cpp \
    devicesupport/deviceprocess.cpp \
    devicesupport/deviceprocessesdialog.cpp \
    devicesupport/deviceprocesslist.cpp \
    devicesupport/devicesettingspage.cpp \
    devicesupport/devicesettingswidget.cpp \
    devicesupport/devicetestdialog.cpp \
    devicesupport/devicetypekitchooser.cpp \
    devicesupport/deviceusedportsgatherer.cpp \
    devicesupport/idevice.cpp \
    devicesupport/idevicefactory.cpp \
    devicesupport/idevicewidget.cpp \
    devicesupport/localprocesslist.cpp \
    devicesupport/sshdeviceprocess.cpp \
    devicesupport/sshdeviceprocesslist.cpp \
    options/ioptionspage.cpp \
    options/kitoptionspage.cpp \
    options/projectexplorersettingspage.cpp \
    options/settingsdialog.cpp \
    options/toolchainconfigwidget.cpp \
    options/toolchainoptionspage.cpp \
    qtsupport/proparser/ioutils.cpp \
    qtsupport/proparser/profileevaluator.cpp \
    qtsupport/proparser/proitems.cpp \
    qtsupport/proparser/prowriter.cpp \
    qtsupport/proparser/qmakebuiltins.cpp \
    qtsupport/proparser/qmakeevaluator.cpp \
    qtsupport/proparser/qmakeglobals.cpp \
    qtsupport/proparser/qmakeparser.cpp \
    qtsupport/proparser/qmakevfs.cpp \
    qtsupport/baseqtversion.cpp \
    qtsupport/customexecutableconfigurationwidget.cpp \
    qtsupport/customexecutablerunconfiguration.cpp \
    qtsupport/debugginghelperbuildtask.cpp \
    qtsupport/desktopqtversion.cpp \
    qtsupport/desktopqtversionfactory.cpp \
    qtsupport/profilereader.cpp \
    qtsupport/qmldumptool.cpp \
    qtsupport/qtconfigwidget.cpp \
    qtsupport/qtkitconfigwidget.cpp \
    qtsupport/qtkitinformation.cpp \
    qtsupport/qtoptionspage.cpp \
    qtsupport/qtoutputformatter.cpp \
    qtsupport/qtparser.cpp \
    qtsupport/qtversionfactory.cpp \
    qtsupport/qtversionmanager.cpp \
    qtsupport/simulatorqtversion.cpp \
    qtsupport/simulatorqtversionfactory.cpp \
    qtsupport/uicodemodelsupport.cpp \
    qtsupport/winceqtversion.cpp \
    qtsupport/winceqtversionfactory.cpp \
    qtsupport/cpp/formclasswizardpage.cpp \
    qtsupport/cpp/formclasswizarddialog.cpp  \
    qtsupport/cpp/formclasswizard.cpp \
    qtsupport/cpp/formclasswizardparameters.cpp \
    resourceeditor/resourcefile.cpp \
    resourceeditor/resourcenode.cpp \
    abi.cpp \
    abiwidget.cpp \
    abstractmsvctoolchain.cpp \
    abstractprocessstep.cpp \
    ansifilterparser.cpp \
    applicationlauncher.cpp \
    appoutputpane.cpp \
    buildconfiguration.cpp \
    buildconfigurationmodel.cpp \
    buildenvironmentwidget.cpp \
    buildmanager.cpp \
    buildprogress.cpp \
    buildsettingspropertiespage.cpp \
    buildstep.cpp \
    buildsteplist.cpp \
    buildstepspage.cpp \
    clangparser.cpp \
    compileoutputwindow.cpp \
    ConfigureProjectsDialog.cpp \
    customparser.cpp \
    customparserconfigdialog.cpp \
    customtoolchain.cpp \
    dependenciespanel.cpp \
    deployablefile.cpp \
    deployconfiguration.cpp \
    deployconfigurationmodel.cpp \
    doubletabwidget.cpp \
    environmentaspect.cpp \
    environmentaspectwidget.cpp \
    environmentitemswidget.cpp \
    environmentwidget.cpp \
    gccparser.cpp \
    gcctoolchain.cpp \
    gnumakeparser.cpp \
    importwidget.cpp \
    ioutputparser.cpp \
    ipotentialkit.cpp \
    kit.cpp \
    kitchooser.cpp \
    kitconfigwidget.cpp \
    kitinformation.cpp \
    kitinformationconfigwidget.cpp \
    kitmanager.cpp \
    kitmanagerconfigwidget.cpp \
    kitmodel.cpp \
    ldparser.cpp \
    linuxiccparser.cpp \
    localapplicationrunconfiguration.cpp \
    localapplicationruncontrol.cpp \
    localenvironmentaspect.cpp \
    msvcparser.cpp \
    msvctoolchain.cpp \
    namedwidget.cpp \
    nodesvisitor.cpp \
    osparser.cpp \
    panelswidget.cpp \
    processparameters.cpp \
    processstep.cpp \
    project.cpp \
    projectconfiguration.cpp \
    projectimporter.cpp \
    projectmacroexpander.cpp \
    projectmanager.cpp \
    projectmodels.cpp \
    projectnodes.cpp \
    projectpanelfactory.cpp \
    projecttreewidget.cpp \
    projectwindow.cpp \
    propertiespanel.cpp \
    removefiledialog.cpp \
    runconfiguration.cpp \
    runconfigurationmodel.cpp \
    runsettingspropertiespage.cpp \
    selectablefilesmodel.cpp \
    SelectProjectsDialog.cpp \
    settingsaccessor.cpp \
    showineditortaskhandler.cpp \
    showoutputtaskhandler.cpp \
    target.cpp \
    targetselector.cpp \
    targetsettingspanel.cpp \
    targetsettingswidget.cpp \
    targetsetuppage.cpp \
    targetsetupwidget.cpp \
    task.cpp \
    taskhub.cpp \
    taskmodel.cpp \
    taskwindow.cpp \
    toolchain.cpp \
    toolchainmanager.cpp \
    unconfiguredprojectpanel.cpp \
    wincetoolchain.cpp \
    windebuginterface.cpp \
    jsonwizard/jsonfieldpage.cpp \
    jsonwizard/jsonfilepage.cpp \
    jsonwizard/jsonkitspage.cpp \
    jsonwizard/jsonprojectpage.cpp \
    jsonwizard/jsonsummarypage.cpp \
    jsonwizard/jsonwizard.cpp \
    jsonwizard/jsonwizardfactory.cpp \
    jsonwizard/jsonwizardfilegenerator.cpp \
    jsonwizard/jsonwizardgeneratorfactory.cpp \
    jsonwizard/jsonwizardpagefactory.cpp \
    jsonwizard/jsonwizardpagefactory_p.cpp \
    projectwizardpage.cpp \
    customwizard/customwizard.cpp \
    customwizard/customwizardpage.cpp \
    customwizard/customwizardparameters.cpp \
    customwizard/customwizardpreprocessor.cpp \
    customwizard/customwizardscriptgenerator.cpp \
    addnewmodel.cpp \
    baseprojectwizarddialog.cpp \
    cpptoolsjsextension.cpp \
    qtsupport/codegenerator.cpp \
    qtsupport/codegensettings.cpp \
    qtsupport/codegensettingspage.cpp \
    cppfilesettingspage.cpp \
    projectfilewizardextension.cpp \
    qtsupport/formtemplatewizardpage.cpp \
    qtsupport/qtdesignerformclasscodegenerator.cpp \
    ProjectManagerPythonModule.cpp \
    miniprojecttargetselector.cpp

FORMS += \
    ConfigureProjectsDialog.ui \
    customparserconfigdialog.ui \
    doubletabwidget.ui \
    processstep.ui \
    removefiledialog.ui \
    SelectProjectsDialog.ui \
    targetsettingswidget.ui \
    devicesupport/desktopdeviceconfigurationwidget.ui \
    devicesupport/devicefactoryselectiondialog.ui \
    devicesupport/devicesettingswidget.ui \
    devicesupport/devicetestdialog.ui \
    options/projectexplorersettingspage.ui \
    qtsupport/debugginghelper.ui \
    qtsupport/qtversioninfo.ui \
    qtsupport/qtversionmanager.ui \
    qtsupport/showbuildlog.ui \
    projectwizardpage.ui \
    qtsupport/codegensettingspagewidget.ui \
    qtsupport/cpp/formclasswizardpage.ui \
    cppfilesettingspage.ui

RESOURCES += \
    projectexplorter.qrc

DISTFILES += \
    jsonwizard/jsonwizard.pri \
    customwizard/customwizard.pri

templates.path = $${DESTDIR}
templates.files = templates/
INSTALLS += templates
