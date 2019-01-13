#ANDROID source directory, Please set it in main profile 

isEmpty(ANDROID_PACKAGE_SOURCE_DIR) : ANDROID_PACKAGE_SOURCE_DIR = $$PWD

include(jni/jni.pri)

OTHER_FILES += \
    $$PWD/AndroidManifest.xml \
    $$PWD/build.gradle

DISTFILES += \
    $$PWD/src/org/KangLinStudio/* \
    $$PWD/res/drawable-hdpi/* \
    $$PWD/res/drawable-ldpi/* \
    $$PWD/res/drawable-mdpi/* \
    $$PWD/res/drawable-nodpi/* 
