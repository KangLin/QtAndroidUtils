# Author: Kang Lin <kl222@126.com>

project(QtAndroidUtils)

set(SOURCE_FILES
    main.cpp
    MainWindow.cpp
    )

set(HEADER_FILES
    MainWindow.h
    )

set(SOURCE_UI_FILES
    MainWindow.ui
    )

LIST(APPEND RCC_FILES Resources.qrc)

set(APP_QT_COMPONENTS Core Gui Widgets)
if(QT_VERSION_MAJOR VERSION_GREATER_EQUAL 6)
    list(APPEND APP_QT_COMPONENTS StateMachine)
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${APP_QT_COMPONENTS})
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${APP_QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
else()
    message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} don't found")
endif()

ADD_TARGET(NAME ${PROJECT_NAME}
    ISEXE
    VERSION ${QtAndroidUtils_VERSION}
    INCLUDE_DIRS ${APP_INCLUDE_DIRS}
    PRIVATE_LIBS QtAndroidUtilsModule
    PRIVATE_DEFINITIONS QtAndroidUtils_VERSION="${QtAndroidUtils_VERSION}"
        QtAndroidUtils_REVISION="${QtAndroidUtils_REVISION}"
        ${APP_PRIVATE_DEFINITIONS}
    SOURCE_FILES ${SOURCE_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES}
    )

if(NOT QtAndroidUtils_VERSION)
    message(WARNING "QtAndroidUtils_VERSION is empty, using default version")
    set(QtAndroidUtils_VERSION "1.0.0")
endif()

# 确保版本号格式正确
if(NOT QtAndroidUtils_VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+")
    message(WARNING "Version format is not 'x.y.z', using fallback parsing")
endif()

# 使用方案一的解析逻辑
string(REGEX MATCHALL "[0-9]+" VERSION_PARTS "${QtAndroidUtils_VERSION}")
list(LENGTH VERSION_PARTS PARTS_COUNT)

if(PARTS_COUNT GREATER_EQUAL 3)
    list(GET VERSION_PARTS 0 VERSION_MAJOR)
    list(GET VERSION_PARTS 1 VERSION_MINOR)
    list(GET VERSION_PARTS 2 VERSION_PATCH)
else()
    # 不足3段时补齐
    set(VERSION_MAJOR 1)
    set(VERSION_MINOR 0)
    set(VERSION_PATCH 0)
endif()

math(EXPR QT_ANDROID_VERSION_CODE "${VERSION_MAJOR}*10000 + ${VERSION_MINOR}*100 + ${VERSION_PATCH}")
set(QT_ANDROID_VERSION_NAME "${QtAndroidUtils_VERSION}")

message(STATUS "Version Code: ${QT_ANDROID_VERSION_CODE}, Version Name: ${QT_ANDROID_VERSION_NAME}")

set_target_properties(${PROJECT_NAME} PROPERTIES
    # Android 包名
    QT_ANDROID_PACKAGE_NAME "io.github.KangLinStudio.QtAndroidUtils"
    #QT_ANDROID_APP_NAME "@string/app_name"
    #QT_ANDROID_APP_ICON "@drawable/icon"
    QT_ANDROID_VERSION_CODE ${QT_ANDROID_VERSION_CODE}
    QT_ANDROID_VERSION_NAME ${QT_ANDROID_VERSION_NAME}
    # 最小 SDK 版本
    QT_ANDROID_MIN_SDK_VERSION 28
    # 目标 SDK 版本
    QT_ANDROID_TARGET_SDK_VERSION 34
    #QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Android
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
