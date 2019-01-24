/*
 * THIS IS PRIVATE CLASS， YOU CAN NOT USE IT.
 */

// Author: KangLin(kl222@!26.com) 

#ifndef CNOTIFICATIONNATIVE_H
#define CNOTIFICATIONNATIVE_H

#include <QObject>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>

class CNotificationNative : public QObject
{
    Q_OBJECT
private:
    explicit CNotificationNative(QObject *parent = nullptr);

public:
    static CNotificationNative* instant();
    void MessageNotificationOnClickCallBack(int id);
    
signals:
     void sigMessageNotificationOnClickCallBack(int id);
     
public slots:
private:
    QAndroidJniObject m_Notify;
};

#endif // CNOTIFICATIONNATIVE_H
