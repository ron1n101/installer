#include "downloader.h"
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDir>
#include <QEventLoop>

Downloader::Downloader(QWidget *parent) : QWidget(parent), m_manager(new QNetworkAccessManager), m_reply(nullptr), m_file(nullptr)
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &Downloader::onDownloadFinished);
}



// start downloading
void Downloader::start(const QString &m_targetFolder, const QString &m_appName ,const QUrl &url)
{
    this->m_targetFolder = m_targetFolder;
    this->m_appName = m_appName;
    m_downloadUrl = url;

    QString fileName = QFileInfo(url.path()).fileName();
    QString filePath = m_targetFolder + "/" + fileName;

    if (QFile::exists(filePath))
    {
        emit downloadFinished();
        return;

    }

    m_file = new QFile (filePath);
    if(!m_file -> open(QIODevice::WriteOnly))
    {
        emit downloadFinished();
        delete m_file;          //
        m_file = nullptr;       //
        return;
    }


    QNetworkRequest request(url);
    m_reply = m_manager->get(request);

    connect(m_reply, &QNetworkReply::downloadProgress, this, &Downloader::onDownloadProgress);
}





bool Downloader::isDownloading() const
{
    return m_reply && m_reply->isRunning();
}

QString Downloader::getFilePath() const
{
    return m_file ? m_file->fileName() : QString();
}



// Cancel Download method
void Downloader::onCancelDownload()
{
    if (m_reply)
    {
        m_reply->abort();
        m_reply->deleteLater(); // Переносим удаление m_reply в асинхронный режим с использованием deleteLater

        if (m_file)
        {
            if(m_file->isOpen())
            {
                m_file->close();
            }
            m_file->remove();
            m_file->deleteLater(); // Переносим удаление m_file в асинхронный режим с использованием deleteLater
            m_file = nullptr;
        }
    }
}

// Close and Remove method
void Downloader::closeAndRemove()
{
    if(m_file)
    {
        m_file->close();
        m_file->remove();
    }
}


// Download progress-bar
void Downloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
    emit downloadProgress(progress);

    if (m_file && m_file->isOpen())
    {
        m_file->write(m_reply->readAll());
    }
}


// Finished download method
void Downloader::onDownloadFinished()
{
    if (m_file)
    {
        if (m_file->isOpen())
        {
            m_file->close();
        }
        if (QFile::exists(m_file->fileName()))
        {
            delete m_file;
            m_file = nullptr;
        }
        else
        {
            qDebug() << "Error: File not found: " << m_file->fileName();
        }
    }

    if (m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    emit downloadFinished();
}
