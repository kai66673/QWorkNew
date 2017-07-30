include (../../../../common.prf)

QT           += sql xml
TEMPLATE      = lib
CONFIG       += plugin

INCLUDEPATH += ..
INCLUDEPATH += ../../../../libs
INCLUDEPATH += ../../../../libs/settings
INCLUDEPATH += ../../../../libs/corelib

TARGET = DspImporter

DESTDIR = ../../../../../../$${BUILDDIR}/QWorkNew/$${QTVERDIR}/$${BINDIR}/aspdbimporters
LIBS += -L../../../../../../$${BUILDDIR}/QWorkNew/$${QTVERDIR}/$${BINDIR}
LIBS += -L../../../../../../$${BUILDDIR}/QWorkNew/$${QTVERDIR}/$${BINDIR}/plugins

LIBS += -lcore
LIBS += -lasproject

HEADERS += DspImporterPlugin.h \
    DspConfigDialog.h

SOURCES += DspImporterPlugin.cpp \
    DspConfigDialog.cpp

FORMS += \
    DspConfigDialog.ui
