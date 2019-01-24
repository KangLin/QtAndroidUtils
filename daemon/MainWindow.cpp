#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AndroidDirectory.h"
#include "AndroidUtils.h"
#include "Notification.h"
#include <QtDebug>
#include <QApplication>

#ifdef BUILD_TEST
    #include "Test.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ShowDirectory();
    CAndroidUtils::InitPermissions();
    bool check = connect(
                 qApp,
                 SIGNAL(applicationStateChanged(Qt::ApplicationState)),
                 this,
                 SLOT(slotApplicationStateChanged(Qt::ApplicationState))
                );
    Q_ASSERT(check);
    check = connect(&m_Notify, SIGNAL(sigOnChilk()),
                    this, SLOT(slotNotifyOnClick()));
    Q_ASSERT(check);
    check = connect(&m_AndroidUtils, SIGNAL(sigSelectPhotos(QStringList)),
                    this, SLOT(slotSelectPhoto(QStringList)));
#ifdef BUILD_TEST
    //CTest::TestTimeout();
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::ShowDirectory()
{
    QString szDir;
    szDir += "AppClassName: " + CAndroidUtils::GetAppClassName() + "\n\n";
    szDir += "AppPackageName: " + CAndroidUtils::GetAppPackageName() + "\n\n";
    szDir += "Apk: : " + CAndroidDirectory::GetApkPath() + "\n\n";
    szDir += "ExternalStorage: " + CAndroidDirectory::GetExternalStorageDirectory() + "\n";
    szDir += "Data: " + CAndroidDirectory::GetDataDirectory() + "\n\n";
    szDir += "Music: " + CAndroidDirectory::GetMusicDirectory() + "\n\n";
    szDir += "Movies: " + CAndroidDirectory::GetMoviesDirectory() + "\n\n";
    szDir += "Picture: " + CAndroidDirectory::GetPictureDirectory() + "\n\n";
    szDir += "App Files: " + CAndroidDirectory::GetAppFilesDirectory() + "\n\n";
    szDir += "Cache: " + CAndroidDirectory::GetAppCacheDirectory() + "\n\n";
    szDir += "Ringtones: " + CAndroidDirectory::GetRingtonesDirectory() + "\n\n";
    ui->teDirectory->setText(szDir);
    return 0;
}

void MainWindow::on_pbVibrate_clicked()
{
    
    CAndroidUtils::Vibrate();
}

void MainWindow::on_pbScreenWake_clicked()
{
    static bool bEnable = false;
    CAndroidUtils::ScreenWake(bEnable);
    bEnable = !bEnable;
}

void MainWindow::on_pbNotification_clicked()
{
    static int n = 0;
    QString text("Text ");
    text += QString::number(n++);
    //CNotification m_Notify;
    m_Notify.Show(text, "Title", 10,
                  QImage(":/icon/app"),
                  QImage(":/icon/app"),
                  true);
}

void MainWindow::on_pbCancelNotify_clicked()
{
    m_Notify.CanCelAll();
}

void MainWindow::slotApplicationStateChanged(Qt::ApplicationState state)
{
    qDebug() << "State: " << state;
}

void MainWindow::slotNotifyOnClick()
{
    qDebug() << "slotNotifyOnClick";
    //showMaximized();
    //activateWindow();
}

void MainWindow::on_pbShare_clicked()
{
    CAndroidUtils::Share("Daemon", "subject", "<html>share content...</html>");
}

void MainWindow::on_pbCamera_clicked()
{
    m_AndroidUtils.OpenCamera();
}

void MainWindow::on_pbPhoto_clicked()
{
    m_AndroidUtils.OpenAlbum();
}

void MainWindow::slotSelectPhoto(QStringList path)
{
    qDebug() << "select photos: " << path;
}
