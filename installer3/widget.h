#ifndef WIDGET_H
#define WIDGET_H


#include "downloader.h"
#include <unordered_map>


#include <QWidget>
#include <QCheckBox>
#include <QList>
#include <QTextEdit>
#include <QProcess>
#include <QDesktopServices>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE


struct DownloadedApp
{
    QString w_link;
    QString w_pathApp;
};


struct Config
{
    QString w_download_path;
    std::unordered_map<QString, DownloadedApp> w_apps =
    {
        {"Acrobat Reader", {"https://admdownload.adobe.com/rdcm/installers/live/readerdc64.exe", "readerdc64.exe"}},
        {"AnyDesk", {"https://download.anydesk.com/AnyDesk.exe", "AnyDesk.exe"}},
        {"Java", {"https://sdlc-esd.oracle.com/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/jre-8u391-windows-x64.exe?GroupName=JSC&FilePath=/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/jre-8u391-windows-x64.exe&BHost=javadl.sun.com&File=jre-8u391-windows-x64.exe&AuthParam=1702223777_f2dc2e8a6084b578fef2b1e0aa38be1d&ext=.exe", "jre-8u391-windows-x64.exe"}},
        {"ShipSure", {"https://ssdeploy.v.group/west/FrameworkPublicLiveWest2.msi","FrameworkPublicLiveWest2.msi"}},
        {"Teams", {"https://statics.teams.cdn.office.net/production-windows-x86/enterprise/webview2/lkg/MSTeams-x86.msix", "MSTeams-x86.msix"}},
        {"7Zip", {"https://www.7-zip.org/a/7z1900.exe", "7z1900.exe"}},
        {"Google Chrome", {"https://dl.google.com/tag/s/appguid%3D%7B8A69D345-D564-463C-AFF1-A69D9E530F96%7D%26iid%3D%7BC0F7877C-26E0-FBAA-936D-51B2D960C642%7D%26lang%3Den%26browser%3D4%26usagestats%3D0%26appname%3DGoogle%2520Chrome%26needsadmin%3Dprefers%26ap%3Dx64-stable-statsdef_1%26installdataindex%3Dempty/chrome/install/ChromeStandaloneSetup64.exe", "ChromeStandaloneSetup64.exe"}},
    };
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    ~Widget();


private slots:

    void onSelectAll_checkBox();
    void onSelectTargetFolder();
    void onInstallPushButton();
    void onCancelDownloadPushButton();
    void updateProgressBar(int progress);





private:
    QList<QCheckBox *> w_checkBoxes;
    QString w_targetFolder;
    Ui::Widget *ui;
    QList<Downloader*> w_downloaders;
    QTextEdit *debugText;
    // static std::unordered_map<QString, DownloadedApp> fileMapping;
    Config w_config;

    void loadConfig();
    void downloadApplication(const QString &appName, const QString &downloadUrl);
    void InstallerRun(const QString &appName);
    bool m_isCancel;
};
#endif // WIDGET_H
