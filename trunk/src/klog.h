/****************************f***********************************************
                          klog.h  -  description
                             -------------------
    begin                : s�b dic  7 18:42:45 CET 2002
    copyright            : (C) 2002 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
�* � it under the terms of the GNU General Public License as published by     *
�* � the Free Software Foundation; either version 2 of the License, or        *
�* � (at your option) any later version.                                      *
 *                                                                            *
�* � This program is distributed in the hope that it will be useful,          *
�* � but WITHOUT ANY WARRANTY; without even the implied warranty of           *
�* � MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. �See the            *
�* � GNU General Public License for more details.                             *
 *                                                                            *
�* � You should have received a copy of the GNU General Public License        *
�* � along with this program; if not, write to the Free Software              *
�* � Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA �02111-1307 �USA*
 *****************************************************************************/

#ifndef KLOG_H
#define KLOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <kapp.h>
#include <qaction.h>
#include <qapplication.h>
#include <qwidget.h>
//#include <qthread.h>
#include <klineedit.h>
#include <kcombobox.h>
#include "klogui.h"
#include <qlistview.h>
#include <klistview.h>
#include <qfile.h>
#include <qdatetime.h>
#include <qdatetimeedit.h>
#include <qspinbox.h>
#include <ktextedit.h>
#include <qstring.h>
#include <qlabel.h>
#include <qpoint.h>
#include <qlcdnumber.h>
#include <kglobal.h>
//#include <klocale.h>
#include <kpushbutton.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qcolor.h>
//#include <qcolorgroup.h> // for cluster
#include <qdialog.h>
#include <qinputdialog.h>
#include <knuminput.h>
#include <kled.h>
#include <kaboutapplication.h>
#include <kaboutdata.h>
#include <kpopupmenu.h>
#include <qtimer.h>
// The next two are for the cluster box
#include <qsocket.h>
#include <qurl.h>
#include <qtextstream.h>
// The next two are for printing
#include <kprinter.h>
#include <qpainter.h>
#include <qprogressdialog.h>
#include <qsplashscreen.h>
#include <qpaintdevicemetrics.h>
#include <kbugreport.h>
#include <qtabwidget.h>
#include <qstringlist.h>
//#include <iostream.h>
#include "awarddxcc.h"
#include "awardwaz.h"  
#include "qso.h"
#include "calls.h"
#include "world.h"
//#include "entity.h"
#include "locator.h"
#include "setup.h"
//#include "qslneeded.h"
#include "adif.h"
#include "workedcalls.h"
#include "kloghamlib.h"
#include "awards.h"
#include "award.h"
#include "reference.h"
//#include "klogtip.h"

//#include <Qt3Support>
//#include "ostream.h"

// BANDS 9 {10/12/15/17/20/30/40/80/160}
//MODES 11 {SSB/CW/FM/PSK31/SSTV/RTTY/TOR/PKT/
//          AM/ATV/PAC/CLO
//
// CONTINENTS AF, AN, AS, EU, NA, OC, SA

//const char SEPARATOR = ',';

/** Klog is the base class of the project */
class Klog : public klogui{
//class Klog : public klogui, QThread {
//f  friend class QslNeeded;
	friend class dxClusterSpotItem;
	friend class bandMapSpotItem;

	Q_OBJECT
	public:
	/** construtor */
	Klog(QWidget* parent=0, const char *name=0);
	/** destructor */
	~Klog();
	//Qso qso;

	World world;
//    Entity entity;

	typedef QValueList<Qso> LogBook;

	LogBook logbook;
	LogBook oLogbook; //Ordered logbook (by date) (exit from orderLog)
	LogBook templogbook; //The "not yet saved" logbook.

	//public:
	bool haveWorld();

//	virtual void run();

