#include "downloader.h"
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDir>


Downloader::Downloader(QWidget *parent) : QWidget(parent), manager(new QNetworkAccessManager), reply(nullptr), file(nullptr)
{
    connect(manager, &QNetworkAccessManager::finished, this, &Downloader::onDownloadFinished);
}




void Downloader::start(const QString &targetFolder, const QString &appName ,const QUrl &url)
{
    this->targetFolder = targetFolder;
    this->appName = appName;
    downloadUrl = url;

    QString fileName = QFileInfo(url.path()).fileName();
    QString filePath = targetFolder + "/" + fileName;

    if (QFile::exists(filePath))
    {
        emit downloadFinished();
        return;

    }

    file = new QFile (filePath);
    if(!file -> open(QIODevice::WriteOnly))
    {
        emit downloadFinished();
        return;
    }


    QNetworkRequest request(url);
    reply = manager->get(request);

    connect(reply, &QNetworkReply::downloadProgress, this, &Downloader::onDownloadProgress);
}

void Downloader::onCancelDownload()
{
    if (reply)
    {
        reply->abort();
    }
}



void Downloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
    emit downloadProgress(progress);

    if (file)
    {
        file->write(reply->readAll());
    }
}

void Downloader::onDownloadFinished()
{
    if (file)
    {
        file->close();
        delete file;
        file = nullptr;
    }

    if (reply)
    {
        reply->deleteLater();
        reply = nullptr;
    }
    emit downloadFinished();
}
