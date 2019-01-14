#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#pragma once

#include <QObject>
#include <QImage>

class CNotification : public QObject
{
    Q_OBJECT
public:
    explicit CNotification(QObject *parent = nullptr);
    
    int Show(const QString &szText,
             const QString &szTitle = QString(),
             int nNum = 0);
    int Show(const QString &szText,
             const QString &szTitle,
             int nNum,
             const QString &szSmallIcon,
             const QString &szLargeIcon = QString());
    int Show(const QString &szText,
             const QString &szTitle,
             int nNum,
             const QImage &smallIcon,
             const QImage &largeIcon = QImage());
    
    int Cancel();
    static int CanCelAll();
    
signals:
    
public slots:
    
private:
    int m_nID;
};

#endif // NOTIFICATION_H
