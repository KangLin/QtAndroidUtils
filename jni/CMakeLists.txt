# Author: Kang Lin <kl222@126.com>

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
#set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "verbose")

project(QtAndroidUtilsModule)

set(QtAndroidUtils_LIBS RabbitCommon)

list(APPEND QtAndroidUtils_INSTALL_HEADER_FILES
    AndroidDirectory.h
    AndroidUtils.h
    Notification.h
    )

list(APPEND QtAndroidUtils_HEADER_FILES
    ${QtAndroidUtils_INSTALL_HEADER_FILES}
    ActivityResultReceiver.h
    NativeCallback.h
    )

list(APPEND QtAndroidUtils_SOURCE_FILES
    AndroidDirectory.cpp
    AndroidUtils.cpp
    Notification.cpp
    ActivityResultReceiver.cpp
    NativeCallback.cpp
    )

set(QtAndroidUtils_UI_FILES

    )

list(APPEND QtAndroidUtils_LIBS jnigraphics)

list(APPEND QtAndroidUtils_QT_COMPONENTS Core Gui Widgets)
if(Qt${QT_VERSION_MAJOR}_VERSION GREATER_EQUAL 6.0.0)
    list(APPEND QtAndroidUtils_QT_COMPONENTS CorePrivate)
elseif(Qt${QT_VERSION_MAJOR}_VERSION GREATER_EQUAL 5.10.0)
    list(APPEND QtAndroidUtils_QT_COMPONENTS androidextras)
else()
    message(FATAL_ERROR "Qt must great 5.10.0")
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QtAndroidUtils_QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QtAndroidUtils_QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
else()
    message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} don't found")
endif()
list(APPEND QtAndroidUtils_LIBS ${QT_LIBRARIES})
message("QtAndroidUtils_LIBS: ${QtAndroidUtils_LIBS} ${QtAndroidUtils_PRIVATE_LIBS}")
list(APPEND QtAndroidUtils_DEFINITIONS
    RabbitRemoteControl_VERSION="${RabbitRemoteControl_VERSION}")
if(QT_VERSION VERSION_GREATER_EQUAL "6.2.4")
    list(APPEND QtAndroidUtils_DEFINITIONS HAVE_QT6_MULTIMEDIA)
endif()
if(QT_VERSION VERSION_GREATER_EQUAL "6.8.0")
    list(APPEND QtAndroidUtils_DEFINITIONS HAVE_QT6_RECORD)
endif()
ADD_TARGET(NAME ${PROJECT_NAME}
    SOURCE_FILES ${QtAndroidUtils_SOURCE_FILES} ${QtAndroidUtils_HEADER_FILES} ${QtAndroidUtils_UI_FILES}
    INCLUDE_DIRS
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # See: http://www.it1352.com/478094.html
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    INSTALL_HEADER_FILES ${QtAndroidUtils_INSTALL_HEADER_FILES}
    PRIVATE_LIBS ${QtAndroidUtils_PRIVATE_LIBS}
    LIBS ${QtAndroidUtils_LIBS}
    VERSION ${RabbitRemoteControl_VERSION}
    PRIVATE_DEFINITIONS ${QtAndroidUtils_PRIVATE_DEFINITIONS}
    DEFINITIONS ${QtAndroidUtils_DEFINITIONS}
    INSTALL_EXPORT_NAME ${PROJECT_NAME}Targets
    INSTALL_NAMESPACE RabbitRemoteControl
    )
