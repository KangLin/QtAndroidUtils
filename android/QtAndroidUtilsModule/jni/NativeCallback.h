/*
 * THIS IS PRIVATE CLASS， YOU CAN NOT USE IT.
 */

// Author: KangLin(kl222@!26.com) 

#ifndef NATIVECALLBACK_H
#define NATIVECALLBACK_H

#include <QObject>

class CNativeCallback : public QObject
{
    Q_OBJECT
private:
    explicit CNativeCallback(QObject *parent = nullptr);

public:
    static CNativeCallback* instant();
    void MessageNotificationOnClickCallBack(int id);
    
signals:
     void sigMessageNotificationOnClickCallBack(int id);
};

#endif // NATIVECALLBACK_H
