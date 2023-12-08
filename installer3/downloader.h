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


    // метод для запуска скачивания
    void start (const QString& targetFolder, const QString &appName , const QUrl &url);
public slots:
    void onCancelDownload();

signals:
    void downloadFinished();
    void downloadProgress(int progress);




private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();


private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;
    QString targetFolder;
    QString appName;
    QUrl downloadUrl;

    bool isCancel;

};

#endif // DOWNLOADER_H
