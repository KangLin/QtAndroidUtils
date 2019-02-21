# Qt android utils

Autrhor: KangLin(kl222@126.com)

---

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

    |-- android/         # The library source code
    |
    |-- daemon/          # The library daemon code


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

    + Add QtAndroidUtils/android/android.pri to application's android.pri

          $ cd application_root/android
          $ vim android.pri
          android: include(QtAndroidUtils/android/android.pri)

    + Generate module library: add settings.gradle

          $ cd application_root/android
          $ vim settings.gradle
          include ':QtAndroidUtils/android/QtAndroidUtilsModule'
          
    + Modify build.gradle to add implementation project(':QtAndroidUtils/android/QtAndroidUtilsModule') to dependencies

          $ cd application_root/android
          $ vim build.gradle
          dependencies {
              implementation fileTree(dir: 'libs', include: ['*.jar'])
              implementation project(':QtAndroidUtils/android/QtAndroidUtilsModule')
          }
          
---

## Contribute

![Contribute( More than ￥20 )](https://github.com/KangLin/RabbitIm/blob/master/Resource/png/Contribute.png  "Contribute( More than ￥20 )")

![Contribute](https://github.com/KangLin/RabbitIm/blob/master/Resource/png/Contribute20.png  "Contribute")

## Community :beers:
- [GitHub](https://github.com/KangLin/QtAndroidUtils.git)
