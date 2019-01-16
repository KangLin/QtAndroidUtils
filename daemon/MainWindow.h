#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Notification.h"

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

    void slotApplicationStateChanged(Qt::ApplicationState state);
    void slotNotifyOnClick();
    
private:
    int ShowDirectory();
    
private:
    Ui::MainWindow *ui;
    CNotification m_Notify;
};

#endif // MAINWINDOW_H
