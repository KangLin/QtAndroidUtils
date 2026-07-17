set_target_properties(${PROJECT_NAME} PROPERTIES
    # Android 包名
    QT_ANDROID_PACKAGE_NAME "io.github.KangLinStudio.${PROJECT_NAME}"
    # 最小 SDK 版本
    QT_ANDROID_MIN_SDK_VERSION 28
    # 目标 SDK 版本
    QT_ANDROID_TARGET_SDK_VERSION 34
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Android
)

qt_add_android_permission(${PROJECT_NAME}
    NAME android.permission.WRITE_EXTERNAL_STORAGE
)
qt_add_android_permission(${PROJECT_NAME}
    NAME android.permission.READ_EXTERNAL_STORAGE
)
qt_add_android_permission(${PROJECT_NAME}
    NAME android.permission.CAMERA
)
qt_add_android_permission(${PROJECT_NAME}
    NAME android.permission.CALL_PHONE
)
qt_add_android_permission(${PROJECT_NAME}
    NAME android.permission.VIBRATE
)
qt_add_android_permission(${PROJECT_NAME}
    NAME android.permission.WAKE_LOCK
)
qt_add_android_permission(${PROJECT_NAME}
    NAME android.permission.DEVICE_POWER
)
qt_add_android_permission(${PROJECT_NAME}
    NAME android.permission.REQUEST_INSTALL_PACKAGES
)
