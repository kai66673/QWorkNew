!win32: error("process_ctrlc_stub is Windows only")

include (../../common.prf)

CONFIG    -= qt
CONFIG    += console warn_on

TEMPLATE  = app
TARGET    = qtcreator_ctrlc_stub

DESTDIR = $${TARGET_DIR}

SOURCES   += process_ctrlc_stub.cpp
LIBS      += -luser32 -lshell32

