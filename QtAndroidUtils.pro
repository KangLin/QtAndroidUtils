TARGET = QtAndroidUtils
TEMPLATE = subdirs

CONFIG += ordered

daemon.file = daemon/daemon.pro

SUBDIRS = daemon

OTHER_FILES += README.md
