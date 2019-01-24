/* Java util
 * Author: KangLin(kl222@!26.com) 
 */

#include "AndroidUtils.h"
#include <QtAndroid>
#include <QAndroidJniExceptionCleaner>
#include <QtAndroidExtras>
#include "ActivityResultReceiver.h"
#include <QAndroidIntent>

#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
    qDebug() << __FILE__ << "(" << __LINE__ << ")" << "exception occured";\
    env->ExceptionClear(); \
    }


CAndroidUtils::CAndroidUtils(QObject *parent) :  QObject(parent)
{
    m_pResultReceiver = new CActivityResultReceiver(this);
    
}

CAndroidUtils::~CAndroidUtils()
{
    if(m_pResultReceiver)
        delete m_pResultReceiver;
}

int CAndroidUtils::InitPermissions()
{
    int nRet = 0;
    static bool inited = false;
    if(inited)
        return 0;
    inited = true;
    nRet = InitExternalStoragePermissions();
    nRet = InitCameraPermissions();
    return nRet;
}

int CAndroidUtils::InitExternalStoragePermissions()
{
    int nRet = 0;
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
    {
        QAndroidJniObject::callStaticMethod<void>(
                "org/KangLinStudio/QtAndroidUtils/Utils",
                "verifyStoragePermissions",
                "(Landroid/app/Activity;)V",
                mainActive.object<jobject>());
        CHECK_EXCEPTION();
    }
    else
    {
        qDebug() << "QtAndroid::androidActivity() isn't valid\n";
    }
#endif

    return nRet;
}

int CAndroidUtils::InitCameraPermissions()
{
    int nRet = 0;
    QAndroidJniEnvironment env;
    
    /*
     The following permission must be set in AndroidManifest.xml:
     <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
     <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
    */
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    QStringList lstPermission;
    QtAndroid::PermissionResult r;
    r = QtAndroid::checkPermission("android.permission.CAMERA");
    if(QtAndroid::PermissionResult::Denied == r)
    {
        lstPermission << "android.permission.CAMERA";
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
    {
        QAndroidJniObject::callStaticMethod<void>(
                "org/KangLinStudio/QtAndroidUtils/Utils",
                "verifyCameraPermissions",
                "(Landroid/app/Activity;)V",
                mainActive.object<jobject>());
        CHECK_EXCEPTION();
    }
    else
    {
        qDebug() << "QtAndroid::androidActivity() isn't valid\n";
    }
#endif
    return nRet;
}

/*
  The following permission must be set in AndroidManifest.xml:
  <uses-permission android:name="android.permission.VIBRATE"/>
*/
int CAndroidUtils::Vibrate(long duration)
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
bool CAndroidUtils::ScreenWake(bool bWake)
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

QString CAndroidUtils::GetAppClassName()
{
    QAndroidJniObject appInfo = QtAndroid::androidActivity().callObjectMethod(
                    "getApplicationInfo",
                    "()Landroid/content/pm/ApplicationInfo;");
    
    return appInfo.getObjectField<jstring>("className").toString();
        
}

QString CAndroidUtils::GetAppPackageName()
{
    return QtAndroid::androidActivity().callObjectMethod<jstring>("getPackageName").toString();
}

void CAndroidUtils::Share(const QString &title, const QString &subject,
                          const QString &content)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject jTitle = QAndroidJniObject::fromString(title);
    QAndroidJniObject jSubject = QAndroidJniObject::fromString(subject);
    QAndroidJniObject jContent = QAndroidJniObject::fromString(content);
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject::callStaticMethod<void>(
        "org/KangLinStudio/QtAndroidUtils/Utils",
        "shareText",
        "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
        activity.object<jobject>(),
        jTitle.object<jstring>(),
        jSubject.object<jstring>(),
        jContent.object<jstring>()
   );
   CHECK_EXCEPTION();
}

void CAndroidUtils::OpenCamera()
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = QtAndroid::androidActivity();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))   
    //https://github.com/DmcSDK/MediaPickerPoject
    QAndroidIntent intent(activity, "com.dmcbig.mediapicker.TakePhotoActivity");
    CHECK_EXCEPTION();
    QtAndroid::startActivity(intent.handle(),
                             CActivityResultReceiver::RESULT_CODE_CAMERA,
                             m_pResultReceiver);
    CHECK_EXCEPTION();
#else
    
    jclass clsTakePhotoActivity =
            env.findClass("com/dmcbig/mediapicker/TakePhotoActivity");
    QAndroidJniObject intent("android.content.Intent",
                             "(Landroid/content/Context;Ljava/lang/Class;)V",
                             activity.object<jobject>(),
                             clsTakePhotoActivity);
    CHECK_EXCEPTION();
    QtAndroid::startActivity(intent,
                             CActivityResultReceiver::RESULT_CODE_CAMERA,
                             m_pResultReceiver);
    CHECK_EXCEPTION();
#endif   
}

void CAndroidUtils::OpenAlbum(int maxSelect)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject maxSelectCount = 
            QAndroidJniObject::getStaticObjectField<jstring>(
                "com.dmcbig.mediapicker.PickerConfig",
                "MAX_SELECT_COUNT");
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))   
    //https://github.com/DmcSDK/MediaPickerPoject
    QAndroidIntent intent(activity, "com.dmcbig.mediapicker.PickerActivity");
    //intent.putExtra(maxSelectCount.toString(), QVariant(maxSelect));
    intent.handle().callObjectMethod("putExtra",
                            "(Ljava/lang/String;I)Landroid/content/Intent;",
                            maxSelectCount.object<jstring>(),
                            maxSelect);
    CHECK_EXCEPTION();
    QtAndroid::startActivity(intent.handle(),
                             CActivityResultReceiver::RESULT_CODE_PHOTO,
                             m_pResultReceiver);
    CHECK_EXCEPTION();
#else
    
    jclass clsPickerActivity = env.findClass("com/dmcbig/mediapicker/PickerActivity");
    QAndroidJniObject intent("android.content.Intent",
                             "(Landroid/content/Context;Ljava/lang/Class;)V",
                             activity.object<jobject>(),
                             clsPickerActivity);
    CHECK_EXCEPTION();//*/
    /*
    QAndroidJniObject objPA = 
        QAndroidJniObject::fromString("com.dmcbig.mediapicker.PickerActivity");
    QAndroidJniObject objPickerActivity = 
        QAndroidJniObject::callStaticObjectMethod(
                "java/lang/Class",
                "forName",
                "(Ljava/lang/String;)Ljava/lang/Class;",
                objPA.object<jstring>());
    CHECK_EXCEPTION();
    QAndroidJniObject intent("android.content.Intent",
                             "(Landroid/content/Context;Ljava/lang/Class;)V",
                             activity.object<jobject>(),
                             objPickerActivity.object<jclass>());
    CHECK_EXCEPTION();//*/
    intent.callObjectMethod("putExtra",
                            "(Ljava/lang/String;I)Landroid/content/Intent;",
                            maxSelectCount.object<jstring>(),
                            maxSelect);
    QtAndroid::startActivity(intent,
                             CActivityResultReceiver::RESULT_CODE_PHOTO,
                             m_pResultReceiver);
    CHECK_EXCEPTION();
#endif   
}

void CAndroidUtils::SelectPhotos(QStringList path)
{
    emit sigSelectPhotos(path);
}