	public slots:
	void slotUpdateTime();
	void slotQSLRec();
	void slotQSLSent();
	void slotQSLRecSent();
	void slotQsoDelete();
	//  slotDeleteQso();
	void slotCancelSearchButton();
	void slotSearchButton();
	void slotQrzChanged();
	void slotOkBtn();
	void slotClearBtn();
	void clearEntityBox();
	void slotQslSentBoxChanged();
	void slotQslRecvBoxChanged();
	void slotQSLcomboBoxChanged();
	void slotSearchQSO2QSL(); // Shows in the search box QSO from which we need the QSL and we have not sent it.
	void slotPreferences();
	void fileNew();
	void fileSave();
	void fileSaveAs();
	void filePrint();
	//void printLabels(); // Generate the file for GLabels or KBarcode
	void slothamlibUpdateFrequency();

//	void adifFileSave();
	void adifTempFileSave(const QString& fn, LogBook lb, bool manualSave);  // To save the templog automatically after each QSO is added
	//  void adifSaveAs(const QString& tfileName);
	void adifReadLog(const QString& tfileName);
	int getProgresStepForDialog(int totalSteps);
	//  void newAdifReadLog (const QString& tfileName);
	void tlfReadLog(const QString& tfileName);
	void fileOpen();
	void fileExit();
	void helpAbout();
	void readQso(); //Just read the values an fill the qso
	void modifyQso(); // Modify an existing QSO with the data on the boxes
	void slotQsoSelectedForEdit(QListViewItem* item);//Puts the QSO in the edit box
	void slotQsoSearchSelectedForEdit( QListViewItem * item); //Puts the QSO in the edit box
	void slotQsoSelected (QListViewItem* item); // Just to see qso data
	void slotQsoRightButtonFromLog(QListViewItem * item, const QPoint &p);
	void slotQsoRightButtonFromSearch(QListViewItem * item, const QPoint &p);
	void slotModifyBtn();
	void showLogList();   // Shows the log in the botton widget
	void showMenuRightButton(int qqso, const QPoint &p); //This is called from  slotQsoRightButton and show the actions menu

// Printing modules
//  void slotPrint();
//  void doPrint(KPrinter *printer);
//  void doPaint(QPainter *p, int autofit, int w, int h);



  Qso getByCall(const QString& tqrz);
  Qso getByNumber(const int n);
  void showQso(); // Gets data from the Input part and show in the botton widget
  void toEditQso(); //Puts the QSO data in the edit box
  void accept();

  private slots:
	void slotLocatorChanged();
	void slotMyLocatorChanged();
	void slotClusterCloseConnection();
	void slotClusterSendToServer();
    void slotClusterSocketReadyRead();
    void slotClusterSocketConnected();
    void slotClusterSocketConnectionClosed();
    void slotClusterSocketClosed();
    void slotClusterSocketError( int e );
    void slotClusterConnect();
    void slotClusterClearInputLine();
    void slotClusterSpotToLog(QListViewItem * item);
    void slotClusterSpotCheck(QListViewItem * item);
//    void slotClusterSetup();
    void slotKlogSetup();
    void slotBugReport();
    void slotModeChanged (int i);
    void slotBandChanged ();

  private:

	Qso qso;
	Qso previousQso;       // This and workedCall is to check if a call has been previously
	WorkedCalls workedCall;// worked and it is posible to reuse the data.
	bool completeWithPrevious;	// Configuration parameter
	bool requireMandatory;		// Configuration parameter (requires all the mandatory data for all the QSO) (QRZ, date, time, band, mode, RST (TX&RX))
	QString tempLogFile;	// Temp log to save automatically

	typedef QValueList<Reference> References; // The list of references of the current award.
	References references;		// The references of the current award.


