#ANDROID source directory, Please set it in main profile 

isEmpty(ANDROID_PACKAGE_SOURCE_DIR) : ANDROID_PACKAGE_SOURCE_DIR = $$PWD

include(QtAndroidUtilsModule/QtAndroidUtilsModule.pri)

# TODO: Check if there is the module*.aar file under the directory of
#       $$ANDROID_PACKAGE_SOURCE_DIR/libs 
# if don't exist, then add settings.gradle to $$ANDROID_PACKAGE_SOURCE_DIR
OTHER_FILES += \
    $$PWD/AndroidManifest.xml \
    $$PWD/build.gradle \
    $$PWD/settings.gradle

