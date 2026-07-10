// Author: Kang Lin (kl222@!26.com)

#include <QLoggingCategory>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QJniObject>
    #include <QtCore/private/qandroidextras_p.h>
#else
    #include <QAndroidJniEnvironment>
    #include <QAndroidJniObject>
    #include <QtAndroid>
#endif

#include "NativeCallback.h"

#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
    qDebug() << __FILE__ << "(" << __LINE__ << ")" << "exception occurred";\
    env->ExceptionClear(); \
    }

static void MessageNotificationOnClickCallBack(JNIEnv *env, jobject thiz, jint id)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)
    CNativeCallback::instant()->MessageNotificationOnClickCallBack(id);
}

static JNINativeMethod g_NativeCallBack[] = {
    {"MessageNotificationOnClickCallBack", "(I)V", (void *)MessageNotificationOnClickCallBack}
};

CNativeCallback::CNativeCallback(QObject *parent) : QObject(parent)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
#else
    QAndroidJniEnvironment env;
#endif
    jclass cls = env->FindClass("org/KangLinStudio/QtAndroidUtils/MessageNotification");
    env->RegisterNatives(cls, g_NativeCallBack,
             sizeof(g_NativeCallBack) / sizeof (g_NativeCallBack[0]));
    CHECK_EXCEPTION()
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniObject active = QNativeInterface::QAndroidApplication::context();
    static QJniObject notify(cls);
#else
    QAndroidJniObject active = QtAndroid::androidActivity();
    static QAndroidJniObject notify(cls);
#endif
    notify.callMethod<void>(
            "init",
            "(Landroid/content/Context;)V",
            active.object<jobject>()
            );
    CHECK_EXCEPTION()
}

CNativeCallback::~CNativeCallback()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniEnvironment env;
#else
    QAndroidJniEnvironment env;
#endif
    jclass cls = env->FindClass("org/KangLinStudio/QtAndroidUtils/MessageNotification");
    env->RegisterNatives(cls, g_NativeCallBack,
                         sizeof(g_NativeCallBack) / sizeof (g_NativeCallBack[0]));
    CHECK_EXCEPTION()
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniObject active = QNativeInterface::QAndroidApplication::context();
    static QJniObject notify(cls);
#else
    QAndroidJniObject active = QtAndroid::androidActivity();
    static QAndroidJniObject notify(cls);
#endif
    notify.callMethod<void>(
            "clean",
            "(Landroid/content/Context;)V",
            active.object<jobject>()
            );
    CHECK_EXCEPTION()
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