	// Awards supported by KLog
	Adif adif;
	AwardDXCC dxcc;
	AwardWAZ waz;
	Awards awards;
	Award award;
	QStringList awardReferences;	// All the references of the current award to prepare the combobox
	QString DXClusterServerToUse;
	void processLogLine (const QString& tLogLine); // Process a line of data/qso from adifReadLog
	void clearGUI();
	bool didISave();
	bool haveAllTheFields();
	void entityState(const int tentity);
	void addQSOToLog();
	void readConf();
	void slotImportTlf();
	void slotAddLog();
	void slotIOTAChanged();
	void prepareIOTAComboBox (const int tenti);
	void prepareAwardBox (const int tenti);
	void readAwardsStatus();	//Reads the full DXCC&WAZ status
	bool checkIfValidDXCluster(const QString &tdxcluster); // Is a valid URL? 
//    void slotQslNeededCheck();
	void setMyQrz(const QString &tqrz);
	QString getMyQrz() const;
	void setMyLocator(const QString &tlocator);
	QString getMyLocator() const;
	void showWhere(const int enti);
	QString getThisQSODXLocator ();
	int needToWorkFromCluster(const QString &tqrz, const int tband); // 0 confirmed, 1 worked, 2 new one, -1 error
//    QColor stateColor(const QString &tqrz, const int tband);    
    void showAwardsNumbers(); // Shows how many worked, confirmed, ...
    void clearAwards();
    void showDistancesAndBeam(const int dist, const int beam); // Shows distance and beam

// Worked before
	void completeAllQSOsFromLog();
	//void KloggetAllTheCallsFromLog(); // To be run once after the log is read from the disk.
	void fillEntityBandState(const int enti);
	void sortLog(); //To order by date the log and produce OLog
	void showIfPreviouslyWorked(); // Uses previousQso and workedCall
	void addToPreviouslyWorked(); // Uses previousQso and workedCall    
	void completeThePreviouslyWorked();
	void getAllTheCallsFromLog();// To be run once after the log is read from the disk.
	
	QString getNumberString(const int intNumber); //Receivs a number a returns a string of a fixed length
	int getEntityFromCall(); // Just read the call from the QRZ box and returns the entity number

	void prepareAwardComboBox(const int tenti);
	QString checkExtension(QString extension); // To check the extension of the file // TNX Kike

	void listHamlib();
	void addDXSpotToBandMap(QString& freq, QString& dx, QString& from, QColor& dxcolor); // Adds a DXSpot to the bandmap from dxcluster
	void showTip();
	QString returnLines(const QString& tword); //Puts \n where \\n

//    int band2Num(const QString &tband);

  //  Qso qso;
	bool showProgressDialog;
	bool qslViac;
	QColor ledColor;
	Locator locator;
	QString myLocatorTemp;  // To manage temp locators and distances
	QString dxLocator;
	QString myQrz;
	QString myLocator;
	QString klogDir;
	QString logFileNameToOpen;
	QString logFileNameToSave;
	QString aux;
    QString operatorStringAux;
    QString textStringAux;
    QString KLogVersion;
	QString dateString;
	QString timeString;
	QString datesString;
	QString daterString;
	QString qsoLine;
	QString adifTab;
	QString theData;
 

    KPrinter printer;
//    QPainter p;

QColorGroup colorGroup; // For cluster
    QPainter clusterPainter; // For cluster
QColor confirmedColor;
QColor workedColor;
QColor neededColor;
QColor defaultColor;
QColor dxSpotColor;
QColor blackColor;
QColor colorInUse;  // Till now only in use in Klog::slotSearchButton()
int number;  // QSO number
int confirmed; //confirmed QSOs
int rstrx;  // RST received from the other peer
int rsttx;  // RST you pass to the other peer
int tTxValue;
int tRxValue;
int sTxValue;
int sRxValue;


