#ifndef WIDGET_H
#define WIDGET_H


#include "downloader.h"
#include <unordered_map>
#include <set>


#include <QWidget>
#include <QCheckBox>
#include <QList>
#include <QProcess>
#include <QDesktopServices>
// #include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE


struct DownloadedApp
{
    QString link;
    QString pathApp;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();


private slots:
    // void onAcrobatReader_checkBox();
    // void onAnyDesk_checkBox();
    // void onJava_checkBox();
    // void onShipSure_checkBox();
    // void onTeams_checkBox();
    // void onZip_checkBox();
    // void onGoogleChrome_checkBox();
    void onSelectAll_checkBox();

    void onSelectTargetFolder();
    void onInstallPushButton();
    void onCheckBoxesClicked();
    void onCancelDownloadPushButton();


    void updateProgressBar(int progress);





private:
    QList<QCheckBox *> checkBoxes;
    // QList<QString> selectedApps;
    std::set<QString> selectedApps;
    QString targetFolder;
    Ui::Widget *ui;
    void downloadApplication(const QString &appName, const QString &downloadUrl);
    QList<Downloader*> downloaders;
    void InstallerRun(const QString &appName);

    static std::unordered_map<QString, DownloadedApp> fileMapping;
};
#endif // WIDGET_H
