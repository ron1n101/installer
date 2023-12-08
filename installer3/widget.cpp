
//

// УЗНАТЬ ПРО МЕТОД СКАЧИВАНИЯ БОЛЬШИХ ФАЙЛОВ ЧЕРЕЗ QT И ТАКЖЕ НАПИСАТЬ МЕТОД СКАЧИВАНИЯ.
// ПРОГУГЛИТЬ ИНФУ ПО ПОТОКИ.

#include "widget.h"
#include "ui_widget.h"
#include <windows.h>

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>


std::unordered_map<QString, DownloadedApp> Widget::fileMapping =
    {
        {"Acrobat Reader", {"https://admdownload.adobe.com/rdcm/installers/live/readerdc64.exe", "readerdc64.exe"}},
        {"AnyDesk", {"https://download.anydesk.com/AnyDesk.exe", "AnyDesk.exe"}},
        {"Java", {"https://sdlc-esd.oracle.com/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/JavaSetup8u391.exe?GroupName=JSC&FilePath=/ESD6/JSCDL/jdk/8u391-b13/b291ca3e0c8548b5a51d5a5f50063037/JavaSetup8u391.exe&BHost=javadl.sun.com&File=JavaSetup8u391.exe&AuthParam=1701801365_844a8fea876d4e7a276b47057e9e7f51&ext=.exe", "JavaSetup8u391.exe"}},
        {"ShipSure", {"https://ssdeploy.v.group/west/FrameworkPublicLiveWest2.msi","FrameworkPublicLiveWest2.msi"}},
        {"Teams", {"https://statics.teams.cdn.office.net/production-windows-x86/enterprise/webview2/lkg/MSTeams-x86.msix", "MSTeams-x86.msix"}},
        {"7Zip", {"https://www.7-zip.org/a/7z2200-x64.msi", "7z2200-x64.msi"}},
        {"Google Chrome", {"https://dl.google.com/tag/s/appguid%3D%7B8A69D345-D564-463C-AFF1-A69D9E530F96%7D%26iid%3D%7B0F9ED7A0-7BDE-5A4E-2175-3A01A3CF3319%7D%26lang%3Duk%26browser%3D4%26usagestats%3D1%26appname%3DGoogle%2520Chrome%26needsadmin%3Dprefers%26ap%3Dx64-stable-statsdef_1%26installdataindex%3Dempty/update2/installers/ChromeSetup.exe", "ChromeSetup.exe"}}
};


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // connect(ui->AcrobatReader_checkBox, &QCheckBox::clicked, this, &Widget::onAcrobatReader_checkBox);
    // connect(ui->AnyDesk_checkBox, &QCheckBox::clicked, this, &Widget::onAnyDesk_checkBox);
    // connect(ui->Java_checkBox, &QCheckBox::clicked, this, &Widget::onJava_checkBox);
    // connect(ui->ShipSure_checkBox, &QCheckBox::clicked, this, &Widget::onShipSure_checkBox);
    // connect(ui->Teams_checkBox, &QCheckBox::clicked, this, &Widget::onTeams_checkBox);
    // connect(ui->zip_checkBox, &QCheckBox::clicked, this, &Widget::onZip_checkBox);

    connect(ui->SelectAll_checkBox, &QCheckBox::clicked, this, &Widget::onSelectAll_checkBox);
    connect(ui->SelectTargetFolderButton, &QPushButton::clicked, this, &Widget::onSelectTargetFolder);
    connect(ui->installPushButton, &QPushButton::clicked, this, &Widget::onInstallPushButton);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &Widget::onCancelDownloadPushButton);


    checkBoxes.append(ui->AcrobatReader_checkBox);
    checkBoxes.append(ui->AnyDesk_checkBox);
    checkBoxes.append(ui->Java_checkBox);
    checkBoxes.append(ui->ShipSure_checkBox);
    checkBoxes.append(ui->Teams_checkBox);
    checkBoxes.append(ui->zip_checkBox);
    checkBoxes.append(ui->GoogleChrome_checkBox);
    for(auto checkBox : checkBoxes)
    {
        connect(checkBox, &QCheckBox::clicked, this, &Widget::onCheckBoxesClicked);
    }
    ui->TargetFolderLineEdit->setText("D:\\testfolder");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onCheckBoxesClicked()      // вместо того что бы подключать все чекбоксы, была написана функция, которая считывает выбранные приложения по названиям из appName
{
    selectedApps.clear();
    for (auto checkBox : checkBoxes)
    {
        if (checkBox->isChecked())
        {
            QString appName = checkBox->text();
            // selectedApps.insert(checkBox->text());
            selectedApps.insert(appName);
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



void Widget::onSelectTargetFolder()         //выбор директории для скачивания
{
    targetFolder = QFileDialog::getExistingDirectory(this, tr("Select Folder"), QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->TargetFolderLineEdit->setText(targetFolder);
    // installPath = targetFolder;
}

void Widget::onCancelDownloadPushButton()   // кнопка остановки закачик
{
    for(Downloader *downloader : downloaders)
    {
        downloader->onCancelDownload();
    }
    ui->UpdateProgressBar->setMaximum(100);
    ui->UpdateProgressBar->setValue(0);
    qDebug() << "Cancel downloading...";

}


// void Widget::onAcrobatReader_checkBox()
// {
//     QString appName = "Acrobat Reader";
//     if(ui->AcrobatReader_checkBox->isChecked())
//     {
//         selectedApps.insert(appName);
//     }
//     else
//     {
//         // TODO: find and remove selectedApps
//     }
// }

// void Widget::onAnyDesk_checkBox()
// {
//     static const QString appName = "AnyDesk";
//     if (ui->AnyDesk_checkBox->isChecked())
//     {
//         selectedApps.insert(appName);
//     }
//     else
//     {
//         // TODO: find and remove selectedApps
//     }
// }


// void Widget::onJava_checkBox()
// {
//     static const QString appName = "Java";
//     if (ui->Java_checkBox->isChecked())
//     {
//         selectedApps.insert(appName);
//     }
//     else
//     {
//         // TODO: find and remove selectedApps
//     }
// }

// void Widget::onShipSure_checkBox()
// {
//     static const QString appName = "ShipSure";
//     if (ui->ShipSure_checkBox->isChecked())
//     {
//         selectedApps.insert(appName);
//     }
//     else
//     {
//         // TODO: find and remove selectedApps
//     }
// }


// void Widget::onTeams_checkBox()
// {
//     static const QString appName = "Teams";
//     if (ui->Teams_checkBox->isChecked())
//     {
//         selectedApps.insert(appName);
//     }
//     else
//     {
//         // TODO: find and remove selectedApps
//     }
// }

// void Widget::onZip_checkBox()
// {
//     static const QString appName = "7Zip";
//     if (ui->zip_checkBox->isChecked())
//     {
//         selectedApps.insert(appName);
//     }
//     else
//     {
//         // TODO: find and remove selectedApps
//     }
// }

// void Widget::onGoogleChrome_checkBox()
// {
//     static const QString appName = "Google Chrome";
//     if(ui->GoogleChrome_checkBox->isChecked())
//     {
//         selectedApps.insert(appName);
//         // fileMapping["Google Chrome"] = "ChromeSetup.exe";
//     }
// }

void Widget::onInstallPushButton()          // кнопка скачивания
{
    for(auto &appName : selectedApps)
    {
        QString downloadUrl;

        auto it = fileMapping.find(appName);
        if(it != fileMapping.end())
        {
            downloadUrl = it->second.link;
        }


        if (!downloadUrl.isEmpty())
        {
            Downloader *downloader = new Downloader(this);
            downloader->start(ui->TargetFolderLineEdit->text(), appName, QUrl(downloadUrl));
            connect(downloader, &Downloader::downloadProgress, this, &Widget::updateProgressBar);
            connect(downloader, &Downloader::downloadFinished, downloader, &Downloader::deleteLater);

            connect(downloader, &Downloader::downloadFinished, this, [this, appName]() {
                InstallerRun(appName);
            });
            downloaders.append(downloader);
        }
    }
}


void Widget::InstallerRun(const QString &appName)       // метод для запуска установки
{
    QProcess *installerProcess = new QProcess(this);

    QString installerExePath = ui->TargetFolderLineEdit->text() + "/" + fileMapping[appName].pathApp;
    qDebug() << "Running installer for " << appName << " from " << installerExePath;

    QFileInfo fileInfo(installerExePath);
    QString fileExtension = fileInfo.suffix().toLower();

    if(fileExtension == "msi" || fileExtension == "msix")
    {
        QDesktopServices::openUrl(installerExePath), QUrl::TolerantMode;        // start .msi, .msix installer file
    }

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
