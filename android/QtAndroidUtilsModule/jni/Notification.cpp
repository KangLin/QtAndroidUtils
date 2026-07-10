// Author: KangLin(kl222@!26.com)

#include <QLoggingCategory>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QJniObject>
    #include <QtCore/private/qandroidextras_p.h>
#else
    #include <QAndroidJniEnvironment>
    #include <QAndroidJniObject>
    #include <QtAndroid>
#endif

#include <android/bitmap.h>
#include "NativeCallback.h"

#include "Notification.h"
#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
    qDebug(log) << __FILE__ << "(" << __LINE__ << ")" << "exception occurred";\
    env->ExceptionClear(); \
    }

static Q_LOGGING_CATEGORY(log, "Notification")
CNotification::CNotification(QObject *parent) : QObject(parent)
{
    static int id = 0;
    m_nID = id++;
    qDebug(log) << "CNotification:" << this;
    bool check = connect(CNativeCallback::instant(),
                         SIGNAL(sigMessageNotificationOnClickCallBack(int)),
                         this,
                         SLOT(slotOnClick(int)));
    Q_ASSERT(check);
}

CNotification::~CNotification()
{
    CNativeCallback::instant()->disconnect(this);
    
    //TODO: If need ?
    Cancel();
}

void CNotification::slotOnClick(int id)
{
    qDebug() << "CNotification::slotOnClick" << this;
    if(m_nID != id)
    {
        qDebug() << "m_nID != id:" << m_nID << "!=" << id;
        return;
    }
    OnClick();
    emit sigOnChilk();
}

void CNotification::OnClick()
{
    //qDebug() << "CNotification::OnClick:" << m_nID;
}

