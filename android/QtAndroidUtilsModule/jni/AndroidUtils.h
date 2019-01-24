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
    static bool ScreenWake(bool bWake = true);
    
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
