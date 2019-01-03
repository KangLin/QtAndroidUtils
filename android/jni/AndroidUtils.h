/* Java util
 * Author: KangLin(kl222@!26.com) 
 */

#ifndef JAVAUTIL_H
#define JAVAUTIL_H

#include <QObject>
#include <QtAndroidExtras>

class Q_CORE_EXPORT CAndroidUtils : public QObject
{
    Q_OBJECT
    
public:
    CAndroidUtils(QObject *parent = nullptr);
    virtual ~CAndroidUtils();
    
    static int InitPermissions();
    static int Vibrate(long duration = 100);
    /** Screen wake
     *  @para bWake: true, screen wake
     *               false, lock screen
     *  @return boolean: success is true，other is false
     */
    static bool ScreenWake(bool bWake = true);
    
    static QString GetAppClassName();
    static QString GetAppPackageName();
};

#endif // JAVAUTIL_H
