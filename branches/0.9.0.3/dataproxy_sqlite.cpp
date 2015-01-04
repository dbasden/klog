#include "dataproxy_sqlite.h"
//#include <QDebug>

DataProxy_SQLite::DataProxy_SQLite()
{
    //qDebug() << "DataProxy_SQLite::DataProxy_SQLite " << endl;

    db = new DataBase(0);
    dbCreated = db->createBandModeMaps();

}
DataProxy_SQLite::~DataProxy_SQLite(){
    //qDebug() << "DataProxy_SQLite::~DataProxy_SQLite" << endl;
}


void DataProxy_SQLite::createLogModel(){
    //qDebug() << "DataProxy_SQLite::createLogModel" << endl;
}

void DataProxy_SQLite::createLogPanel(){
    //qDebug() << "DataProxy_SQLite::createLogPanel" << endl;
}

int DataProxy_SQLite::getIdFromModeName(const QString& _modeName)
{
    //qDebug() << "DataProxy_SQLite::getIdFromModeName: " << _modeName << "/" << QString::number(db->getModeIDFromName2(_modeName)) << endl;
    if (_modeName.length()<2)
    {
        return -3;
    }
    return db->getModeIDFromName2(_modeName);

}

int DataProxy_SQLite::getIdFromBandName(const QString& _bandName)
{
    //qDebug() << "DataProxy_SQLite::getIdFromBandName: " << _bandName  << "/" << QString::number(db->getBandIDFromName2(_bandName))<< endl;
    if (_bandName.length()<1)
    {
        return -3;
    }
    return db->getBandIDFromName2(_bandName);


}

QString DataProxy_SQLite::getNameFromBandId (const int _id)
{
    //qDebug() << "DataProxy_SQLite::getNameFromBandId " << endl;
    return db->getBandNameFromID2(_id);
}

QString DataProxy_SQLite::getNameFromModeId (const int _id)
{
    //qDebug() << "DataProxy_SQLite::getNameFromModeId" << endl;
    return db->getModeNameFromID2(_id);
}


