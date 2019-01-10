#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AndroidDirectory.h"
#include "AndroidUtils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ShowDirectory();
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
