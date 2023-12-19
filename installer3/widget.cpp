
// TODO: Добавить кнопку Exit.
// перепроверить как отрабывает кнопка Cancel. Она должна отрабатывать так, что бы когда идёт закачка всех/одного файла, и при нажатии на кнопку,
// закачка останавливалась и то что успело скачаться не полностью, должно удалиться.
// продебажить всё приложение!

#include "widget.h"
#include "ui_widget.h"
#include <windows.h>

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QPixmap>
#include <QThread>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    loadConfig();
    connect(ui->SelectAll_checkBox, &QCheckBox::clicked, this, &Widget::onSelectAll_checkBox);
    connect(ui->SelectTargetFolderButton, &QPushButton::clicked, this, &::Widget::onSelectTargetFolder);
    connect(ui->installPushButton, &QPushButton::clicked, this, &Widget::onInstallPushButton);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &Widget::onCancelDownloadPushButton);
    QPixmap pix(":/resources/img/V_LOGO_VISION_GREEN_RGB.png");

    int w = ui->logo->width();
    int h = ui->logo->height();
    ui->logo->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));

    w_checkBoxes.append(ui->AcrobatReader_checkBox);
    w_checkBoxes.append(ui->AnyDesk_checkBox);
    w_checkBoxes.append(ui->Java_checkBox);
    w_checkBoxes.append(ui->ShipSure_checkBox);
    w_checkBoxes.append(ui->Teams_checkBox);
    w_checkBoxes.append(ui->zip_checkBox);
    w_checkBoxes.append(ui->GoogleChrome_checkBox);

    // ui->TargetFolderLineEdit->setText("D:\\testfolder");
    ui->UpdateProgressBar->setMaximum(100);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::loadConfig()
{
    w_config.w_download_path = w_targetFolder;
    // w_config.w_remove_after_install = false;
    w_config.w_apps;
}



void Widget::onSelectAll_checkBox()
{
    bool selectedAllChecked = ui->SelectAll_checkBox->isChecked();
    for (auto checkBox : w_checkBoxes)
    {
        checkBox->setChecked(selectedAllChecked);
    }

}




void Widget::updateProgressBar(int progress)        // Обновление прогресса в прогресс-баре
{
    ui->UpdateProgressBar->setValue(progress);
}



void Widget::onSelectTargetFolder()         //выбор директории для скачивания
{
    w_targetFolder = QFileDialog::getExistingDirectory(this, tr("Select Folder"), QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->TargetFolderLineEdit->setText(w_targetFolder);

}

void Widget::onCancelDownloadPushButton()   // кнопка остановки закачик
{
    for(Downloader *downloader : w_downloaders)
    {
        downloader->onCancelDownload();
    }
    
    // for (Downloader *downloader : w_downloaders)
    // {
    //     if (downloader->isDownloading())
    //     {
    //         QString filePath = ui->TargetFolderLineEdit->text() + "/" + downloader->getFilePath();
    //         QFile file = filePath;
    //         if (file.exists())
    //         {
    //             file.close();
    //             file.remove();
    //             qDebug() << "Removed non-downloaded file: " << filePath;
    //         }
    //     }
    // }
    qDeleteAll(w_downloaders);
    w_downloaders.clear();
    ui->UpdateProgressBar->setValue(0);
    qDebug() << "Cancel downloading...";

}


void Widget::onInstallPushButton()
{
    w_targetFolder = ui->TargetFolderLineEdit->text();
    if(w_targetFolder.isEmpty() || !QDir(w_targetFolder).exists())      // проверка на то, что существует ли директория с таким именем, куда нужно будет скачать файлы.
    {
        qDebug() << "Error: Invalid Target Folder!";
        ui->debugMsg->append("Error: Invalid Target Folder!\n");
        QMessageBox::warning(this, "Warning", "Invalid target folder or does not exist.");
        return;
    }

    for (auto checkBox : w_checkBoxes)                              // если да, то начинаем скачку.
    {
        QString downloadUrl;
        if (checkBox->isChecked())
        {
            QString appName = checkBox->text();

            auto it = w_config.w_apps.find(appName);
            if (it != w_config.w_apps.end())
            {
                downloadUrl = it->second.w_link;
            }
            if(!downloadUrl.isEmpty())
            {
                QString installerPath = ui->TargetFolderLineEdit->text() + "/" + w_config.w_apps[appName].w_pathApp;

                if(QFile::exists(installerPath))
                {
                    qDebug() << "App " << appName << " is already downloaded. Download skipped.";
                    ui->debugMsg->append("App " + appName + " is already downloaded. Download skipped.\n");
                    continue;
                }
                else
                {
                    qDebug() << "App " << appName << " start downloading.";
                    ui->debugMsg->append("App " + appName + " start downloading.\n");
                }

                Downloader *m_downloader = new Downloader(this);
                m_downloader->start(ui->TargetFolderLineEdit->text(), appName, QUrl(downloadUrl));
                connect(m_downloader, &Downloader::downloadProgress, this, &Widget::updateProgressBar);
                connect(m_downloader, &Downloader::downloadFinished, m_downloader, &Downloader::deleteLater);
                connect(m_downloader, &Downloader::downloadFinished, this, [this, appName]() {
                    InstallerRun(appName);
                });
                w_downloaders.append(m_downloader);
            }
        }
    }
}


void Widget::InstallerRun(const QString &appName)       // метод для запуска установки
{
    QProcess *installerProcess = new QProcess(this);
    QString installerExePath = QUrl::fromLocalFile(ui->TargetFolderLineEdit->text() + "\\" + w_config.w_apps[appName].w_pathApp).toLocalFile();       // changed to open method

    qDebug() << "Running installer for " << appName << " from " << installerExePath;
    ui->debugMsg->append("Running installer for " + appName + " from " + installerExePath);

    QFileInfo fileInfo(installerExePath);
    QString fileExtension = fileInfo.suffix().toLower();

    if(fileExtension == "msi" || fileExtension == "msix" || fileExtension == "exe")
    {
        QThread::msleep(100);
        QDesktopServices::openUrl(installerExePath), QUrl::TolerantMode;        // start .msi, .msix installer file
    }

    if (!QFile::exists(installerExePath))
    {
        qDebug() << "Error: Installer file not found!\n";
        ui->debugMsg->append("Error: Installer file not found!\n");
    }
    else
    {
        qDebug() << "Installer file found!\n";
        ui->debugMsg->append("Installer file found!\n");

        installerProcess->start(installerExePath);

        return;
    }
    connect(installerProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [installerProcess]()
    {
        qDebug() << "Installer finished with exit code: " << installerProcess->exitCode() << "\n";


        installerProcess->deleteLater();
    });
}
