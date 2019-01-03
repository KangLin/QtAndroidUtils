/* Java util
 * Author: KangLin(kl222@!26.com) 
 */

#include "JavaUtils.h"
#include <QtAndroid>
#include <QAndroidJniExceptionCleaner>

#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
    qDebug() << __FILE__ << "(" << __LINE__ << ")" << "exception occured";\
    env->ExceptionClear(); \
    }

CJavaUtils::CJavaUtils(QObject *parent) :  QObject(parent)
{
}

CJavaUtils::~CJavaUtils()
{
}

int CJavaUtils::InitPermissions()
{
    int nRet = 0;
    static bool inited = false;
    if(inited)
        return 0;
    inited = true;

    QAndroidJniEnvironment env;
    
    /*
     The following permission must be set in AndroidManifest.xml:
     <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
     <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
    */
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    QStringList lstPermission;
    QtAndroid::PermissionResult r;
    r = QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
    if(QtAndroid::PermissionResult::Denied == r)
    {
        lstPermission << "android.permission.READ_EXTERNAL_STORAGE";
    }
    r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(QtAndroid::PermissionResult::Denied == r)
    {
        lstPermission << "android.permission.WRITE_EXTERNAL_STORAGE";
    }
    if(!lstPermission.isEmpty())
        QtAndroid::requestPermissionsSync(lstPermission);
#else
    
    /* Checks if the app has permission to read and write to device storage
     * If the app does not has permission then the user will be prompted to
     * grant permissions, When android > 6.0(SDK API > 23)
     */
    QAndroidJniObject mainActive = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    if(mainActive.isValid())
        QAndroidJniObject::callStaticMethod<void>(
                "org/KangLinStudio/Utils",
                "verifyStoragePermissions",
                "(Ljava/lang/Object;)V",
                mainActive.object<jobject>());
        CHECK_EXCEPTION();
    else {
        qDebug() << "QtAndroid::androidActivity() isn't valid\n";
    }
#endif
    
    return nRet;
}

/*
  The following permission must be set in AndroidManifest.xml:
  <uses-permission android:name="android.permission.VIBRATE"/>
*/
int CJavaUtils::Vibrate(long duration)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject active = QtAndroid::androidActivity();
    CHECK_EXCEPTION();
    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField(
                "android/content/Context",
                "VIBRATOR_SERVICE",
                "Ljava/lang/String;"
                );
    CHECK_EXCEPTION();
    QAndroidJniObject vibrateService = active.callObjectMethod(
                "getSystemService",
                "(Ljava/lang/String;)Ljava/lang/Object;",
                name.object<jstring>());
    CHECK_EXCEPTION();
    jlong d = duration;
    vibrateService.callMethod<void>("vibrate", "(J)V", d);
    CHECK_EXCEPTION();
    return 0;
}

/*
  The following permission must be set in AndroidManifest.xml:
  <uses-permission android:name="android.permission.WAKE_LOCK"/>
*/
bool CJavaUtils::ScreenWake(bool bWake)
{
    QAndroidJniEnvironment env;
    static QAndroidJniObject screenLock;
    if(!screenLock.isValid())
    {
        QAndroidJniObject activity = QtAndroid::androidActivity();
        CHECK_EXCEPTION();
        QAndroidJniObject name = QAndroidJniObject::getStaticObjectField(
                    "android/content/Context",
                    "POWER_SERVICE",
                    "Ljava/lang/String;"
                    );
        CHECK_EXCEPTION();
        QAndroidJniObject powerService = activity.callObjectMethod(
                    "getSystemService",
                    "(Ljava/lang/String;)Ljava/lang/Object;",
                    name.object<jstring>());
        CHECK_EXCEPTION();
        QAndroidJniObject tag = QAndroidJniObject::fromString("QtJniWakeLock");
        screenLock = powerService.callObjectMethod(
                    "newWakeLock",
                    "(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;",
                    10, //SCREEN_BRIGHT_WAKE_LOCK
                    tag.object<jstring>()
                    );
        CHECK_EXCEPTION();
    }
    
    if(!screenLock.isValid())
        return false;
    
    if(bWake)
        screenLock.callMethod<void>("acquire");
    else
        screenLock.callMethod<void>("release");
    CHECK_EXCEPTION();
    return true;    
}

QString CJavaUtils::GetAppClassName()
{
    QAndroidJniObject appInfo = QtAndroid::androidActivity().callObjectMethod(
                    "getApplicationInfo",
                    "()Landroid/content/pm/ApplicationInfo;");
    
    return appInfo.getObjectField<jstring>("className").toString();
        
}

QString CJavaUtils::GetAppPackageName()
{
    return QtAndroid::androidActivity().callObjectMethod<jstring>("getPackageName").toString();
}
