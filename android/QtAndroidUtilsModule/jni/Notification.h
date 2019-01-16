#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#pragma once

#include <QObject>
#include <QImage>

/*
 * The following permission must be set in AndroidManifest.xml:
 * <activity android:launchMode="singleInstance" />
 */
class CNotification : public QObject
{
    Q_OBJECT
public:
    explicit CNotification(QObject *parent = nullptr);
    virtual ~CNotification();
    
    int Show(const QString &szText,
             const QString &szTitle = QString(),
             int nNum = 0,
             bool bCallBack = false);
    int Show(const QString &szText,
             const QString &szTitle,
             int nNum,
             const QString &szSmallIcon,
             const QString &szLargeIcon = QString(),
             bool bCallBack = false);
    int Show(const QString &szText,
             const QString &szTitle,
             int nNum,
             const QImage &smallIcon,
             const QImage &largeIcon = QImage(),
             bool bCallBack = false);
    
    int Cancel();
    static int CanCelAll();
    
signals:
    void sigOnChilk();
    
public slots:
    void slotOnClick(int id);

protected:
    virtual void OnClick();

private:
    int m_nID;
};

#endif // NOTIFICATION_H
