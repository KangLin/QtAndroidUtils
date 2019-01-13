# Qt android utils

Autrhor: KangLin(kl222@126.com)

---

<!-- toc -->

- [Source directory](#Source-directory)
- [Build](#Build)
- [Other application use the library](#Other-application-use-the-library)
- [Contribute](#Contribute)
- [Community :beers:](#Community-beers)

<!-- tocstop -->

---

## Source directory

    |-- android/         # The library source code
    |
    |-- daemon/          # The library daemon code


---

## Build

+ Use Qtcreate open QtAndroidUtils.pro 
+ build 

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

    + First disable dependencies module library
    
          $ cd application_root/android/android
          $ vim build.gradle
          dependencies {
              implementation fileTree(dir: 'libs', include: ['*.jar'])
              // TODO: The first compilation needs to disable the following line
              //  Enable the following line after generate module library
              //implementation(name:'QtAndroidUtilsModule-debug', ext:'aar')
          }
          
    + Build and generate module library. The library name is determined by the build configuration. module library's position: $build_root/android-build/QtAndroidUtilsModule/build/outputs/aar/QtAndroidUtilsModule-*.aar
    + Copy QtAndroidUtilsModule-*.aar to application_root/android/android/libs
    
          $ cd application_root/android/android
          $ mkdir -p libs
          $ cd libs
          $ cp $build_root/android-build/module/build/outputs/aar/QtAndroidUtilsModule-*.aar .
    
    + Modify  build.gradle to 

          $ cd application_root/android/android
          $ vim build.gradle

         * enable 'implementation(name:'QtAndroidUtilsModule-debug', ext:'aar')' to dependencies

          dependencies {
              implementation fileTree(dir: 'libs', include: ['*.jar'])
              // TODO: The first compilation needs to disable the following line
              //  Enable the following line after generate module library
              implementation(name:'QtAndroidUtilsModule-debug', ext:'aar')
          }

  - If the application has its own android source code. ag: application_root/android

        $ ls application_root/android
        android.pri  AndroidManifest.xml  build.gradle  jni/  src/

    +  then git submodle from this project to the application's android directory:

            $ cd application_root/android  
            $ git submodule add https://github.com/KangLin/QtAndroidUtils.git

    + Add android/android.pri to application's android.pri

          $ cd application_root/android
          $ vim android.pri
          android: include(QtAndroidUtils/android/android.pri)

    + Generate module library. add settings.gradle

          $ cd application_root/android
          $ cat ''include ':QtAndroidUtils/android/QtAndroidUtilsModule'"" >> settings.gradle
          
    + Build and generate module library. The library name is determined by the build configuration. module library's position: $build_root/android-build/QtAndroidUtilsModule/build/outputs/aar/QtAndroidUtilsModule-*.aar
    + Copy QtAndroidUtilsModule-*.aar to application_root/android/android/libs
    
          $ cd application_root/android
          $ mkdir -p libs
          $ cd libs
          $ cp $build_root/android-build/QtAndroidUtils/android/QtAndroidUtilsModule/build/outputs/aar/QtAndroidUtilsModule-*.aar .
          
    + Modify  build.gradle to 

          $ cd application_root/android
          $ vim build.gradle

         * add 'implementation(name:'QtAndroidUtilsModule-debug', ext:'aar')' to dependencies

          dependencies {
              implementation fileTree(dir: 'libs', include: ['*.jar'])
              // TODO: The first compilation needs to disable the following line
              //  Enable the following line after generate module library
              implementation(name:'QtAndroidUtilsModule-debug', ext:'aar')
          }

         * add "dirs 'libs'" to repositories
      
          repositories {
              google()
              jcenter()
              flatDir{
                  dirs 'libs'
              }
          }
          
---

## Contribute

![Alipay contribute( More than ￥20 )](https://raw.githubusercontent.com/KangLin/RabbitIm/master/Resource/png/zhifubao.png  "Alipay contribute( More than ￥20 )")
![WeChat payment( More than ￥20 )](https://github.com/KangLin/RabbitIm/blob/master/Resource/png/weixinpay.png "WeChat payment( More than ￥20 )")

![Alipay contribute](https://raw.githubusercontent.com/KangLin/RabbitIm/master/Resource/png/zhifubao20.png  "Alipay contribute")
![WeChat payment](https://github.com/KangLin/RabbitIm/blob/master/Resource/png/weixinpay20.png "WeChat payment")

## Community :beers:
- [GitHub](https://github.com/KangLin/QtAndroidUtils.git)
