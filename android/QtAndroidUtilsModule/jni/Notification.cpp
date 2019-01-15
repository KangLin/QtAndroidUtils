#include "Notification.h"
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#include <QDebug>
#include <android/bitmap.h>

#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
    qDebug() << __FILE__ << "(" << __LINE__ << ")" << "exception occured";\
    env->ExceptionClear(); \
    }

CNotification::CNotification(QObject *parent) : QObject(parent)
{
    static int id = 0;
    m_nID = id++;
}

int CNotification::Show(const QString &szText, const QString &szTitle,
                        int nNum)
{
    int nRet = 0;
    
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    
    QAndroidJniObject objText = QAndroidJniObject::fromString(szText);
    CHECK_EXCEPTION();
    QAndroidJniObject objTitle = QAndroidJniObject::fromString(szTitle);
    CHECK_EXCEPTION();
    //*
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "notify",
            "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;II)V",
            active.object<jobject>(),
            objText.object<jstring>(),
            objTitle.object<jstring>(),
            nNum,
            m_nID
            );
    CHECK_EXCEPTION();//*/
    
    /*
    QAndroidJniObject builder("android/app/Notification/Builder",
                              "(Landroid/content/Context;)V",
                              active.object<jobject>());
    CHECK_EXCEPTION();
    builder.callObjectMethod("setContentTitle",
                             "(Ljava/lang/String;)Landroid/app/Notification/Builder;",
                              objTitle.object<jstring>());
    CHECK_EXCEPTION();
    builder.callObjectMethod("setContentText",
                             "(Ljava/lang/String;)Landroid/app/Notification/Builder;",
                              objText.object<jstring>());
    CHECK_EXCEPTION();
    QAndroidJniObject notification = builder.callObjectMethod<jobject>("build");
    CHECK_EXCEPTION();
    
    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField(
                "android/content/Context",
                "NOTIFICATION_SERVICE",
                "Ljava/lang/String;"
                );
    CHECK_EXCEPTION();
    QAndroidJniObject notificationManager = active.callObjectMethod(
                "getSystemService",
                "(Ljava/lang/String;)Landroid/app/NotificationManager;",
                name.object<jstring>());
    CHECK_EXCEPTION();
    notificationManager.callObjectMethod("notify", "(ILjava/lang/Object;)V",
                                         m_nID, notification.object<jobject>());
    //*/
    return nRet;
}

int CNotification::Show(const QString &szText,
                        const QString &szTitle,
                        int nNum,
                        const QString &szSmallIcon,
                        const QString &szLargeIcon)
{
    int nRet = 0;
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    
    QAndroidJniObject objText = QAndroidJniObject::fromString(szText);
    CHECK_EXCEPTION();
    QAndroidJniObject objTitle = QAndroidJniObject::fromString(szTitle);
    CHECK_EXCEPTION();
    QAndroidJniObject objSmallIcon = QAndroidJniObject::fromString(szSmallIcon);
    CHECK_EXCEPTION();
    QAndroidJniObject objLargeIcon = QAndroidJniObject::fromString(szLargeIcon);
    CHECK_EXCEPTION();
    
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "notify",
            "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;)V",
            active.object<jobject>(),
            objText.object<jstring>(),
            objTitle.object<jstring>(),
            nNum,
            m_nID,
            objSmallIcon.object<jstring>(),
            objLargeIcon.object<jstring>()
            );
    CHECK_EXCEPTION();
    return nRet;
}

QAndroidJniObject BitmapFromQImage(const QImage image)
{
    QAndroidJniEnvironment env;
    if(image.isNull())
        return nullptr;
    
    QImage img;
    if (image.format() != QImage::Format_RGBA8888
            && image.format() != QImage::Format_RGB16)
        img = image.convertToFormat(QImage::Format_RGBA8888);
    
    if(img.isNull())
        return nullptr;
    
    jclass clsConfig = env->FindClass("android/graphics/Bitmap$Config");
    jfieldID fieldId = env->GetStaticFieldID(clsConfig,
                                "ARGB_8888",
                                "Landroid/graphics/Bitmap$Config;");

    QAndroidJniObject config = env->GetStaticObjectField(clsConfig, fieldId);
    CHECK_EXCEPTION();
    QAndroidJniObject bitmap = 
            QAndroidJniObject::callStaticObjectMethod(
                "android/graphics/Bitmap",
                "createBitmap",
                "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;",
                img.width(),
                img.height(),
                config.object<jobject>()
            );
    CHECK_EXCEPTION();
    if(!bitmap.isValid())
        return nullptr;
   
    AndroidBitmapInfo info;
    if (AndroidBitmap_getInfo(env, bitmap.object<jobject>(), &info) < 0) {
        env->DeleteLocalRef(bitmap.object<jobject>());
        return nullptr;
    }

    void *pixels;
    if (AndroidBitmap_lockPixels(env, bitmap.object<jobject>(), &pixels) < 0) {
        env->DeleteLocalRef(bitmap.object<jobject>());
        return nullptr;
    }

    if (info.stride == uint(img.bytesPerLine())
            && info.width == uint(img.width())
            && info.height == uint(img.height())) {
        memcpy(pixels, img.constBits(), info.stride * info.height);
    } else {
        uchar *bmpPtr = static_cast<uchar *>(pixels);
        const unsigned width = qMin(info.width, (uint)img.width()); //should be the same
        const unsigned height = qMin(info.height, (uint)img.height()); //should be the same
        for (unsigned y = 0; y < height; y++, bmpPtr += info.stride)
            memcpy(bmpPtr, img.constScanLine(y), width);
    }
    AndroidBitmap_unlockPixels(env, bitmap.object<jobject>());
    
    return bitmap;
}

int CNotification::Show(const QString &szText,
                        const QString &szTitle,
                        int nNum,
                        const QImage &smallIcon,
                        const QImage &largeIcon)
{
    int nRet = 0;
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    
    QAndroidJniObject objText = QAndroidJniObject::fromString(szText);
    CHECK_EXCEPTION();
    QAndroidJniObject objTitle = QAndroidJniObject::fromString(szTitle);
    CHECK_EXCEPTION();
    QAndroidJniObject objSmallIcon = BitmapFromQImage(smallIcon);
    QAndroidJniObject objLargeIcon = BitmapFromQImage(largeIcon);
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "notify",
            "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;IILandroid/graphics/Bitmap;Landroid/graphics/Bitmap;)V",
            active.object<jobject>(),
            objText.object<jstring>(),
            objTitle.object<jstring>(),
            nNum,
            m_nID,
            objSmallIcon.object<jobject>(),
            objLargeIcon.object<jobject>()
            );
    return nRet;
}

int CNotification::Cancel()
{
    int nRet = 0;
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "cancel",
            "(Landroid/content/Context;I)V",
            active.object<jobject>(),
            m_nID);
    CHECK_EXCEPTION();
    return nRet;
}

int CNotification::CanCelAll()
{
    int nRet = 0;
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "cancelAll",
            "(Landroid/content/Context;)V",
            active.object<jobject>());
    CHECK_EXCEPTION();
    return nRet;
}
