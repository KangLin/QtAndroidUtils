
versionAtMost(QT_VERSION, 5.3.0) {
    error("Android: Qt version must greater than or equal to 5.3.0")
}
else {
    QT += androidextras
}

QT += gui

INCLUDEPATH += $$PWD
LIBS += -ljnigraphics

SOURCES += \ 
    $$PWD/AndroidDirectory.cpp \
    $$PWD/AndroidUtils.cpp \
    $$PWD/Notification.cpp

HEADERS += \ 
    $$PWD/AndroidDirectory.h \
    $$PWD/AndroidUtils.h \
    $$PWD/Notification.h
