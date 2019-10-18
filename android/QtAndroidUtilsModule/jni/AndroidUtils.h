/* Java util
 * Author: KangLin(kl222@!26.com) 
 */

#ifndef JAVAUTIL_H
#define JAVAUTIL_H

#include <QObject>

class CActivityResultReceiver;
class Q_CORE_EXPORT CAndroidUtils : public QObject
{
    Q_OBJECT
    
public:
    CAndroidUtils(QObject *parent = nullptr);
    virtual ~CAndroidUtils();

    static int InitPermissions();

    /*
     The following permission must be set in AndroidManifest.xml:
     <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
     <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
    */
    static int InitExternalStoragePermissions();
    /*
     The following permission must be set in AndroidManifest.xml:
     <uses-permission android:name="android.permission.CAMERA" />
    */
    static int InitCameraPermissions();

    /**
     * @brief CallPhone: 
     *  The following permission must be set in AndroidManifest.xml:
     *  <uses-permission id="android.permission.CALL_PHONE" />  
     * @param szNumber
     * @return 
     */
    static int CallPhone(const QString szNumber = QString());
    
    /*
      The following permission must be set in AndroidManifest.xml:
      <uses-permission android:name="android.permission.VIBRATE"/>
    */
    static int Vibrate(long duration = 100);
    /** Screen wake
     *  @para bWake: true, screen wake
     *               false, lock screen
     *  @return boolean: success is true，other is false
     * 
     *  The following permission must be set in AndroidManifest.xml:
     *  <uses-permission android:name="android.permission.WAKE_LOCK"/>
     */
    static bool PowerWakeLock(bool bWake = true);
    
    /** Power sleep and wakeup
     *  @para bSleep: true, power sleep
     *               false, power wakeup
     *  @return boolean: success is true，other is false
     * 
     *  The following permission must be set in AndroidManifest.xml:
     *  <uses-permission android:name="android.permission.DEVICE_POWER"/>
     */
    static bool PowerSleep(bool bSleep = true);
    
    static void Reboot();
    
    /**
     * @brief Install Apk file
     * The following permission must be set in AndroidManifest.xml:
     *   <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"></uses-permission>
     *   <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"></uses-permission>
     *   <uses-permission android:name="android.permission.REQUEST_INSTALL_PACKAGES"></uses-permission>
     * @param szFile
     * @return 
     */
    static int InstallApk(const QString szFile);
    static int UninstallApk(const QString szPackageName);
    
    /**
     * @brief Share 
     * @param title
     * @param subject
     * @param content
     */
    static void Share(const QString &title,
                      const QString &subject,
                      const QString &content,
                      const QString &htmlContext = QString(),
                      const QStringList &imageFiles = QStringList());
 
    
    static QString GetAppClassName();
    static QString GetAppPackageName();
    
    void OpenCamera();
    /**
     * @brief Open android system album
     * @param num: Enable max select number
     */
    void OpenAlbum(int maxSelect = 40);
signals:
    void sigSelectPhotos(QStringList path);
    
    
private:
    friend CActivityResultReceiver;
    CActivityResultReceiver* m_pResultReceiver;
    void SelectPhotos(QStringList path);
};

#endif // JAVAUTIL_H
