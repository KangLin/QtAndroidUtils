#include "NativeCallback.h"
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

static void MessageNotificationOnClickCallBack(JNIEnv *env, jobject thiz, jint id)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);
    CNativeCallback::instant()->MessageNotificationOnClickCallBack(id);
}

static JNINativeMethod g_NativeCallBack[] = {
    {"MessageNotificationOnClickCallBack", "(I)V", (void *)MessageNotificationOnClickCallBack}
};

CNativeCallback::CNativeCallback(QObject *parent) : QObject(parent)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject cls("org/KangLinStudio/QtAndroidUtils/MessageNotification");
    env->RegisterNatives(cls.object<jclass>(), g_NativeCallBack,
             sizeof(g_NativeCallBack) / sizeof (g_NativeCallBack[0]));
    CHECK_EXCEPTION();
    QAndroidJniObject active = QtAndroid::androidActivity();
    static QAndroidJniObject notify(cls);
    notify.callMethod<void>(
            "init",
            "(Landroid/content/Context;)V",
            active.object<jobject>()
            );
    CHECK_EXCEPTION();
}

CNativeCallback::~CNativeCallback()
{
    QAndroidJniEnvironment env;
    QAndroidJniObject cls("org/KangLinStudio/QtAndroidUtils/MessageNotification");
    env->RegisterNatives(cls.object<jclass>(), g_NativeCallBack,
             sizeof(g_NativeCallBack) / sizeof (g_NativeCallBack[0]));
    CHECK_EXCEPTION();
    QAndroidJniObject active = QtAndroid::androidActivity();
    static QAndroidJniObject notify(cls);
    notify.callMethod<void>(
            "clean",
            "(Landroid/content/Context;)V",
            active.object<jobject>()
            );
    CHECK_EXCEPTION();
}

CNativeCallback* CNativeCallback::instant()
{
    static CNativeCallback* p = NULL;
    if(!p) p = new CNativeCallback();
    return p;
}

void CNativeCallback::MessageNotificationOnClickCallBack(int id)
{
    emit sigMessageNotificationOnClickCallBack(id);
}
