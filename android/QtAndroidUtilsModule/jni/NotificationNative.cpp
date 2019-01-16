#include "NotificationNative.h"

#include <QDebug>

#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
    qDebug() << __FILE__ << "(" << __LINE__ << ")" << "exception occured";\
    env->ExceptionClear(); \
    }

void MessageNotificationOnClickCallBack(JNIEnv *env, jobject thiz, jint id)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);
    CNotificationNative::instant()->MessageNotificationOnClickCallBack(id);
}

static JNINativeMethod g_NotificationMethods[] = {
    {"MessageNotificationOnClickCallBack", "(I)V", (void *)MessageNotificationOnClickCallBack}
};

CNotificationNative* CNotificationNative::instant()
{
    static CNotificationNative* p = NULL;
    if(!p) p = new CNotificationNative();
    return p;
}

CNotificationNative::CNotificationNative(QObject *parent) : QObject(parent)
{
    QAndroidJniEnvironment env;
    jclass cls = env.findClass("org/KangLinStudio/QtAndroidUtils/MessageNotification");
    QAndroidJniObject notify(cls);
    m_Notify = notify;
    env->RegisterNatives(cls, g_NotificationMethods,
             sizeof(g_NotificationMethods) / sizeof (g_NotificationMethods[0]));
    CHECK_EXCEPTION();
    QAndroidJniObject active = QtAndroid::androidActivity();
    m_Notify.callMethod<void>(
            "init",
            "(Landroid/content/Context;)V",
            active.object<jobject>()
            );
    CHECK_EXCEPTION();
}

void CNotificationNative::MessageNotificationOnClickCallBack(int id)
{
    emit sigMessageNotificationOnClickCallBack(id);
}
