#include "downloadcty.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
#include <QDebug>

DownLoadCTY::DownLoadCTY(const QString _kontestDir) : QObject(0)
{    
    kontestDir = _kontestDir;
   //qDebug() << "DownLoadCTY::DownLoadCTY(): " << kontestDir << endl;
    result = -1;  // Error unknown        

    urld = QString("http://www.country-files.com/bigcty/cty.csv").toLocal8Bit();
    //urld = QString("http://www.country-files.com/cty/cty.csv").toLocal8Bit();
    url.setUrl(urld.toLocal8Bit());
    //url.setEncodedUrl(urld.toLocal8Bit());
    request.setRawHeader( "User-Agent" , "KLog" );
    request.setUrl(url);

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));

}

DownLoadCTY::~DownLoadCTY()
{
   //qDebug() << "DownLoadCTY::~DownLoadCTY"  << endl;
}


void DownLoadCTY::setTarget(const QString &t){
   //qDebug() << "DownLoadCTY::setTarget: " << t << endl;
    this->target = t;
}



 void DownLoadCTY::slotDownloadFinished(QNetworkReply *data)
{
   //qDebug() << "DownLoadCTY::downloadFinished"  << endl;

    QFile localFile("cty.csv");

   //qDebug() << "DownLoadCTY::downloadFinished - error: " << data->errorString() << endl;

    result = data->error();
   //qDebug() << "DownLoadCTY::downloadFinished - Result = " << QString::number(result) << endl;

    if (result == QNetworkReply::NoError)
    {
        if (!localFile.open(QIODevice::WriteOnly))
        {
           //qDebug() << "DownLoadCTY::downloadFinished: CTY file could not be created!" << endl;
            emit actionReturnDownload(-1);
            //return;
        }
        else
        {
            const QByteArray sdata = data->readAll();
            localFile.write(sdata);
           //qDebug() << sdata;
            if (localFile.flush())
            {
             //qDebug() << "DownLoadCTY::downloadFinished: CTY file Flushed 100%!" << endl;

            }
            else
            {
             //qDebug() << "DownLoadCTY::downloadFinished: CTY file NOT flushed 100%!" << endl;
            }
            localFile.close();


           //qDebug() << "DownLoadCTY::downloadFinished: and CTY file created!" << endl;
            emit actionReturnDownload(result);
        }
    }
    else
    {
       //qDebug() << "DownLoadCTY::downloadFinished - Result = UNDEFINED = " << QString::number(result)  << endl;
    }

    //TODO: Check if the cty.csv file is already existing.

    //qDebug() << "DownLoadCTY::downloadFinished - Result = " << QString::number(result) << endl;
    emit done();


}

int DownLoadCTY::download()
{
   //qDebug() << "DownLoadCTY::download " << endl;

    QNetworkReply *reply= manager.get(request);

    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotDownloadProgress(qint64,qint64)));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotErrorManagement(QNetworkReply::NetworkError)), Qt::QueuedConnection);
    //qDebug() << "DownLoadCTY::download: " << QString::number(result) << endl;
    return 1;
}

void DownLoadCTY::slotDownloadProgress(qint64 received, qint64 total) {
   //qDebug() << "DownLoadCTY::downloadProgress: " << QString::number(received) << "/" << QString::number(total) << endl;
    //qDebug() << received << total;

    emit actionShowProgres(received, total);
}

void DownLoadCTY::slotErrorManagement(QNetworkReply::NetworkError networkError)
{
    //qDebug() << "DownLoadCTY::slotErrorManagement: " << QString::number(networkError) << endl;

    result = networkError;

    if (result == QNetworkReply::NoError)
    {
       //qDebug() << "DownLoadCTY::downloadFinished: No error" << endl;
    }
    else if (result == QNetworkReply::HostNotFoundError)
    {
        //qDebug() << "DownLoadCTY::downloadFinished: Host not found" << endl;
    }
    else
    {
       //qDebug() << "DownLoadCTY::downloadFinished: ERROR: " << QString::number(result) << endl;
    }

    actionError(result);
}