	int band;   // Band used in the QSO
	int imode;
	QString power;  // Power used for the QSO (in your side)
	QStringList fields; // Auxiliary variable to splir log/formated strings
	int i, j;
	int kk; // This is just for intra-fuction use 
	int itemPos; // for modifying
	int enti;   //Just for entity name show
	int entiBak;
	int callLen;  // The call's lenght
	int callLenPrev;  // The previous call's lenght to detect if deleting
	int callLenFound;
	int beam; // the beam in degrees DELETE
	int distance; // Distance between the two stations in the QSO
	bool lastDelete;  // �Last key pressed is delete?
	bool callFound;   // Boolean to show if the call has been found
	bool modify;
	bool searching2QSL; 
	bool prefixFound;
	bool dirExist;
	bool needToSave;
	bool actionSent;
	bool actionRec;
	bool timeInUTC;   // Time in UTC or LocalTime
	bool realTimeLog; // The clock will be in real time or stopped to add previous QSOs
//bool justOneTime; //  For slotQslRec to avoid twice confirmed--
	bool wasConfirmed; // For the same purporse as above.
	bool openLastByDefault;
QString mode;
QString qrz;  // Just used in the searching functions
QString iota;
QString localAward;	// A String as the iota to temporaly save the localAward (if any)
QDateTime dateTime;
QDateTime dateTimeContinuous;
QDate qslSen;
QDate qslRec;
//QDate date;
//QTime time;
QString comment;
QString dateTimeString;
//Preferences setupDialog;

QPixmap *editdeletePixMap;
QPixmap *editdeleteOffPixMap;
QPixmap *qslRecPixMap;
QPixmap *qslRecOffPixMap;
QPixmap *qslSenPixMap;
QPixmap *qslSenOffPixMap;
// Cluster variables
int neededWorkedConfirmed; // 0 needed, 1 worked, 2 confirmed, 3 default
QSocket *socket;
QLineEdit *inputText;
QString dxClusterHost;
QString dxClusterString;
QString dxClusterDxCall;
QString dxClusterShow;
//QString dxClustertmp;
int dxClusterPort;
int dxClusterCallLen;
int dxClusterEnti;
int dxClusterBand; // tmp variable
	bool dxClusterConnected;
	bool dxClusterHFSpots;
	bool dxClusterVHFSpots;
	bool dxClusterWARCSpots;
	bool dxClusterWXANNounces;
	bool dxClusterWCYANNounces;
	bool dxClusterANNounces;
	bool dxClusterConfirmedSpots;
//void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment );
//  virtual void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int align );

KBugReport  *bugReport;
// 0 QRZ
// 1 Date
// 2 Time
// 3 Band
// 4 Mode
// 5 RST tx
// 6 RST rx
bool haveAllMandatoryFields[6];

// hamlib support
kloghamlib KlogHamlib;
bool hamlib;
bool hamlibReading;
bool hamlibPossible;
double hamlibFreq;
int bauds;
int rignameNumber;
//int strength;
int hamlibInterval;
QString serialPort;
QTimer *hamlibtimer;


};

class dxClusterSpotItem : public KListViewItem {
public:
dxClusterSpotItem( KListView *parent, const QString& spot, const QColor& color );
~dxClusterSpotItem();
virtual void paintCell( QPainter *p, const QColorGroup &cg,
int column, int width, int alignment );

//Message *message() { return myMessage; }

protected:
  //  Message *myMessage;
  QColor spotColor;

};


class searchBoxItem : public KListViewItem {
public:
searchBoxItem( KListView *parent, const QString& call, const QString& date, const QString& time, const QString& band, const QString& mode, const QString& RSTsent, const QString& RSTrec, const QString& numb, const QColor& color );
~searchBoxItem();
virtual void paintCell( QPainter *p, const QColorGroup &cg,
        int column, int width, int alignment );


protected:
  QColor qsoColor;
  QString call;
  QString date;
  QString band;
  QString mode;
  QString RSTsent;
  QString RSTrec;
};

// class bandMapSpotItem : public KListViewItem {
// // Items for the BandMap
// public:
// bandMapSpotItem( KListView *parent, const QString& freq, const QString& dx, const QString& from, const QColor& dxcolor );
// ~bandMapSpotItem();
// virtual void paintCell( QPainter *p, const QColorGroup &cg,
//                             int column, int width, int alignment );
// 
// //Message *message() { return myMessage; }
// 
// protected:
//   //  Message *myMessage;
// 	QColor spotColor;
// 	
// 
// };

#endif
