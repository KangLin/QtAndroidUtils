# Qt android 应用库

作者: 康林(kl222@126.com)

---

[<img src="Image/English.png" alt="英语" title="英语" width="16" height="16" />英语](README.md) 
[![Build Status](https://travis-ci.org/KangLin/QtAndroidUtils.svg?branch=master)](https://travis-ci.org/KangLin/QtAndroidUtils)

<!-- toc -->

- [源码目录](#源码目录)
- [功能](#功能)
- [编译](#编译)
- [应用程序使用本库](#应用程序使用本库)
- [捐赠](#捐赠)
- [贡献 :beers:](#贡献-beers)

<!-- tocstop -->

---

## 源码目录

    |-- android/                          # The library source code
    |      |-- QtAndroidUtilsModule
    |             |-- jni                 # jni source code
    |             |-- src                 # java source code
    |-- daemon/                           # The daemon code

---

## 功能

- 验证存储权限
- 振动
- 屏幕唤醒
- Android 系统目录
- 消息通知
- 调用系统摄像头和相册
- 分享功能

---

## 编译

+ 用 Qtcreate 打开 QtAndroidUtils.pro 
+ 编译 
+ 生成库
  - JNI库(*.so) : libQtAndroidUtilsModule.so
  - Android 库(*.aar): QtAndroidUtilsModule-*.aar

---

## 应用程序使用本库

+ 做为源码使用：导入本工程到应用程序源码目录树中
  - 如果就用程序没有自己的 android 源码
    + 增加本工程到应用程序

          $ cd application_root
          $ git submodule add https://github.com/KangLin/QtAndroidUtils.git android

    + 增加 android/android.pri 到应用程序的主 pro 文件中

          $ cd application_root
          $ vim application.pro
          android: include(android/android/android.pri)

  - 如果应用程序有它自己的 android 源代码。例如: application_root/android

        $ ls application_root/android
        android.pri  AndroidManifest.xml  build.gradle  jni/  src/

    + 本工程做为子模块加入到应用程序的 android 目录:

            $ cd application_root/android
            $ git submodule add https://github.com/KangLin/QtAndroidUtils.git

    + 增加 jni 模块到应用程序的 android.pri 中：

          $ cd application_root/android
          $ vim android.pri
          // 增加 JNI
          android: include(QtAndroidUtils/android/QtAndroidUtilsModule/jni/jni.pri)
          // 或者
          //android: include(QtAndroidUtils/android/android.pri)

    + 增加 JAVA 模块
      - 增加生成 JAVA 模块代码到 settings.gradle 中：

            $ cd application_root/android
            $ vim settings.gradle
            include ':QtAndroidUtils/android/QtAndroidUtilsModule'
          
      - 修改 build.gradle 增加 implementation project(':QtAndroidUtils/android/QtAndroidUtilsModule') 到 dependencies 块

            $ cd application_root/android
            $ vim build.gradle
            dependencies {
                implementation project(':QtAndroidUtils/android/QtAndroidUtilsModule')
            }
          
+ 做为库使用
  * Android 库(*.aar)
    - 复制 QtAndroidUtilsModule-release.aar 到 libs 目录中
    - 增加下列代码到 build.gradle 文件中：
  
          $ cd application_root/android
          $ vim build.gradle
          repositories {
              flatDir {
                  dirs 'libs'
              }
          }
            
    - 增加下列代码到 dependencies 块：
  
          $ cd application_root/android
          $ vim build.gradle
          dependencies {
              implementation (name:'QtAndroidUtilsModule-release',ext:'aar')
          }
        
  * JNI 库(*.so)
    - 导入库 libQtAndroidUtilsModule.so
    - 包含头文件
    
---

## 捐赠

![donation](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "donation")

## 贡献 :beers:
- [GitHub](https://github.com/KangLin/QtAndroidUtils)
