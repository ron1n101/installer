#include "widget.h"
#include "downloader.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


//  переписать этот блок
    checkBoxes.append(ui->AcrobatReader_checkBox);
    checkBoxes.append(ui->AnyDesk_checkBox);
    checkBoxes.append(ui->Java_checkBox);
    checkBoxes.append(ui->ShipSure_checkBox);
    checkBoxes.append(ui->Teams_checkBox);
    checkBoxes.append(ui->zip_checkBox);
//


    connect(ui->AcrobatReader_checkBox, &QCheckBox::clicked, this, &Widget::onAcrobatReader_checkBox);
    connect(ui->AnyDesk_checkBox, &QCheckBox::clicked, this, &Widget::onAnyDesk_checkBox);
    connect(ui->Java_checkBox, &QCheckBox::clicked, this, &Widget::onJava_checkBox);
    connect(ui->ShipSure_checkBox, &QCheckBox::clicked, this, &Widget::onShipSure_checkBox);
    connect(ui->Teams_checkBox, &QCheckBox::clicked, this, &Widget::onTeams_checkBox);
    connect(ui->zip_checkBox, &QCheckBox::clicked, this, &Widget::onZip_checkBox);

    connect(ui->SelectAll_checkBox, &QCheckBox::clicked, this, &Widget::onSelectAll_checkBox);
    connect(ui->SelectTargetFolderButton, &QPushButton::clicked, this, &Widget::onSelectTargetFolder);
    connect(ui->installPushButton, &QPushButton::clicked, this, &Widget::onInstallPushButton);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &Widget::onCancelDownloadPushButton);

    for(auto checkBox : checkBoxes)
    {
        connect(checkBox, &QCheckBox::clicked, this, &Widget::onCheckBoxesClicked);
    }

    // connect slots progressbar (add after coding downloader.h)
    // connect(ui->UpdateProgressBar, &QProgressBar::valueChanged, this, &Widget::)

    // создать кнопку отмены загрузки
    // создать кнопку выбрать всё

    // УЗНАТЬ ПРО МЕТОД СКАЧИВАНИЯ БОЛЬШИХ ФАЙЛОВ ЧЕРЕЗ QT И ТАКЖЕ НАПИСАТЬ МЕТОД СКАЧИВАНИЯ.
    // ПРОГУГЛИТЬ ИНФУ ПО ПОТОКИ.


}

Widget::~Widget()
{
    delete ui;
}

void Widget::onCheckBoxesClicked()
{
    selectedApps.clear();
    for (auto checkBox : checkBoxes)
    {
        if (checkBox->isChecked())
        {
            selectedApps.append(checkBox->text());
        }
    }
}

void Widget::onSelectAll_checkBox()
{
    bool selectedAllChecked = ui->SelectAll_checkBox->isChecked();
    for (auto checkBox : checkBoxes)
    {
        checkBox->setChecked(selectedAllChecked);
    }
    onCheckBoxesClicked();
}
void Widget::updateProgressBar(int progress)
{
    // Обновление прогресса в вашем прогресс-баре
    ui->UpdateProgressBar->setValue(progress);
}



