#ifndef DATAPROXY_H
#define DATAPROXY_H
/***************************************************************************
                          dataproxy.h  -  description
                             -------------------
    begin                : feb 2015
    copyright            : (C) 2015 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
#include <QString>
#include <QStringList>
#include <QObject>

class DataProxy : public QObject
{    
    Q_OBJECT
public:
    DataProxy();
   // DataProxy(const QStringList _qs);
    ~DataProxy();

    virtual QString getSoftVersion();
    virtual QString getDBVersion();
    virtual bool reconnectDB();

    virtual void createLogModel();
    virtual void createLogPanel();
    virtual bool haveAtLeastOneLog();

    virtual QStringList getColumnNamesFromTableLog();


    //UI functions
    virtual int getIdFromModeName(const QString& _modeName);
    virtual int getIdFromBandName(const QString& _bandName);
    virtual int getSubModeIdFromSubMode(const QString _subModeName);
    virtual int getModeIdFromSubModeId(const int _sm);

    virtual QString getNameFromBandId (const int _id);
    virtual QString getNameFromModeId (const int _id);
    virtual QString getNameFromSubModeId (const int _id);
    virtual QString getSubModeFromId (const int _id);
    virtual QString getNameFromSubMode (const QString _sm); // Checks if a submode is deprecated //TODO: Check if really needed
    //virtual QString getModeFromSubMode (const QString _sm);
    virtual bool isModeDeprecated (const QString _sm);

    virtual QString getFreqFromBandId(const int _id);
    virtual int getBandIdFromFreq(const double _n);
    virtual QString getBandNameFromFreq(const double _n);

    virtual double getLowLimitBandFromBandName(const QString _sm);
    virtual double getLowLimitBandFromBandId(const QString _sm);
    virtual bool isThisFreqInBand(const QString b, const QString fr);
    
    virtual bool isHF(const int _band);
    virtual bool isWARC(const int _band);
    virtual bool isVHF(const int _band);
    virtual bool isUHF(const int _band);

    virtual QStringList getBands();
    virtual QStringList getModes();
    virtual QStringList sortBandNamesBottonUp(const QStringList _qs);
    virtual QStringList getBandIDs();
    virtual QStringList getModesIDs();
    virtual QStringList getBandsInLog(const int _log);
    virtual QStringList getModesInLog(const int _log);
    virtual int getMostUsedBand(const int _log);
    virtual int getMostUsedMode(const int _log);
    virtual int getQSOsInBand(const QString _band, const int _log);
    virtual int getQSOsInMode(const QString _mode, const int _log);    

    virtual int getLastQSOid();
    virtual bool clearLog();

    virtual bool addQSOFromWSJTX(const QString _dxcall, const double _freq, const QString _mode,
                                 const QString _dx_grid, const QString _time_off, const QString _report_sent, const QString _report_rec,
                                 const QString _tx_power, const QString _comments, const QString _name, const QString _time_on,
                                 const int _dxcc, const QString _opQrz, const QString _stQrz, const QString _myLoc, const int _logN);

    virtual bool deleteQSO(const int _qsoId);
    virtual int isWorkedB4(const QString _qrz, const int _currentLog);
    virtual bool isThisQSODuplicated(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode);
    virtual int getDuplicatedQSOId(const QString _qrz, const QString _date, const QString _time, const int _band, const int _mode);
    virtual bool isDXCCConfirmed(const int _dxcc, const int _currentLog);
    virtual bool isQSLReceived(const int _qsoId);
    virtual bool isQSLSent(const int _qsoId);


    virtual bool qslSentViaDirect(const int _qsoId, const QString _updateDate);
    virtual bool qslSentViaBureau(const int _qsoId, const QString _updateDate);
    virtual bool qslSentAsRequested(const int _qsoId, const QString _updateDate);
    virtual bool qslRecAsRequested(const int _qsoId, const QString _updateDate);
    virtual bool qslRecViaBureau(const int _qsoId, const QString _updateDate);
    virtual bool qslRecViaBureau(const int _qsoId, const QString _updateDate, const bool _queueSentQSL);
    virtual bool qslRecViaDirect(const int _qsoId, const QString _updateDate);
    virtual bool qslRecViaDirect(const int _qsoId, const QString _updateDate, const bool _queueSentQSL);
    virtual bool setClubLogSent(const int _qsoId, const QString _st, const QString _updateDate);

    virtual int getBandFromId(const int _qsoId);
    virtual int getModeFromId(const int _qsoId);
    virtual int getDXCCFromId(const int _qsoId);
    virtual int getCQZFromId(const int _qsoId);
    virtual QString getCallFromId(const int _qsoId);
    virtual QStringList getClubLogRealTimeFromId(const int _qsoId);
    // Complete with previous
    virtual QString getNameFromQRZ(const QString _call);
    virtual QString getQTHFromQRZ(const QString _call);
    virtual QString getLocatorFromQRZ(const QString _call);
    virtual QString getIOTAFromQRZ(const QString _call);
    virtual QString getQSLViaFromQRZ(const QString _call);
    // /Complete with previous

    virtual bool updateAwardDXCC();
    virtual bool updateAwardWAZ();

    virtual int getContinentIdFromContinentShortName(const QString _n);
    virtual QString getContinentShortNameFromEntity(const int _n);
    virtual int getContinentIdFromEntity(const int _n);
    virtual QStringList getContinentShortNames();
    virtual bool isValidContinentShortName(const QString _n);

    virtual int getCQzFromPrefix(const QString _p);
    virtual int getCQzFromEntity(const int _n);
    virtual int getITUzFromEntity(const int _n);
    virtual int getITUzFromPrefix(const QString _p);
    virtual QString getEntityNameFromId(const int _n);
    virtual QString getEntityMainPrefix(const int _entityN);
    virtual bool isNewCQz(int _c);
    virtual bool isNewEntity(int _e);
    virtual double getLongitudeFromEntity(const int _e);
    virtual double getLatitudeFromEntity(const int _e);

    virtual int getDXCCFromPrefix(const QString _p);
    virtual QString getEntityPrefixes(const int _enti);
    virtual QStringList getEntitiesNames();
    virtual QStringList getEntitiesIds();
    virtual int getHowManyEntities();
    virtual int getMaxEntityID(bool limit);                 // limit = true implies that it will only shouw ARRL valid DXCC (i.e. will not count Sicily)

    virtual QStringList getOperatingYears(const int _currentLog);
    virtual void compressDB();
    virtual bool unMarkAllQSO();                                // Unmarks all the marked QSO
    virtual bool lotwSentQueue(const QString _updateDate, const int _currentLog);          // Mark LOTW QSL SENT as Q (Queued)
    virtual bool lotwSentYes(const QString _updateDate, const int _currentLog, const QString _station);         // Updat LOTW QSL SENT marked as Q as Y (Queued)

    virtual int getQSOonYear(const int _year, const int _logNumber);
    virtual int getDXCConYear(const int _year, const int _logNumber);
    virtual int getCQzonYear(const int _year, const int _logNumber);
    virtual int getQSOsWithDXCC(const int _dxcc, const int _logNumber);
    virtual int getQSOsAtHour(const int _hour, const int _log);
    virtual int getQSOsOnMonth(const int _month, const int _log);

    virtual bool newDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber);

    virtual QStringList getContestNames();
    virtual QStringList getContestCat(const int _catn);
    virtual QStringList getContestOverlays();
    virtual int getContestTypeN(const int _co, const int _catop, const int _catas, const int _catpo, const int _catba, const int _catov, const int _catmo);
    virtual QStringList getDataFromContestType(const int _n);
    virtual int getLogTypeNumber(const QString _logType);   // Returns the number of the type of log name
    virtual QString getLogTypeName(const int _logType);     // Returns the name of the type of log number
    virtual QString getLogTypeOfUserLog(const int _logN);       // Returns the type of log of a specific log
    virtual int getLogNumberFromQSOId(const int _qsoId);    // Returns the log where the QSO id was added

    virtual QStringList getBandNames();
    virtual QStringList getPropModeList();

    virtual bool clearSatList();
    virtual bool addSatellite(const QString _arrlId, const QString _name, const QString _downLink, const QString _upLink, const QString _mode, int id = -1);
    virtual int getDBSatId(const QString _arrlId);
    virtual QStringList getSatellitesList();
    virtual QString getSatelliteUplink(const QString _sat);
    virtual QString getSatelliteDownlink(const QString _sat);
    virtual QString getSatelliteMode(const QString _sat);
    virtual QString getSatelliteFullUplink(const QString _sat);
    virtual QString getSatelliteFullDownlink(const QString _sat);
    virtual QString getSatelliteFullMode(const QString _sat);
    virtual QString getSatelliteName(const QString _sat);
    virtual QString getSateliteArrlIdFromId(const int _id);
    virtual QStringList getQSLRcvdList();
    virtual QStringList getQSLSentList();
    virtual QStringList getClubLogStatusList();
    virtual QStringList getQSLViaList();

    virtual QStringList getValidCatOptions(const int _currentCat, const int _lowerCa);

    virtual bool fillEmptyDXCCInTheLog();
    virtual int getHowManyQSOInLog(const int _log);
    virtual int getHowManyConfirmedQSLInLog(const int _log);
    virtual int getHowManyQSLSentInLog(const int _log);
    virtual int getQSOsWithContinent(const QString _cont, const int _logNumber);
    virtual int getNumberOfManagedLogs();
    virtual int getMaxLogNumber();
    virtual QStringList getListOfManagedLogs();
    virtual QString getStationCallSignFromLog(const int _log);
    virtual QStringList getStationCallSignsFromLog(const int _log);
    virtual QString getOperatorsFromLog(const int _log);
    virtual QString getCommentsFromLog(const int _log);
    virtual QString getLogDateFromLog(const int _log);
    virtual QString getLogTypeNFromLog(const int _log);
    virtual bool addNewLog (const QStringList _qs);
    virtual bool doesThisLogExist(const int _log);

    virtual bool updateISONames(); // Update the entities ISO 3166 names for the flags
    virtual QString getISOName(const int _n);

    virtual void getFoundInLog(const QString _txt, const int _log=-1);
    virtual bool setDXCCAwardStatus(const int _qsoId);
    virtual bool setWAZAwardStatus(const int _qsoId);

    //virtual bool queryPrepare(const QString _query);
    //virtual bool queryBind(const QString _field, const QString value);
    //virtual bool queryExec();

/*
    virtual bool isMultiplier(const QStringList _qs);
    virtual int getQSOPoints(const QStringList _qs);
    virtual bool saveFileToSend(const QString& _fileName);
    virtual int getTotalScore();
    virtual int getMultipliers();
    virtual int getPoints();
*/
private:
    virtual QStringList sortBandIdBottonUp(const QStringList _qs);
    //virtual QStringList getColumnNamesFromTable(const QString _tableName);

   // int points;
   // int multipliers;

signals:
    void qsoFound(const QStringList _qs); // Each: QString with format: Fieldname:value

};
#endif // DATAPROXY_H


