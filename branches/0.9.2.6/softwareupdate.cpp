#include "softwareupdate.h"
//#include <QDebug>

SoftwareUpdate::SoftwareUpdate(const QString _klogVersion) : QObject(0)
{
    //qDebug() << "SoftwareUpdate::SoftwareUpdate(): " << _klogDir << endl;
    util = new Utilities;
    updateDialog = new SoftwareUpdateDialog();
    latestVersion = "0.0";
    //updateDialog->setVersion(_klogVersion, fale);
    //toUpdate = false;
    repositoryFound = false;
    url = new QUrl;
    //klogDir = _klogDir;
    setVersion(_klogVersion);
    //klogVersion = _klogVersion;
    //latestVersion = "0.0";
    callsign = QString();
    //result = -1;  // Error unknown
    //reply = new QNetworkReply;
    manager = new QNetworkAccessManager(this);
     //request = new QNetworkRequest(this);    
    //request.setUrl(QUrl("http://localhost"));
    //request.setUrl(QUrl("https://download.savannah.gnu.org/releases/klog/"));
    //request.setUrl(QUrl("http://www.klog.xyz/download"));
    setTheURL("http://download.klog.xyz");
    //request.setUrl(QUrl("http://download.klog.xyz"));
    messageShown = false;
    setHeader();

}

SoftwareUpdate::~SoftwareUpdate()
{

}

void SoftwareUpdate::setTheURL(QString _url)
{
   //qDebug() << "SoftwareUpdate::setTheURL: " << _url << endl;
    request.setUrl(QUrl(_url));
}

void SoftwareUpdate::setVersion(const QString _klogVersion)
{
    klogVersion = _klogVersion;
    //latestVersion = "0.0";
    setHeader();
}

void SoftwareUpdate::slotReadyRead()
{
   //qDebug() << "SoftwareUpdate::slotReadyRead: " << endl;
}

void SoftwareUpdate::slotError(int _p)
{
    //qDebug() << "SoftwareUpdate::slotError: " << endl;
}

void SoftwareUpdate::slotDownloadFinished(QNetworkReply *reply)
{
//qDebug() << "SoftwareUpdate::slotDownloadFinished"  << endl;

    QUrl url = reply->url();
 //qDebug() << "SoftwareUpdate::slotDownloadFinished - URL: " << url.toString()  << endl;

    //QMessageBox msgBox;
    //QString aux;
    //aux.clear();
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error())
    {
       //qDebug() << "SoftwareUpdate::slotDownloadFinished: reply error"  << endl;
/*
      fprintf(stderr, "Updates %s failed: %s\n",
              url.toEncoded().constData(),
              qPrintable(reply->errorString()));


      //errorCode = query.lastError().number();

      msgBox.setIcon(QMessageBox::Warning);
      aux = tr("The following error code was received when trying to check for updates: ");
      msgBox.setText(aux + reply->errorString());
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      int ret = msgBox.exec();

*/

    }
    else if (!redirectionTarget.isNull())
    {
        repositoryFound = false;
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
       //qDebug() << "SoftwareUpdate::slotDownloadFinished: Redirect: " << newUrl.toString() << endl;
        url = newUrl;
        reply->deleteLater();
        setTheURL(url.toString());
        connectToURL();
        return;
    }
    else
    {
        if (checkUpdates(reply))
        {
         //qDebug() << "SoftwareUpdate::slotDownloadFinished checkupdates true"  << endl;
            if (repositoryFound)
            {
               //qDebug() << "SoftwareUpdate::slotDownloadFinished repository found"  << endl;
                if(latestVersion>klogVersion)
                {
                   //qDebug() << "SoftwareUpdate::slotDownloadFinished checkupdates should update!"  << endl;
                    updateDialog->setVersion(latestVersion, true);
                }
                updateDialog->show();
                latestVersion = klogVersion;
                repositoryFound = false;
            }
        }
        else
        {
            if (repositoryFound && messageShown)
            {
                updateDialog->setVersion(latestVersion, false);
                updateDialog->show();
            }

          //qDebug() << "SoftwareUpdate::slotDownloadFinished:  checkupdates false"  << endl;

        }
    }

    reply->deleteLater();
   //qDebug() << "SoftwareUpdate::slotDownloadFinished end"  << endl;
}

