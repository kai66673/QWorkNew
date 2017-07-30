include (../../../common.prf)

TEMPLATE = lib
TARGET = Qt4Concurrent
DEFINES += BUILD_QTCONCURRENT

DESTDIR = ../../../../../$${BUILDDIR}/QWorkNew/$${QTVERDIR}/$${BINDIR}

HEADERS += \
    qtconcurrent_global.h \
    multitask.h \
    runextensions.h
