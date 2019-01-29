#include "Notification.h"
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#include <QDebug>
#include <android/bitmap.h>
#include "NativeCallback.h"

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
    
    bool check = connect(CNativeCallback::instant(),
                         SIGNAL(sigMessageNotificationOnClickCallBack(int)),
                         this,
                         SLOT(slotOnClick(int)));
    Q_ASSERT(check);
}

CNotification::~CNotification()
{
    Cancel();
}

int CNotification::Show(const QString &szText,
                        const QString &szTitle,
                        int nNum,
                        bool bCallBack)
{
    int nRet = 0;
    
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    
    QAndroidJniObject objText = QAndroidJniObject::fromString(szText);
    CHECK_EXCEPTION();
    QAndroidJniObject objTitle = QAndroidJniObject::fromString(szTitle);
    CHECK_EXCEPTION();
    
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "notify",
            "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;IIZ)V",
            active.object<jobject>(),
            objText.object<jstring>(),
            objTitle.object<jstring>(),
            nNum,
            m_nID,
            bCallBack
            );
    CHECK_EXCEPTION();
    return nRet;
}

int CNotification::Show(const QString &szText,
                        const QString &szTitle,
                        int nNum,
                        const QString &szSmallIcon,
                        const QString &szLargeIcon,
                        bool bCallBack)
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
            "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;IILjava/lang/String;Ljava/lang/String;Z)V",
            active.object<jobject>(),
            objText.object<jstring>(),
            objTitle.object<jstring>(),
            nNum,
            m_nID,
            objSmallIcon.object<jstring>(),
            objLargeIcon.object<jstring>(),
            bCallBack
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
    CHECK_EXCEPTION();
    jfieldID fieldId = env->GetStaticFieldID(clsConfig,
                                "ARGB_8888",
                                "Landroid/graphics/Bitmap$Config;");
    CHECK_EXCEPTION();
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
        return nullptr;
    }
    CHECK_EXCEPTION();
    void *pixels;
    if (AndroidBitmap_lockPixels(env, bitmap.object<jobject>(), &pixels) < 0) {
        return nullptr;
    }
    CHECK_EXCEPTION();
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
    CHECK_EXCEPTION();
    return bitmap;
}

int CNotification::Show(const QString &szText,
                        const QString &szTitle,
                        int nNum,
                        const QImage &smallIcon,
                        const QImage &largeIcon,
                        bool bCallBack)
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
    CHECK_EXCEPTION();
    QAndroidJniObject objLargeIcon = BitmapFromQImage(largeIcon);
    CHECK_EXCEPTION();
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "notify",
            "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;IILandroid/graphics/Bitmap;Landroid/graphics/Bitmap;Z)V",
            active.object<jobject>(),
            objText.object<jstring>(),
            objTitle.object<jstring>(),
            nNum,
            m_nID,
            objSmallIcon.object<jobject>(),
            objLargeIcon.object<jobject>(),
            bCallBack
            );
    return nRet;
}

template <typename T>
static T GetResourceId(const QString szClass, const QString szId)
{
    return QAndroidJniObject::getStaticField<T>(
                szClass.toStdString().c_str(),
                szId.toStdString().c_str());
}

template <typename T>
static T GetResourceId(const QString szId)
{
    QAndroidJniEnvironment env;
    T ret;
    QString szVal = szId;
    szVal.replace('.', '/');
    int nPos = szVal.lastIndexOf("/");
    if(-1 == nPos )
    {
        qCritical() << "There is not a class";
        return ret;
    }
    
    QString szClass = szVal.left(nPos);
    
    QString szID = szVal.mid(nPos + 1);
    qDebug() << "class name: " << szClass << " id: " << szID;
    ret = GetResourceId<T>(szClass, szID);
    CHECK_EXCEPTION();
    return ret;
}

int CNotification::ShowFromResource(const QString &szText,
                                    const QString &szTitle,
                                    int nNum,
                                    const QString &szSmallIconId,
                                    const QString &szLargeIconId,
                                    bool bCallBack)
{
    int nRet = 0;
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    QAndroidJniObject objText = QAndroidJniObject::fromString(szText);
    CHECK_EXCEPTION();
    QAndroidJniObject objTitle = QAndroidJniObject::fromString(szTitle);
    CHECK_EXCEPTION();
    int nSmall = GetResourceId<jint>(szSmallIconId);
    CHECK_EXCEPTION();
    int nLarge = GetResourceId<jint>(szLargeIconId);
    CHECK_EXCEPTION();
    qDebug() << "small id: " << nSmall << " large id: " << nLarge;
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "notify",
            "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;IIIIZ)V",
            active.object<jobject>(),
            objText.object<jstring>(),
            objTitle.object<jstring>(),
            nNum,
            m_nID,
            nSmall,
            nLarge,
            bCallBack
            );
    CHECK_EXCEPTION();
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

void CNotification::slotOnClick(int id)
{
    if(m_nID != id)
        return;
    OnClick();
    emit sigOnChilk();
}

void CNotification::OnClick()
{
}