bool SoftwareUpdate::checkUpdates(QIODevice *data)
{    
    // Checks if there is a new version in the repository

  //qDebug() << "SoftwareUpdate::checkUpdates: " << QString::number(data->size()) << endl;
    QString line, release;
    QStringList stringList, klogStringList;

    QRegularExpression rx("href=\"klog-(\\d\\.)+tar.gz");

    //qDebug() << "SoftwareUpdate::checkUpdates: Before entering the while"<< endl;

    while (!data->atEnd())
    {
        //qDebug() << "SoftwareUpdate::checkUpdates: In the while"<< endl;
        stringList.clear();
        klogStringList.clear();
        line.clear();
        line = data->readLine();
        //qDebug() << "SoftwareUpdate::checkUpdates: line: " << line << endl;
        if (line.contains("klog-"))
        {
            repositoryFound = true;
            stringList << line.split(">", QString::SkipEmptyParts);
            klogStringList << stringList.filter("klog");
            foreach (const QString &str, klogStringList)
            {
                //qDebug() << "SoftwareUpdate::checkUpdates klog: " << str << endl;
                   if (rx.match(str).hasMatch())
                   {
                     //qDebug() << "SoftwareUpdate::checkUpdates: MATCH: " << str << endl;
                       release = str.section("-",1);                       
                       release = release.section("\.tar.gz", 0, 0);                       
                       updateNeeded(release);
                   }
                   else
                   {
                      //qDebug() << "SoftwareUpdate::checkUpdates: DOES NOT MATCH: " << str << endl;
                   }
            }
          //qDebug() << "SoftwareUpdate::checkUpdates: " << line << endl;
        }
    }
  //qDebug() << "SoftwareUpdate::checkUpdates:Latest/Actual: " << latestVersion << "/" << klogVersion << endl;
    if (latestVersion > klogVersion)
    {
        emit updateNeededSignal (true);
       //qDebug() << "SoftwareUpdate::checkUpdates: signal  true" << endl;
        return true;
    }
    else
    {
        //emit updateNeededSignal (false);
       //qDebug() << "SoftwareUpdate::checkUpdates: signal  false 1" << endl;
        return false;
    }

    //emit updateNeededSignal (false);
   //qDebug() << "SoftwareUpdate::checkUpdates: signal  false 2" << endl;
    return false;
}

void SoftwareUpdate::updateNeeded(QString _newVer)
{
  //qDebug() << "SoftwareUpdate::updateNeeded: " << _newVer  << endl;

    if (latestVersion<_newVer)
    {
        latestVersion = _newVer;
    }
/*
    if (klogVersion < _newVer)
    {
      //qDebug() << "SoftwareUpdate::updateNeeded TRUE: " << _newVer  << endl;
        if

    }
    else
    {
      //qDebug() << "SoftwareUpdate::updateNeeded - FALSE " << _newVer  << endl;
    }
*/

   //qDebug() << "SoftwareUpdate::updateNeeded - KLogVersion/latestVersion/newver: "<< klogVersion <<"/"<< latestVersion << "/"<<_newVer  << endl;
}

void SoftwareUpdate::needToUpdate(bool _showWithoutVersion)
{ // This is used to connect to the main server URL.
  // If _showWithoutVersio is false: We are checking for new versions at KLog start: No message should be shown if no new version is found.
  // If _showWithoutVersio is true: The user is manually asking to check. A message should is shown if no new version is found.

   //qDebug() << "SoftwareUpdate::needToUpdate (current version: " << klogVersion  << ")"  << endl;
    messageShown = _showWithoutVersion;
    setVersion(klogVersion);
    setTheURL("http://download.klog.xyz");
    connectToURL();

    //QNetworkReply *reply = manager->get(request);
    //connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    //connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));

}


void SoftwareUpdate::connectToURL()
{
    // This is where the connection takes place.... so first connection may be the main URL but it launches connection after redirections
    QNetworkReply *reply = manager->get(request);

    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slotDownloadFinished(QNetworkReply*)));
}

void SoftwareUpdate::setHeader()
{
    QString ver = util->getAgent(klogVersion);
    if (callsign.length()>2)
    {
        ver = ver + "-" + callsign;
    }
    QByteArray str;
    str.clear();
    str.append(ver);

    request.setRawHeader("User-Agent", str);
}

void SoftwareUpdate::addCall(const QString _call)
{
    if (_call.length()>2)
    {
        callsign = _call;
        setHeader();
    }

}
