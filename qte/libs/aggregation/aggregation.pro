include (../../common.prf)

TEMPLATE = lib
TARGET = aggregate

DEFINES += AGGREGATION_LIBRARY

INCLUDEPATH += .

DESTDIR = $${TARGET_DIR}

HEADERS += \
    aggregation_global.h \
    aggregate.h

SOURCES += \
    aggregate.cpp
