#ifndef WIDGET_H
#define WIDGET_H


#include "downloader.h"

#include <QWidget>
#include <QCheckBox>
#include <QList>


QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();


private slots:
    void onAcrobatReader_checkBox();
    void onAnyDesk_checkBox();
    void onJava_checkBox();
    void onShipSure_checkBox();
    void onTeams_checkBox();
    void onZip_checkBox();
    void onSelectAll_checkBox();

    void onSelectTargetFolder();
    void onInstallPushButton();
    void onCheckBoxesClicked();
    void onCancelDownloadPushButton();


    void updateProgressBar(int progress);




private:
    QList<QCheckBox *> checkBoxes;
    QList<QString> selectedApps;
    QString targetFolder;
    Ui::Widget *ui;
    void downloadApplication(const QString &appName, const QString &downloadUrl);
    QList<Downloader*> downloaders;

    // Downloader cancel_downloader;
};
#endif // WIDGET_H
