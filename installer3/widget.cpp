
//

// УЗНАТЬ ПРО МЕТОД СКАЧИВАНИЯ БОЛЬШИХ ФАЙЛОВ ЧЕРЕЗ QT И ТАКЖЕ НАПИСАТЬ МЕТОД СКАЧИВАНИЯ.
// ПРОГУГЛИТЬ ИНФУ ПО ПОТОКИ.

#include "widget.h"
#include "ui_widget.h"
#include <windows.h>

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

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

    checkBoxes.append(ui->AcrobatReader_checkBox);
    checkBoxes.append(ui->AnyDesk_checkBox);
    checkBoxes.append(ui->Java_checkBox);
    checkBoxes.append(ui->ShipSure_checkBox);
    checkBoxes.append(ui->Teams_checkBox);
    checkBoxes.append(ui->zip_checkBox);
    checkBoxes.append(ui->GoogleChrome_checkBox);
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

        // тут был файлмаппинг!!!

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
    installPath = targetFolder;
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
        fileMapping["Acrobat Reader"] = "readerdc64.exe";
    }
}

void Widget::onAnyDesk_checkBox()
{
    QString appName = "AnyDesk";
    QString downloadUrl = "https://download.anydesk.com/AnyDesk.exe";

    if (ui->AnyDesk_checkBox->isChecked())
    {
        selectedApps.append(appName);
        fileMapping["AnyDesk"] = "AnyDesk.exe";
    }
}


void Widget::onJava_checkBox()
{
    QString appName = "Java";
    QString downloadUrl = "https://sdlc-esd.oracle.com/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/JavaSetup8u391.exe?GroupName=JSC&FilePath=/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/JavaSetup8u391.exe&BHost=javadl.sun.com&File=JavaSetup8u391.exe&AuthParam=1701801365_844a8fea876d4e7a276b47057e9e7f51&ext=.exe";
    if (ui->Java_checkBox->isChecked())
    {
        selectedApps.append(appName);
        fileMapping["Java"] = "JavaSetup8u391.exe";
    }
}

void Widget::onShipSure_checkBox()
{
    QString appName = "ShipSure";
    QString downloadUrl = "https://ssdeploy.v.group/west/FrameworkPublicLiveWest2.msi";
    if (ui->ShipSure_checkBox->isChecked())
    {
        selectedApps.append(appName);
        fileMapping["ShipSure"] = "FrameworkPublicLiveWest2.msi";
    }
}


void Widget::onTeams_checkBox()
{
    QString appName = "Teams";
    QString downloadUrl = "https://statics.teams.cdn.office.net/production-windows-x86/enterprise/webview2/lkg/MSTeams-x86.msix";
    if (ui->Teams_checkBox->isChecked())
    {
        selectedApps.append(appName);
        fileMapping["Teams"] = "MSTeams-x86.msix";
    }
}

void Widget::onZip_checkBox()
{
    QString appName = "7Zip";
    QString downloadUrl = "https://www.7-zip.org/a/7z2200-x64.msi";

    if (ui->zip_checkBox->isChecked())
    {
        selectedApps.append(appName);
        fileMapping["7Zip"] = "7z2200-x64.msi";
    }
}

void Widget::onGoogleChrome_checkBox()
{
    QString appName = "Google Chrome";
    QString downloadUrl = "https://dl.google.com/tag/s/appguid%3D%7B8A69D345-D564-463C-AFF1-A69D9E530F96%7D%26iid%3D%7B0F9ED7A0-7BDE-5A4E-2175-3A01A3CF3319%7D%26lang%3Duk%26browser%3D4%26usagestats%3D1%26appname%3DGoogle%2520Chrome%26needsadmin%3Dprefers%26ap%3Dx64-stable-statsdef_1%26installdataindex%3Dempty/update2/installers/ChromeSetup.exe";

    if(ui->GoogleChrome_checkBox->isChecked())
    {
        selectedApps.append(appName);
        fileMapping["Google Chrome"] = "ChromeSetup.exe";
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
            downloadUrl = "https://www.7-zip.org/a/7z2200-x64.msi";
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
            downloadUrl = "https://sdlc-esd.oracle.com/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/JavaSetup8u391.exe?GroupName=JSC&FilePath=/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/JavaSetup8u391.exe&BHost=javadl.sun.com&File=JavaSetup8u391.exe&AuthParam=1701801365_844a8fea876d4e7a276b47057e9e7f51&ext=.exe";
        }

        if (appName == "ShipSure")
        {
            qDebug() << "Downloading SS";
            downloadUrl = "https://ssdeploy.v.group/west/FrameworkPublicLiveWest2.msi";
        }

        if (appName == "Google Chrome")
        {
            qDebug() << "Downloading Google Chrome";
            downloadUrl = "https://dl.google.com/tag/s/appguid%3D%7B8A69D345-D564-463C-AFF1-A69D9E530F96%7D%26iid%3D%7B0F9ED7A0-7BDE-5A4E-2175-3A01A3CF3319%7D%26lang%3Duk%26browser%3D4%26usagestats%3D1%26appname%3DGoogle%2520Chrome%26needsadmin%3Dprefers%26ap%3Dx64-stable-statsdef_1%26installdataindex%3Dempty/update2/installers/ChromeSetup.exe";
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

            connect(downloader, &Downloader::downloadFinished, this, [this, appName]() {
                InstallerRun(appName);
            });
            downloaders.append(downloader);
        }
    }
}

// void Widget::InstallerRun(const QString &appName)
// {
//     QProcess *installerProcess = new QProcess(this);
//     QString installerPath = installPath + "/" + fileMapping[appName];

//     if(QFile::exists(installerPath))
//     {
//         qDebug() << "Running for " << appName << " from " << installerPath;
//         installerProcess->start(installerPath);
//     }
//     else
//     {
//         qDebug() << "Error: installer file not found!!!";
//     }

//     connect(installerProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this, installerProcess]()
//             {
//         qDebug() << "Installer finished with exit code: " << installerProcess ->exitCode();
//         installerProcess->deleteLater();
//     });
// }

void Widget::InstallerRun(const QString &appName)
{
    QProcess *installerProcess = new QProcess(this);

    if (fileMapping.contains(appName))
    {
        QString installerExePath = installPath + "/" + fileMapping[appName];
        qDebug() << "Running installer for " << appName << " from " << installerExePath;

        if (QFile::exists(installerExePath))
        {
            qDebug() << "Installer file found!";
            installerProcess->start(installerExePath);
        }
        else
        {
            qDebug() << "Error: Installer file not found!";
            return;
        }

        connect(installerProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [installerProcess]()
                {
                    qDebug() << "Installer finished with exit code: " << installerProcess ->exitCode();
                    installerProcess->deleteLater();
                });
    }
    else
    {
        qDebug() << "Error: App name not found in fileMapping!";
    }
}

