/* Android Directory
 * Author: KangLin(kl222@!26.com) 
 */

#include <QCoreApplication>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QJniObject>
#else
    #include <QAndroidJniObject>
    #include <QtAndroid>
#endif

#include "AndroidDirectory.h"

CAndroidDirectory::CAndroidDirectory(QObject *parent) :  QObject(parent)
{
}

CAndroidDirectory::~CAndroidDirectory()
{}

QString CAndroidDirectory::GetExternalStorageDirectory()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    auto d = QJniObject::callStaticObjectMethod(
        "android/os/Environment",
        "getExternalStorageDirectory",
        "()Ljava/io/File;"
        );
#else
    auto d = QAndroidJniObject::callStaticObjectMethod(
        "android/os/Environment",
        "getExternalStorageDirectory",
        "()Ljava/io/File;"
        );
#endif
    return d.toString();
}

QString CAndroidDirectory::GetDataDirectory()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    auto dataDir = QJniObject::callStaticObjectMethod(
        "android/os/Environment",
        "getDataDirectory",
        "()Ljava/io/File;"
        );
#else
    auto dataDir = QAndroidJniObject::callStaticObjectMethod(
        "android/os/Environment",
        "getDataDirectory",
        "()Ljava/io/File;"
        );
#endif
    return dataDir.toString();
}

QString CAndroidDirectory::GetExternalStoragePublicDirectory(QString name)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    auto n = QJniObject::getStaticObjectField(
        "android/os/Environment",
        name.toStdString().c_str(),
        "Ljava/lang/String;"
        );

    auto dir = QJniObject::callStaticObjectMethod(
        "android/os/Environment",
        "getExternalStoragePublicDirectory",
        "(Ljava/lang/String;)Ljava/io/File;",
        n.object<jstring>()
        );
#else
    auto n = QAndroidJniObject::getStaticObjectField(
        "android/os/Environment",
        name.toStdString().c_str(),
        "Ljava/lang/String;"
        );
    
    auto dir = QAndroidJniObject::callStaticObjectMethod(
        "android/os/Environment",
        "getExternalStoragePublicDirectory",
        "(Ljava/lang/String;)Ljava/io/File;",
        n.object<jstring>()
        );
#endif
    return dir.toString();
}

QString CAndroidDirectory::GetPictureDirectory()
{
    return GetExternalStoragePublicDirectory("DIRECTORY_DCIM");
}

QString CAndroidDirectory::GetMusicDirectory()
{
    return GetExternalStoragePublicDirectory("DIRECTORY_MUSIC");
}

QString CAndroidDirectory::GetMoviesDirectory()
{
    return GetExternalStoragePublicDirectory("DIRECTORY_MOVIES");
}

QString CAndroidDirectory::GetRingtonesDirectory()
{
    return GetExternalStoragePublicDirectory("DIRECTORY_RINGTONES");
}

QString CAndroidDirectory::GetAppFilesDirectory()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
#else
    QAndroidJniObject activity = QtAndroid::androidActivity();
#endif
    return activity.callObjectMethod(
                       "getFilesDir",
                       "()Ljava/io/File;"
                       ).toString();
}

QString CAndroidDirectory::GetAppCacheDirectory()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
#else
    QAndroidJniObject activity = QtAndroid::androidActivity();
#endif
    return activity.callObjectMethod(
                       "getCacheDir",
                       "()Ljava/io/File;"
                       ).toString();
}

QString CAndroidDirectory::GetApkPath()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    QJniObject appInfo = activity.callObjectMethod(
        "getApplicationInfo",
        "()Landroid/content/pm/ApplicationInfo;");
    return appInfo.getObjectField(
                      "sourceDir", "Ljava/lang/String;").toString();
#else
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject appInfo = activity.callObjectMethod(
        "getApplicationInfo",
        "()Landroid/content/pm/ApplicationInfo;");
    return appInfo.getObjectField(
                      "sourceDir", "Ljava/lang/String;").toString();
#endif
}