int DataProxy_SQLite::getLastQSOid()
{
    //qDebug() << "DataProxy_SQLite::getLastQSOid" << endl;
    QSqlQuery query;
    if (query.exec("SELECT MAX(id) from log"))
    {
        query.next();
        if (query.isValid())
        {
            return (query.value(0)).toInt();
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

}

bool DataProxy_SQLite::clearLog()
{
    //qDebug() << "DataProxy_SQLite::clearLog" << endl;
    int errorCode = 0;
    QSqlQuery query;
    if (query.exec("DELETE FROM log"))
    {
        //qDebug() << "DataProxy_SQLite::clearLog: Log deleted!" << endl;
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::clearLog: Log deleted FAILED" << endl;
        errorCode = query.lastError().number();
        //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(errorCode) << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
    }

    if (query.exec("DELETE FROM awarddxcc"))
    {
        //qDebug() << "DataProxy_SQLite::clearLog: Awarddxcc deleted!" << endl;
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::clearLog: Awarddxcc deletedFAILED" << endl;
        errorCode = query.lastError().number();
        //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(errorCode) << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
    }

    if (query.exec("DELETE FROM awardwaz"))
    {
        //qDebug() << "DataProxy_SQLite::clearLog: Awardwaz deleted!" << endl;
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::clearLog: Awardwaz deleted FAILED" << endl;
        errorCode = query.lastError().number();
        //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(errorCode) << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
        //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
    }



    query.finish();
    //query.clear();

    if (query.isActive())
    {
        //qDebug() << "DataProxy_SQLite::clearLog: Query Active!" << endl;
        return false;
    }
    else
    {
        query.prepare("VACUUM;");

        //qDebug() << "DataProxy_SQLite::clearLog: Query Not Active!" << endl;
        if (query.exec())
        {
            //qDebug() << "DataProxy_SQLite::clearLog: VACUUM OK!" << endl;
            return true;
        }
        else
        {
            //qDebug() << "DataProxy_SQLite::clearLog: VACUUM FAILED" << endl;
            errorCode = query.lastError().number();
            //qDebug() << "DataProxy_SQLite::clearLog - query error: " << QString::number(errorCode) << endl;
            //qDebug() << "DataProxy_SQLite::clearLog: LastQuery: " << query.lastQuery()  << endl;
            //qDebug() << "DataProxy_SQLite::clearLog: LastError-data: " << query.lastError().databaseText()  << endl;
            //qDebug() << "DataProxy_SQLite::clearLog: LastError-driver: " << query.lastError().driverText()  << endl;
            //qDebug() << "DataProxy_SQLite::clearLog: LastError-n: " << QString::number(query.lastError().number() ) << endl;
        }
    }
    return false;
}

bool DataProxy_SQLite::qslSentViaDirect(const int _qsoId, const QString _updateDate)
{
    //qDebug() << "DataProxy_SQLite::qslSentViaDirect" << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_sent = 'Y', qsl_sent_via = 'D', qslsdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    //QDebug() << "DataProxy_SQLite::qslSentViaDirect: " << stringQuery << endl;
    if (query.exec(stringQuery))
    {
        return true;
    }

    return false;
}

bool DataProxy_SQLite::qslSentViaBureau(const int _qsoId, const QString _updateDate)
{
    //qDebug() << "DataProxy_SQLite::qslSentViaBureau" << endl;

    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_sent = 'Y', qsl_sent_via = 'B', qslsdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);

    if (query.exec(stringQuery))
    {
        return true;
    }

    return false;
}

bool DataProxy_SQLite::qslRecViaBureau(const int _qsoId, const QString _updateDate)
{
    //qDebug() << "DataProxy_SQLite::" << QString::number (_qsoId) << "/" << _updateDate << endl;
    QSqlQuery query;
    QString stringQuery;


    stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    if (query.exec(stringQuery))
    {
        //qDebug() << "DataProxy_SQLite:: TRUE" << endl;
        return true;
    }

    //qDebug() << "DataProxy_SQLite:: FALSE" << endl;
    return false;
}

bool DataProxy_SQLite::qslRecViaBureau(const int _qsoId, const QString _updateDate, const bool _queueSentQSL)
{
    //qDebug() << "DataProxy_SQLite::qslRecViaBureau: " << _updateDate << endl;
    QSqlQuery query;
    QString stringQuery;
    bool requestQSL = false;


    if  (_queueSentQSL)
    {
        stringQuery = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);
        if(query.exec(stringQuery))
        {
            query.next();
            if (query.isValid())
            {
                stringQuery = (query.value(0)).toString();
                if ((stringQuery == "Y") || (stringQuery == "R"))
                {
                    // NO ACTION REQUIRED, QSL IS ALREADY SENT
                     //qDebug() << "DataProxy_SQLite::qslRecViaBureau: QSL already requested" << endl;
                     requestQSL = false;
                     stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                }
                else
                {
                    //qDebug() << "DataProxy_SQLite::qslRecViaBureau: Request QSL-1" << endl;
                    requestQSL = true;
                    stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                }
            }
            else
            {
                //qDebug() << "DataProxy_SQLite::qslRecViaBureau: Request QSL-2" << endl;
                stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                requestQSL = true;
            }
        }
        else
        {
            //qDebug() << "DataProxy_SQLite::qslRecViaBureau: Request QSL-3" << endl;
            stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
            requestQSL = true;
        }

    }
    else
    {
        requestQSL = false;
        stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    }


    //stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'B', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    if (query.exec(stringQuery))
    {
        //qDebug() << "DataProxy_SQLite::qslRecViaBureau TRUE" << endl;
        return true;
    }

    //qDebug() << "DataProxy_SQLite::qslRecViaBureau FALSE" << endl;
    return false;
}

bool DataProxy_SQLite::qslRecViaDirect(const int _qsoId, const QString _updateDate)
{
    //qDebug() << "DataProxy_SQLite::qslRecViaDirect" << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);

    if (query.exec(stringQuery))
    {
        return true;
    }

    return false;
}

