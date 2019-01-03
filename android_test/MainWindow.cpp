#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AndroidDirectory.h"
#include "JavaUtils.h"

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
    szDir += "AppClassName: " + CJavaUtils::GetAppClassName() + "\n";
    szDir += "AppPackageName: " + CJavaUtils::GetAppPackageName() + "\n";
    szDir += "Apk: : " + CAndroidDirectory::GetApkPath() + "\n";
    szDir += "ExternalStorage: " + CAndroidDirectory::GetExternalStorageDirectory() + "\n";
    szDir += "Data: " + CAndroidDirectory::GetDataDirectory() + "\n";
    szDir += "Music: " + CAndroidDirectory::GetMusicDirectory() + "\n";
    szDir += "Movies: " + CAndroidDirectory::GetMoviesDirectory() + "\n";
    szDir += "Picture: " + CAndroidDirectory::GetPictureDirectory() + "\n";
    szDir += "App Files: " + CAndroidDirectory::GetAppFilesDirectory() + "\n";
    szDir += "Cache: " + CAndroidDirectory::GetAppCacheDirectory() + "\n";
    szDir += "Ringtones: " + CAndroidDirectory::GetRingtonesDirectory() + "\n";
    ui->teDirectory->setText(szDir);
    return 0;
}