int CNotification::Show(const QString &szText,
                        const QString &szTitle,
                        int nNum,
                        bool bCallBack)
{
    int nRet = 0;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
    QJniObject active = QNativeInterface::QAndroidApplication::context();
    CHECK_EXCEPTION()

    QJniObject objText = QJniObject::fromString(szText);
    CHECK_EXCEPTION()
    QJniObject objTitle = QJniObject::fromString(szTitle);
    CHECK_EXCEPTION()

    QJniObject::callStaticMethod<void>(
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
#else
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION()
    
    QAndroidJniObject objText = QAndroidJniObject::fromString(szText);
    CHECK_EXCEPTION()
    QAndroidJniObject objTitle = QAndroidJniObject::fromString(szTitle);
    CHECK_EXCEPTION()
    
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
#endif
    CHECK_EXCEPTION()
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
    QJniObject active = QNativeInterface::QAndroidApplication::context();
    CHECK_EXCEPTION()

    QJniObject objText = QJniObject::fromString(szText);
    CHECK_EXCEPTION()
    QJniObject objTitle = QJniObject::fromString(szTitle);
    CHECK_EXCEPTION()
    QJniObject objSmallIcon = QJniObject::fromString(szSmallIcon);
    CHECK_EXCEPTION()
    QJniObject objLargeIcon = QJniObject::fromString(szLargeIcon);
    CHECK_EXCEPTION()

    QJniObject::callStaticMethod<void>(
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
    CHECK_EXCEPTION()
#else
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION()
    
    QAndroidJniObject objText = QAndroidJniObject::fromString(szText);
    CHECK_EXCEPTION()
    QAndroidJniObject objTitle = QAndroidJniObject::fromString(szTitle);
    CHECK_EXCEPTION()
    QAndroidJniObject objSmallIcon = QAndroidJniObject::fromString(szSmallIcon);
    CHECK_EXCEPTION()
    QAndroidJniObject objLargeIcon = QAndroidJniObject::fromString(szLargeIcon);
    CHECK_EXCEPTION()
    
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
    CHECK_EXCEPTION()
#endif
    return nRet;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QJniObject BitmapFromQImage(const QImage image)
#else
QAndroidJniObject BitmapFromQImage(const QImage image)
#endif
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
    if(image.isNull())
    {
        qWarning() << "QJniObject: image is null";
        return nullptr;
    }

    QImage img;
    if (image.format() != QImage::Format_RGBA8888)
        img = image.convertToFormat(QImage::Format_RGBA8888);

    if(img.isNull())
    {
        qWarning() << "QJniObject: img is null";
        return nullptr;
    }

    jclass clsConfig = env->FindClass("android/graphics/Bitmap$Config");
    CHECK_EXCEPTION()
    jfieldID fieldId = env->GetStaticFieldID(clsConfig,
                                             "ARGB_8888",
                                             "Landroid/graphics/Bitmap$Config;");
    CHECK_EXCEPTION()
    QJniObject config = env->GetStaticObjectField(clsConfig, fieldId);
    CHECK_EXCEPTION()
    QJniObject bitmap =
        QJniObject::callStaticObjectMethod(
            "android/graphics/Bitmap",
            "createBitmap",
            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;",
            img.width(),
            img.height(),
            config.object<jobject>()
            );
    CHECK_EXCEPTION()
    if(!bitmap.isValid())
        return nullptr;

    AndroidBitmapInfo info;
    if (AndroidBitmap_getInfo(env.getJniEnv(), bitmap.object<jobject>(), &info) < 0) {
        qCritical() << "QAndroidJniObject AndroidBitmap_getInfo fail";
        return nullptr;
    }
    CHECK_EXCEPTION()
    void *pixels;
    if (AndroidBitmap_lockPixels(env.getJniEnv(), bitmap.object<jobject>(), &pixels) < 0) {
        qCritical() << "QAndroidJniObject AndroidBitmap_lockPixels fail";
        return nullptr;
    }
    CHECK_EXCEPTION()
    if (info.stride == uint(img.bytesPerLine())
        && info.width == uint(img.width())
        && info.height == uint(img.height())) {
        memcpy(pixels, img.constBits(), info.stride * info.height);
    } else {
        uchar *bmpPtr = static_cast<uchar *>(pixels);
        const unsigned width = qMin(info.width, (uint)img.width());    //should be the same
        const unsigned height = qMin(info.height, (uint)img.height()); //should be the same
        for (unsigned y = 0; y < height; y++, bmpPtr += info.stride)
            memcpy(bmpPtr, img.constScanLine(y), width);
    }
    AndroidBitmap_unlockPixels(env.getJniEnv(), bitmap.object<jobject>());
    CHECK_EXCEPTION()
    return bitmap;
#else
    QAndroidJniEnvironment env;
    if(image.isNull())
    {
        qWarning() << "QAndroidJniObject: image is null";
        return nullptr;
    }
    
    QImage img;
    if (image.format() != QImage::Format_RGBA8888)
        img = image.convertToFormat(QImage::Format_RGBA8888);
    
    if(img.isNull())
    {
        qWarning() << "QAndroidJniObject: img is null";   
        return nullptr;
    }
    
    jclass clsConfig = env->FindClass("android/graphics/Bitmap$Config");
    CHECK_EXCEPTION()
    jfieldID fieldId = env->GetStaticFieldID(clsConfig,
                                "ARGB_8888",
                                "Landroid/graphics/Bitmap$Config;");
    CHECK_EXCEPTION()
    QAndroidJniObject config = env->GetStaticObjectField(clsConfig, fieldId);
    CHECK_EXCEPTION()
    QAndroidJniObject bitmap = 
            QAndroidJniObject::callStaticObjectMethod(
                "android/graphics/Bitmap",
                "createBitmap",
                "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;",
                img.width(),
                img.height(),
                config.object<jobject>()
            );
    CHECK_EXCEPTION()
    if(!bitmap.isValid())
        return nullptr;

    AndroidBitmapInfo info;
    if (AndroidBitmap_getInfo(env, bitmap.object<jobject>(), &info) < 0) {
        qCritical() << "QAndroidJniObject AndroidBitmap_getInfo fail";
        return nullptr;
    }
    CHECK_EXCEPTION()
    void *pixels;
    if (AndroidBitmap_lockPixels(env, bitmap.object<jobject>(), &pixels) < 0) {
        qCritical() << "QAndroidJniObject AndroidBitmap_lockPixels fail";
        return nullptr;
    }
    CHECK_EXCEPTION()
    if (info.stride == uint(img.bytesPerLine())
            && info.width == uint(img.width())
            && info.height == uint(img.height())) {
        memcpy(pixels, img.constBits(), info.stride * info.height);
    } else {
        uchar *bmpPtr = static_cast<uchar *>(pixels);
        const unsigned width = qMin(info.width, (uint)img.width());    //should be the same
        const unsigned height = qMin(info.height, (uint)img.height()); //should be the same
        for (unsigned y = 0; y < height; y++, bmpPtr += info.stride)
            memcpy(bmpPtr, img.constScanLine(y), width);
    }
    AndroidBitmap_unlockPixels(env, bitmap.object<jobject>());
    CHECK_EXCEPTION()
    return bitmap;
#endif
}

int CNotification::Show(const QString &szText,
                        const QString &szTitle,
                        int nNum,
                        const QImage &smallIcon,
                        const QImage &largeIcon,
                        bool bCallBack)
{
    int nRet = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
    QJniObject active = QNativeInterface::QAndroidApplication::context();
    CHECK_EXCEPTION()

    QJniObject objText = QJniObject::fromString(szText);
    CHECK_EXCEPTION()
    QJniObject objTitle = QJniObject::fromString(szTitle);
    CHECK_EXCEPTION()
    QJniObject objSmallIcon = BitmapFromQImage(smallIcon);
    CHECK_EXCEPTION()
    QJniObject objLargeIcon = BitmapFromQImage(largeIcon);
    CHECK_EXCEPTION()
    QJniObject::callStaticMethod<void>(
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
#else
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION()
    
    QAndroidJniObject objText = QAndroidJniObject::fromString(szText);
    CHECK_EXCEPTION()
    QAndroidJniObject objTitle = QAndroidJniObject::fromString(szTitle);
    CHECK_EXCEPTION()
    QAndroidJniObject objSmallIcon = BitmapFromQImage(smallIcon);
    CHECK_EXCEPTION()
    QAndroidJniObject objLargeIcon = BitmapFromQImage(largeIcon);
    CHECK_EXCEPTION()
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
#endif
    return nRet;
}

template <typename T>
static T GetResourceId(const QString szClass, const QString szId)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return QJniObject::getStaticField<T>(
        szClass.toStdString().c_str(),
        szId.toStdString().c_str());
#else
    return QAndroidJniObject::getStaticField<T>(
                szClass.toStdString().c_str(),
                szId.toStdString().c_str());
#endif
}

template <typename T>
static T GetResourceId(const QString szId)
{
    T ret;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
#else
    QAndroidJniEnvironment env;
#endif
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
    CHECK_EXCEPTION()
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
    QJniObject active = QNativeInterface::QAndroidApplication::context();
    CHECK_EXCEPTION()
    QJniObject objText = QJniObject::fromString(szText);
    CHECK_EXCEPTION()
    QJniObject objTitle = QJniObject::fromString(szTitle);
    CHECK_EXCEPTION()
    int nSmall = GetResourceId<jint>(szSmallIconId);
    CHECK_EXCEPTION()
    int nLarge = GetResourceId<jint>(szLargeIconId);
    CHECK_EXCEPTION()
    qDebug() << "small id: " << nSmall << " large id: " << nLarge;
    QJniObject::callStaticMethod<void>(
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
    CHECK_EXCEPTION()
#else
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION()
    QAndroidJniObject objText = QAndroidJniObject::fromString(szText);
    CHECK_EXCEPTION()
    QAndroidJniObject objTitle = QAndroidJniObject::fromString(szTitle);
    CHECK_EXCEPTION()
    int nSmall = GetResourceId<jint>(szSmallIconId);
    CHECK_EXCEPTION()
    int nLarge = GetResourceId<jint>(szLargeIconId);
    CHECK_EXCEPTION()
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
    CHECK_EXCEPTION()
#endif
    return nRet;
}

int CNotification::Cancel()
{
    int nRet = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
    QJniObject active = QNativeInterface::QAndroidApplication::context();
    CHECK_EXCEPTION()
    QJniObject::callStaticMethod<void>(
        "org/KangLinStudio/QtAndroidUtils/MessageNotification",
        "cancel",
        "(Landroid/content/Context;I)V",
        active.object<jobject>(),
        m_nID);
    CHECK_EXCEPTION()
#else
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION()
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "cancel",
            "(Landroid/content/Context;I)V",
            active.object<jobject>(),
            m_nID);
    CHECK_EXCEPTION()
#endif
    return nRet;
}

int CNotification::CanCelAll()
{
    int nRet = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
    QJniObject active = QNativeInterface::QAndroidApplication::context();
    CHECK_EXCEPTION()
    QJniObject::callStaticMethod<void>(
        "org/KangLinStudio/QtAndroidUtils/MessageNotification",
        "cancelAll",
        "(Landroid/content/Context;)V",
        active.object<jobject>());
    CHECK_EXCEPTION()
#else
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION()
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/QtAndroidUtils/MessageNotification",
            "cancelAll",
            "(Landroid/content/Context;)V",
            active.object<jobject>());
    CHECK_EXCEPTION()
#endif
    return nRet;
}