bool DataProxy_SQLite::qslRecViaDirect(const int _qsoId, const QString _updateDate, const bool _queueSentQSL)
{
    //qDebug() << "DataProxy_SQLite::qslRecViaDirect: " << _updateDate << endl;
    QSqlQuery query;
    QString stringQuery;

    if  (_queueSentQSL)
    {
        stringQuery = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);
        if(query.exec(stringQuery))
        {
            query.next();
            if (query.isValid())
            {
                stringQuery = (query.value(0)).toString();
                if ((stringQuery == "Y") || (stringQuery == "R"))
                {
                    // NO ACTION REQUIRED, QSL IS ALREADY SENT
                     //qDebug() << "DataProxy_SQLite::qslRecViaDirect: QSL already requested" << endl;
                     stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                }
                else
                {
                    //qDebug() << "DataProxy_SQLite::qslRecViaDirect: Request QSL-1" << endl;
                    stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
                }
            }
            else
            {
                //qDebug() << "DataProxy_SQLite::qslRecViaDirect: Request QSL-2" << endl;
                stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
            }
        }
        else
        {
            //qDebug() << "DataProxy_SQLite::qslRecViaDirect: Request QSL-3" << endl;
            stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qsl_sent='R', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
        }

    }
    else
    {
        stringQuery = QString("UPDATE log SET qsl_rcvd = 'Y', qsl_rcvd_via = 'D', qslrdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    }



    if (query.exec(stringQuery))
    {
        //qDebug() << "DataProxy_SQLite::qslRecViaDirect TRUE" << endl;
        return true;
    }

    //qDebug() << "DataProxy_SQLite::qslRecViaDirect FALSE" << endl;
    return false;
}


bool DataProxy_SQLite::qslSentAsRequested(const int _qsoId, const QString _updateDate)
{
    //TODO: Add some protection to the data before modifying
    //qDebug() << "DataProxy_SQLite::qslSentAsRequested" << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_sent = 'R', qslsdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    //qDebug() << "DataProxy_SQLite::qslSentAsRequested: " << stringQuery << endl;

    if (query.exec(stringQuery))
    {
        //qDebug() << "DataProxy_SQLite::qslSentAsRequested" << endl;
        return true;
    }

    return false;
}

bool DataProxy_SQLite::qslRecAsRequested(const int _qsoId, const QString _updateDate)
{
//TODO: Add some protection to the data before modifying
    //qDebug() << "DataProxy_SQLite::qslRecAsRequested" << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("UPDATE log SET qsl_rcvd = 'R', qslsdate = '%1' WHERE id = '%2'").arg(_updateDate).arg(_qsoId);
    //qDebug() << "DataProxy_SQLite::qslRecAsRequested: " << stringQuery << endl;

    if (query.exec(stringQuery))
    {
        //qDebug() << "DataProxy_SQLite::qslRecAsRequested" << endl;
        return true;
    }

    return false;
}

bool DataProxy_SQLite::isQSLReceived(const int _qsoId)
{

    //qDebug() << "DataProxy_SQLite::isQSLReceived" << QString::number(_qsoId) << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("SELECT qsl_rcvd FROM log WHERE id = '%1'").arg(_qsoId);
    query.exec(stringQuery);
    query.next();
    if (query.isValid())
    {
        stringQuery = (query.value(0)).toString();
        if (stringQuery == "Y")
        {
            //qDebug() << "DataProxy_SQLitew::isQSLReceived: " << QString::number(_qsoId) << "QSL Received" << endl;
            return true;
        }
        else
        {
            //qDebug() << "DataProxy_SQLite::isQSLReceived: " << QString::number(_qsoId) << "QSL NOT Received-1" << endl;
            return false;
        }
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::isQSLReceived: " << QString::number(_qsoId) << "QSL NOT Received-2" << endl;
        return false;
    }
}

bool DataProxy_SQLite::isQSLSent(const int _qsoId)
{

    //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << endl;
    QSqlQuery query;
    QString stringQuery;
    stringQuery = QString("SELECT qsl_sent FROM log WHERE id = '%1'").arg(_qsoId);
    query.exec(stringQuery);
    query.next();
    if (query.isValid())
    {
        stringQuery = (query.value(0)).toString();
        if (stringQuery == "Y")
        {
            //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << "QSL Sent" << endl;
            return true;
        }
        else
        {
            //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << "QSL NOT Sent-1" << endl;
            return false;
        }
    }
    else
    {
        //qDebug() << "DataProxy_SQLite::isQSLSent: " << QString::number(_qsoId) << "QSL NOT Sent-2" << endl;
        return false;
    }
}

QString DataProxy_SQLite::getCallFromId(const int _qsoId)
{
    //qDebug() << "DataProxy_SQLite::getCallFromId" << endl;
    QSqlQuery query;
    QString stringQuery = QString("SELECT call FROM log WHERE id='%1'").arg(_qsoId);
    query.exec(stringQuery);
    query.next();
    if (query.isValid())
    {
        return (query.value(0)).toString();
    }
    else
    {
        return "";
    }
}

bool DataProxy_SQLite::deleteQSO(const int _qsoId)
{
    //qDebug() << "DataProxy_SQLite::deleteQSO" << endl;
    QSqlQuery query;
    QString stringQuery = QString("DELETE FROM log WHERE id='%1'").arg(_qsoId);
    return query.exec(stringQuery);
}

int DataProxy_SQLite::isWorkedB4(const QString _qrz, const int _currentLog)
{
    //qDebug() << "DataProxy_SQLite::isWorkedB4" << endl;
    QSqlQuery query;
    QString queryString;
    queryString = QString("SELECT id FROM log WHERE call='%1' AND lognumber='%2'").arg(_qrz).arg(_currentLog);
    //queryString = "SELECT id FROM log WHERE call='" + _qrz +"'";
    //qDebug() << "World::checkIfWorkedB4: " << queryString << endl;
    query.exec(queryString);
    query.next();
    if (query.isValid())
    {
        return (query.value(0)).toInt();
    }
    else
    {
        return -1;
    }
}

bool DataProxy_SQLite::isDXCCConfirmed(const int _dxcc, const int _currentLog)
{
    qDebug() << "DataProxy_SQLite::isDXCCConfirmed: " << QString::number(_dxcc) << "/" << QString::number(_currentLog) << endl;
    QString queryString = QString("SELECT confirmed from awarddxcc WHERE dxcc='%1' AND lognumber='%2'").arg(_dxcc).arg(_currentLog);
    QSqlQuery query;

    if (query.exec (queryString))
    {
        query.next();
        if (query.isValid())
        {
            if ( (query.value(0)).toInt() == 1)
            {
                qDebug() << "DataProxy_SQLite::isDXCCConfrmed: TRUE" << endl;
                return true;
            }
            else
            {
                qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE1" << endl;
                return false;
            }
        }
        else
        {
            qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE2" << endl;
            return false;
        }
    }
    else
    {
        qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE3" << endl;
        return false;
    }
    qDebug() << "DataProxy_SQLite::isDXCCConfrmed: FALSE4" << endl;
    return false;
}

bool DataProxy_SQLite::isHF(const int _band)
{
    if ((_band>=18) && (_band<=27))
    {
        qDebug() << "DataProxy_SQLite::isHF: TRUE" << endl;
        return true;
    }
    else
    {
        qDebug() << "DataProxy_SQLite::isHF: FALSE" << endl;
        return false;
    }
}

bool DataProxy_SQLite::isWARC(const int _band)
{
    if ((_band == 19) || (_band==21) ||(_band==23) )
    {
        qDebug() << "DataProxy_SQLite::isWARC: tRUE" << endl;
        return true;
    }
    else
    {
        qDebug() << "DataProxy_SQLite::isWARC: FALSE" << endl;
        return false;
    }
}

bool DataProxy_SQLite::isVHF(const int _band)
{
    if (_band<=17)
    {
        qDebug() << "DataProxy_SQLite::isVHF: TRUE" << endl;
        return true;
    }
    else
    {
        qDebug() << "DataProxy_SQLite::isVHF: FALSE" << endl;
        return false;
    }
}

