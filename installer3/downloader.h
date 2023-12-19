#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QCheckBox>


class QNetworkReply;
class QFile;



class Downloader : public QWidget
{
    Q_OBJECT
    using BaseClass = QObject;
public:
    explicit Downloader(QWidget *parent = nullptr);


    // bool isDownloading() const;
    // QString getFilePath() const;

    // метод для запуска скачивания
    void start (const QString& targetFolder, const QString &m_appName , const QUrl &url);
public slots:
    void onCancelDownload();

signals:
    void downloadFinished();
    void downloadProgress(int progress);




private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();


private:
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    QFile *m_file;
    QString m_targetFolder;
    QString m_appName;
    QUrl m_downloadUrl;

    // bool m_isCancel;

};

#endif // DOWNLOADER_H