void Widget::onSelectTargetFolder()
{
    targetFolder = QFileDialog::getExistingDirectory(this, tr("Select Folder"), QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->TargetFolderLineEdit->setText(targetFolder);
}

void Widget::onCancelDownloadPushButton()
{
    for(Downloader *downloader : downloaders)
    {
        downloader->onCancelDownload();
    }
    ui->UpdateProgressBar->setMaximum(100);
    ui->UpdateProgressBar->setValue(0);
    qDebug() << "Cancel downloading...";

}



// void Widget::onProgress

void Widget::onAcrobatReader_checkBox()
{
    QString appName = "Acrobat Reader";
    QString downloadUrl = "https://admdownload.adobe.com/rdcm/installers/live/readerdc64.exe";

    if(ui->AcrobatReader_checkBox->isChecked())
    {
        selectedApps.append(appName);
    }
}

void Widget::onAnyDesk_checkBox()
{
    QString appName = "AnyDesk";
    QString downloadUrl = "https://download.anydesk.com/AnyDesk.exe";

    if (ui->AnyDesk_checkBox->isChecked())
    {
        selectedApps.append(appName);
    }
}


void Widget::onJava_checkBox()
{
    QString appName = "Java";
    QString downloadUrl = "https://sdlc-esd.oracle.com/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/jre-8u391-windows-x64.exe?GroupName=JSC&FilePath=/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/jre-8u391-windows-x64.exe&BHost=javadl.sun.com&File=jre-8u391-windows-x64.exe&AuthParam=1701611483_4dd0fec506c8b98b2381a5befff0304b&ext=.exe";
    if (ui->Java_checkBox->isChecked())
    {
        selectedApps.append(appName);
    }
}

void Widget::onShipSure_checkBox()
{
    QString appName = "ShipSure";
    QString downloadUrl = "https://ssdeploy.v.group/west/FrameworkPublicLiveWest2.msi";
    if (ui->ShipSure_checkBox->isChecked())
    {
        selectedApps.append(appName);
    }
}


void Widget::onTeams_checkBox()
{
    QString appName = "Teams";
    QString downloadUrl = "https://statics.teams.cdn.office.net/production-windows-x86/enterprise/webview2/lkg/MSTeams-x86.msix";
    if (ui->Teams_checkBox->isChecked())
    {
        selectedApps.append(appName);
    }
}

void Widget::onZip_checkBox()
{
    QString appName = "7Zip";
    QString downloadUrl = "https://www.7-zip.org/a/7z2301-x64.exe";

    if (ui->zip_checkBox->isChecked())
    {
        selectedApps.append(appName);
    }
}


void Widget::onInstallPushButton()
{
    for(auto &appName : selectedApps)
    {
        QString downloadUrl;
        if (appName == "7Zip")
        {
            qDebug() << "Downloading 7Zip";
            downloadUrl = "https://www.7-zip.org/a/7z2301-x64.exe";
        }

        if (appName == "AnyDesk")
        {
            qDebug() << "Downloading AnyDesk";
            downloadUrl = "https://download.anydesk.com/AnyDesk.exe";
        }

        if (appName == "Acrobat Reader")
        {
            qDebug() << "Downloading Acrobat Reader";
            downloadUrl = "https://admdownload.adobe.com/rdcm/installers/live/readerdc64.exe";
        }

        if (appName == "Java")
        {
            qDebug() << "Downloading Java";
            downloadUrl = "https://sdlc-esd.oracle.com/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/jre-8u391-windows-x64.exe?GroupName=JSC&FilePath=/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/jre-8u391-windows-x64.exe&BHost=javadl.sun.com&File=jre-8u391-windows-x64.exe&AuthParam=1701611483_4dd0fec506c8b98b2381a5befff0304b&ext=.exe";
        }

        if (appName == "ShipSure")
        {
            qDebug() << "Downloading SS";
            downloadUrl = "https://ssdeploy.v.group/west/FrameworkPublicLiveWest2.msi";
        }

        if (appName == "Teams")
        {
            qDebug() << "Downloading Teams";
            downloadUrl = "https://statics.teams.cdn.office.net/production-windows-x86/enterprise/webview2/lkg/MSTeams-x86.msix";
        }
        // downloadApplication(appName, downloadUrl);

        // if (!downloadUrl.isEmpty())
        // {
        //     Downloader *downloader = new Downloader(this);
        //     downloader->start(targetFolder, appName, QUrl(downloadUrl));
        //     connect(downloader, &Downloader::downloadFinished, downloader, &Downloader::deleteLater);
        // }
        if (!downloadUrl.isEmpty())
        {
            Downloader *downloader = new Downloader(this);
            downloader->start(targetFolder, appName, QUrl(downloadUrl));
            connect(downloader, &Downloader::downloadProgress, this, &Widget::updateProgressBar);
            connect(downloader, &Downloader::downloadFinished, downloader, &Downloader::deleteLater);

            downloaders.append(downloader);
        }


    }
}





