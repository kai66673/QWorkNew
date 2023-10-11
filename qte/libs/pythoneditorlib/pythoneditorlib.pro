include (../../common.prf)

TEMPLATE = lib
TARGET = pythoneditorlib

QT += xml

DEFINES += PYTHONEDITOR_LIBRARY

INCLUDEPATH += .
INCLUDEPATH += ../settings
INCLUDEPATH += ../corelib
INCLUDEPATH += ../utils
INCLUDEPATH += ../texteditorlib

DESTDIR = $${TARGET_DIR}
LIBS += -L$${TARGET_DIR}

LIBS += -lqtcutils
LIBS += -lsettings
LIBS += -lcore
LIBS += -ltexteditor

HEADERS += \
    tools/lexical/pythonscanner.h \
    tools/lexical/pythonformattoken.h \
    tools/lexical/sourcecodestream.h \
    tools/pythonhighlighter.h \
    tools/pythonindenter.h \
    pythoneditor_global.h \
    pythoneditor.h \
    pythoneditorwidget.h

SOURCES += \
    tools/lexical/pythonscanner.cpp \
    tools/pythonindenter.cpp \
    tools/pythonhighlighter.cpp \
    pythoneditorwidget.cpp \
    pythoneditor.cpp
