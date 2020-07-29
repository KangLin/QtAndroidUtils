#ANDROID source directory, Please set it in main profile 

isEmpty(ANDROID_PACKAGE_SOURCE_DIR): ANDROID_PACKAGE_SOURCE_DIR = $$PWD

include(QtAndroidUtilsModule/QtAndroidUtilsModule.pri)

# TODO: Check if there is the QtAndroidUtilsModule*.aar file under the directory of
#     $$ANDROID_PACKAGE_SOURCE_DIR/libs
#     If it is exist, then use aar.
#     If it isn't exist, then add settings.gradle to $$ANDROID_PACKAGE_SOURCE_DIR,
#        include ':QtAndroidUtilsModule'
#        to import QtAndroidUtilsModule*.aar

OTHER_FILES += \
    $$PWD/AndroidManifest.xml \
    $$PWD/build.gradle \
    $$PWD/settings.gradle
