/* Java util
 * Author: KangLin(kl222@!26.com) 
 */

#ifndef JAVAUTIL_H
#define JAVAUTIL_H

#include <QObject>

class Q_CORE_EXPORT CAndroidUtils : public QObject
{
    Q_OBJECT
    
public:
    CAndroidUtils(QObject *parent = nullptr);
    virtual ~CAndroidUtils();
    
    /*
     The following permission must be set in AndroidManifest.xml:
     <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
     <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
    */
    static int InitPermissions();
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
    
    static QString GetAppClassName();
    static QString GetAppPackageName();
};

#endif // JAVAUTIL_H
