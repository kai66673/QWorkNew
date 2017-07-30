# --------- PythonQt profile -------------------
# Last changed by $Author: florian $
# $Id: PythonQt.pro 35381 2006-03-16 13:05:52Z florian $
# $Source$
# --------------------------------------------------

include ( ../../../common.prf )
include ( ../build/common.prf )
include ( ../build/python.prf )

TARGET   = PythonQt
TEMPLATE = lib

DESTDIR = ../$${TARGET_DIR}
LIBS += -L../$${TARGET_DIR}

CONFIG += qt
CONFIG -= flat


# allow to choose static linking through the environment variable PYTHONQT_STATIC
PYTHONQT_STATIC = $$(PYTHONQT_STATIC)
isEmpty(PYTHONQT_STATIC) {
  CONFIG += dll
} else {
  CONFIG += static
}

contains(QT_MAJOR_VERSION, 5) {
  QT += widgets
}

# Qt 5.4 adds this option, but this is not compatible with the Python API
QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
 
INCLUDEPATH += $$PWD

include ( src.pri )  

include($${PYTHONQT_GENERATED_PATH}/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin.pri)
include($${PYTHONQT_GENERATED_PATH}/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin.pri)
