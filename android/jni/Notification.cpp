#include "Notification.h"
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#include <QDebug>

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
            "org/KangLinStudio/MessageNotification",
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
            "org/KangLinStudio/MessageNotification",
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

int CNotification::Cancel()
{
    int nRet = 0;
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    QAndroidJniObject::callStaticMethod<void>(
            "org/KangLinStudio/MessageNotification",
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
            "org/KangLinStudio/MessageNotification",
            "cancelAll",
            "(Landroid/content/Context;)V",
            active.object<jobject>());
    CHECK_EXCEPTION();
    return nRet;
}
