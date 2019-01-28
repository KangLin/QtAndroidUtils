#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Notification.h"
#include "AndroidUtils.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void on_pbVibrate_clicked();
    void on_pbScreenWake_clicked();
    void on_pbNotification_clicked();
    void on_pbCancelNotify_clicked();
    void on_pbShare_clicked();
    void on_pbCamera_clicked();
    void on_pbPhoto_clicked();

    void slotApplicationStateChanged(Qt::ApplicationState state);
    void slotNotifyOnClick();

    void slotSelectPhoto(QStringList path);
    
    void on_ptPhone_clicked();
    
    void on_ptInstall_clicked();
    
    void on_ptUnistall_clicked();
    
private:
    int ShowDirectory();
    
private:
    Ui::MainWindow *ui;
    CNotification m_Notify;
    CAndroidUtils m_AndroidUtils;
};

#endif // MAINWINDOW_H
