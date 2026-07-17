/**
 * @note   THIS IS PRIVATE CLASS， YOU CAN NOT USE IT.
 * @brief  native call back
 * @author Kang Lin (kl222@!26.com) 
 */

#ifndef NATIVECALLBACK_H
#define NATIVECALLBACK_H

#pragma once

#include <QObject>

// @note THIS IS PRIVATE CLASS， YOU CAN NOT USE IT.
class CNativeCallback : public QObject
{
    Q_OBJECT
private:
    explicit CNativeCallback(QObject *parent = nullptr);
    virtual ~CNativeCallback();

public:
    static CNativeCallback* instant();
    void MessageNotificationOnClickCallBack(int id);
    
signals:
     void sigMessageNotificationOnClickCallBack(int id);
};

#endif // NATIVECALLBACK_H
