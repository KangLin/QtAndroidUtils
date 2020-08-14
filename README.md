# Qt android utils

Autrhor: KangLin(kl222@126.com)

---

[<img src="Image/china.png" alt="Chinese" title="Chinese" width="16" height="16"/>Chinese](README_zh_CN.md) 
[![Build Status](https://travis-ci.org/KangLin/QtAndroidUtils.svg?branch=master)](https://travis-ci.org/KangLin/QtAndroidUtils)

<!-- toc -->

- [Source directory](#Source-directory)
- [Functions](#Functions)
- [Build](#Build)
- [Other application use the library](#Other-application-use-the-library)
- [Contribute](#Contribute)
- [Community :beers:](#Community-beers)

<!-- tocstop -->

---

## Source directory

    |-- android/                          # The library source code
    |      |-- QtAndroidUtilsModule
    |             |-- jni                 # jni source code
    |             |-- src                 # java source code
    |-- daemon/                           # The daemon code

---

## Functions

- Verify storage permissions
- Vibrate
- Screen wake
- Android system directory
- Message notifiction
- Call system album and camera
- Share

---

## Build

+ Use Qtcreate open QtAndroidUtils.pro
+ build
+ Generate library
  - JNI(*.so) : libQtAndroidUtilsModule.so
  - Android(*.aar): QtAndroidUtilsModule-*.aar

---

## Other application use the library

+ Introduce this project to the application source tree
  - If the application hasn't its own android source code
    + Add the project to the application

          $ cd application_root
          $ git submodule add https://github.com/KangLin/QtAndroidUtils.git android

    + Add android/android.pri to application's main profile

          $ cd application_root
          $ vim application.pro
          android: include(android/android/android.pri)

  - If the application has its own android source code. ag: application_root/android

        $ ls application_root/android
        android.pri  AndroidManifest.xml  build.gradle  jni/  src/

    +  then git submodle from this project to the application's android directory:

            $ cd application_root/android  
            $ git submodule add https://github.com/KangLin/QtAndroidUtils.git

    + Add jni module to application project file
      - qmake project: android.pri

            $ cd application_root/android
            $ vim android.pri
            # Add jni
            android {
                # Include pri
                include(QtAndroidUtils/android/QtAndroidUtilsModule/jni/jni.pri)
                # Include header files path
                INCLUDEPATH *= QtAndroidUtils/android/QtAndroidUtilsModule/jni
            }

      - cmake project

            $ cd application_root
            $ vim CMakeLists.txt
            add_subdirectory(android/QtAndroidUtils/android/QtAndroidUtilsModule/jni)

    + Add java module 
      - Generate module library: add the following code to settings.gradle

            $ cd application_root/android
            $ vim settings.gradle
            // Add to compile aar module
            include ':QtAndroidUtils/android/QtAndroidUtilsModule'
          
      - Modify build.gradle to add implementation project(':QtAndroidUtils/android/QtAndroidUtilsModule') to dependencies

            $ cd application_root/android
            $ vim build.gradle
            dependencies {
                // Add aar project
                implementation project(':QtAndroidUtils/android/QtAndroidUtilsModule')
            }
          
+ Use as a library
  * Android (*.aar)
    - Copy QtAndroidUtilsModule-release.aar to libs
    - Add the following code to build.gradle
  
          $ cd application_root/android
          $ vim build.gradle
          repositories {
              flatDir {
                  dirs 'libs'
              }
          }
            
    - add the following code to dependencies
  
          $ cd application_root/android
          $ vim build.gradle
          dependencies {
              // Add aar module
              implementation (name:'QtAndroidUtilsModule-release',ext:'aar')
          }
    
  * JNI (*.so)
    - qmake project
      + Import library: libQtAndroidUtilsModule.so
      + inlude heade files
    - cmake project
    
          find_package(QtAndroidUtilsModule)

---

## Donation

[![donation](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "donation")](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "donation")

## Community :beers:
- [GitHub](https://github.com/KangLin/QtAndroidUtils)
