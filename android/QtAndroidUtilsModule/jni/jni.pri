
versionAtMost(QT_VERSION, 5.4.0) : error("Android: Qt version must greater than or equal to 5.4.0")

QT += androidextras gui

INCLUDEPATH += $$PWD
LIBS += -ljnigraphics

SOURCES += \ 
    $$PWD/AndroidDirectory.cpp \
    $$PWD/AndroidUtils.cpp \
    $$PWD/Notification.cpp \
    $$PWD/ActivityResultReceiver.cpp \
    $$PWD/NativeCallback.cpp

INSTALL_HEADERS += \
    $$PWD/AndroidDirectory.h \
    $$PWD/AndroidUtils.h \
    $$PWD/Notification.h 

HEADERS += \
    $$INSTALL_HEADERS \
    $$PWD/ActivityResultReceiver.h \
    $$PWD/NativeCallback.h

ENABLE_TEST {
    DEFINES += ENABLE_TEST
    SOURCES += $$PWD/Test.cpp
    HEADERS += $$PWD/Test.h
}

android {
    ANDROID_PERMISSIONS *= android.permission.WRITE_EXTERNAL_STORAGE \
        android.permission.READ_EXTERNAL_STORAGE \
        android.permission.CAMERA \
        android.permission.CALL_PHONE \
        android.permission.VIBRATE \
        android.permission.WAKE_LOCK \
        android.permission.DEVICE_POWER \
        android.permission.READ_EXTERNAL_STORAGE \
        android.permission.WRITE_EXTERNAL_STORAGE \
        android.permission.REQUEST_INSTALL_PACKAGES
}
