#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AndroidDirectory.h"
#include "AndroidUtils.h"
#include "Notification.h"
#include <QtDebug>
#include <QFileDialog>
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
    CAndroidUtils::PowerWakeLock(bEnable);
    bEnable = !bEnable;
    if(bEnable)
        ui->pbScreenWake->setText(tr("Lock screen"));
    else
        ui->pbScreenWake->setText(tr("Unlock screen"));
}

void MainWindow::on_pbPowerSleep_clicked()
{
    static bool bSleep = true;
    CAndroidUtils::PowerSleep(bSleep);
    bSleep = !bSleep;
    if(bSleep)
        ui->pbPowerSleep->setText(tr("Power sleep"));
    else
        ui->pbPowerSleep->setText(tr("Power wakeup"));
}

void MainWindow::on_pbNotification_clicked()
{
    static int n = 0;
    QString text("Text ");
    text += QString::number(n++);
    //CNotification m_Notify;
    m_Notify.Show(text, "Title", 10,
                  QImage(":/icon/app"),
                  QImage(":/icon/application"));
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
    m_AndroidUtils.Share("titie", "subject", "content", "<html>share html content...</html>", path);
}

void MainWindow::on_ptPhone_clicked()
{
    CAndroidUtils::CallPhone("12345678901");
}

void MainWindow::on_ptInstall_clicked()
{
    QString szFile;
#ifdef Q_OS_ANDROID
    // In Android, the file dialog is not shown maximized by the static
    // function, which looks weird, since the dialog doesn't have borders or
    // anything. To make sure it's shown maximized, we won't be using
    // QFileDialog::getExistingDirectory().
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.showMaximized();
    dialog.exec();
    if (!dialog.selectedFiles().isEmpty()) {
        szFile = dialog.selectedFiles().front();
    }
#else
    szFile = QFileDialog::getExistingDirectory(this,
                                          QString(),
                      ui->lineEditRootPath->text());
#endif
    CAndroidUtils::InstallApk(szFile);
}

void MainWindow::on_ptUnistall_clicked()
{
    CAndroidUtils::UninstallApk("org.KangLinStudio.QtAndroidUtils.daemon");
}
