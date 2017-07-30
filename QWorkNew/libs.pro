TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += aggregation \
           utils \
           ssh \
           PythonQt \
           settings \
           corelib \
           projectexplorer \
           find \
           texteditorlib \
           cplusplus \
           richeditorlib \
           pythoneditorlib \
           sqlengine \
           rpmllib \
           qworkbase

win32: SUBDIRS += process_ctrlc_stub

win32: process_ctrlc_stub.file = ../libs/utils/process_ctrlc_stub.pro

aggregation.file        = ../libs/aggregation/aggregation.pro
utils.file              = ../libs/utils/utils.pro
ssh.file                = ../libs/ssh/ssh.pro
PythonQt.file           = ../libs/PythonQt3.0/PythonQt.pro
settings.file           = ../libs/settings/settings.pro
corelib.file            = ../libs/corelib/corelib.pro
projectexplorer.file    = ../libs/ProjectExplorer/projectexplorer.pro
find.file               = ../libs/find/find.pro
cplusplus.file          = ../libs/cplusplus/cplusplus.pro
texteditorlib.file      = ../libs/texteditorlib/texteditorlib.pro
richeditorlib.file      = ../libs/richeditorlib/richeditorlib.pro
rpmllib.file            = ../libs/rpmllib/rpmllib.pro
pythoneditorlib.file    = ../libs/pythoneditorlib/pythoneditorlib.pro
sqlengine.file          = ../libs/sqlengine/sqlengine.pro
qworkbase.file          = ../libs/qworkbase/qworkbase.pro
