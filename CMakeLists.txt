# Author: Kang Lin <kl222@126.com>

cmake_minimum_required(VERSION 3.25)
MESSAGE(STATUS "Found CMake ${CMAKE_VERSION}")

project(QtAndroidUtils)

if(NOT ANDROID)
    message(FATAL_ERROR "The project is a android project")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
# 确保编译器支持 C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 避免使用编译器特定的扩展语法，从而提高跨平台一致性。
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 17)
# 确保编译器支持 C17
set(CMAKE_C_STANDARD_REQUIRED ON)
# 避免使用编译器特定的扩展语法，从而提高跨平台一致性。
set(CMAKE_C_EXTENSIONS OFF)

#set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "verbose")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CMakeDependentOption)
include(CheckSymbolExists)
include(CheckCXXSymbolExists)
include(CheckStructHasMember)

#CHECK_INCLUDE_FILE_CXX("string" HAVE_STRING_H)
#check_include_file("math.h" HAVE_MATH_H)

#check_function_exists("fabs" HAVE_FABS)

# Open qt build tool
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

# Need qt components
SET(QT_COMPONENTS Core)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()
get_filename_component(QT_INSTALL_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../.." ABSOLUTE)
message("QT_ROOT:${QT_ROOT}")
message("QT_DIR:${QT_DIR}")
message("Qt${QT_VERSION_MAJOR}_ROOT:${Qt${QT_VERSION_MAJOR}_ROOT}")
message("Qt${QT_VERSION_MAJOR}_DIR:${Qt${QT_VERSION_MAJOR}_DIR}")
message("QT_INSTALL_DIR:${QT_INSTALL_DIR}")
message("${PROJECT_NAME} QT_LIBRARIES:${QT_LIBRARIES}")

if(NOT RabbitCommon_ROOT)
    set(RabbitCommon_ROOT $ENV{RabbitCommon_ROOT})
    if(NOT RabbitCommon_ROOT)
        set(RabbitCommon_ROOT ${CMAKE_SOURCE_DIR}/../RabbitCommon)
    endif()
endif()
if(RabbitCommon_ROOT AND EXISTS ${RabbitCommon_ROOT}/Src)
    message("Use RabbitCommon source code")
    add_subdirectory(${RabbitCommon_ROOT}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
else()
    find_package(RabbitCommon)
    if(NOT RabbitCommon_FOUND)
        message(FATAL_ERROR
            "RabbitCommon_ROOT is not found. Please use one of the following ways to set it:\n"
            "1. Set RabbitCommon_ROOT to the install prefix of RabbitCommon.\n"
            "2. Set RabbitCommon_ROOT to source code root of RabbitCommon.\n"
            "2.1 Please download the source code of RabbitCommon from https://github.com/KangLin/RabbitCommon\n"
            "   ag:\n"
            "       git clone https://github.com/KangLin/RabbitCommon.git ${CMAKE_SOURCE_DIR}/..\n"
            "2.2 Then set cmake variable or environment variable RabbitCommon_ROOT to download root directory.\n"
            "   ag:\n"
            "       cmake -DRabbitCommon_ROOT=${CMAKE_SOURCE_DIR}/.. ")
    endif()
endif()

GET_VERSION(OUT_VERSION QtAndroidUtils_VERSION
    OUT_REVISION QtAndroidUtils_REVISION
    OUT_TAG QtAndroidUtilsTag)
IF(NOT QtAndroidUtils_VERSION)
    SET(QtAndroidUtils_VERSION "0.0.1")
ENDIF()
if(NOT(QtAndroidUtils_VERSION MATCHES "[0-9]+\\.[0-9]+\\.[0-9]+[+\\-^_~0-9A-Za-z]*"))
    SET(QtAndroidUtils_VERSION "0.0.1")
endif()
IF(NOT QtAndroidUtilsTag)
    SET(QtAndroidUtilsTag "0.0.1")
ENDIF()
message("QtAndroidUtils_VERSION:${QtAndroidUtils_VERSION};Revision:${QtAndroidUtils_REVISION};Tag:${QtAndroidUtilsTag}")
set(VERSION ${QtAndroidUtils_VERSION})

add_subdirectory(jni)
add_subdirectory(App)
