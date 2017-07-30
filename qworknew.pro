TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += libs \
           QWorkNew/plugins \
           qworkide

libs.file       = QWorkNew/libs.pro
qworkide.file   = QWorkNew/app/src.pro

