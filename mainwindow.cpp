/***************************************************************************
                          mainwindow.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
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


#include <QtWidgets>
#include <QtSql>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "mainwindow.h"

//#include <qDebug>


MainWindow::MainWindow(const QString _klogDir, const QString tversion)
{

 //qDebug() << "MainWindow::MainWindow: "<<  _klogDir << " Ver: " << tversion << endl;
   QTime start;
   start = QTime::currentTime();
  //qDebug() << "MainWindow::MainWindow: "<<  (QTime::currentTime()).toString("hhmmsszzz")<< endl;

    upAndRunning = false; // To define some actions that can only be run when starting the software
    //connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(slotDownloadFinished(QNetworkReply*))); // To download cty.csv
    //flagIcon = new QPushButton; // To paint a flag of the worked entity

    // <ui>
    doc = new QTextDocument;
    util = new Utilities;

    needToEnd = false;
    cleaning = false;
    qrzAutoChanging = false;
    dxclusterServerToConnect = "dxfun.com";
    dxclusterServerPort = 8000;
    contestMode = DX;

    defaultADIFLogFile = "klog.adi";
    softwareVersion = tversion;

    klogDir = _klogDir;


    InValidCharsInPrevCall = false;
    //stationCallSignShownInSearch = true;
    checkNewVersions = true;
    reportInfo = false;
    configured = false;
    modify = false;
    qslingNeeded = false; // When clicking on Find QSO to QSL
    manageMode = false;
    selectedYear = 0;
    defaultMode = 0;
    defaultBand = 0;
  //qDebug() << "MainWindow::MainWindow: 1 - currentMode: " << QString::number(currentMode) << endl;
    currentMode = 0;
   //qDebug() << "MainWindow::MainWindow: 2 - currentMode: " << QString::number(currentMode) << endl;
    currentModeShown = currentMode;
    currentBand = 0;
    currentBandShown = currentBand;
    currentLog = 1;
    points = 0;
    multipliers = 0;
    qsoPoints = 0;
    qsoMultiplier = 0;
    operatorQRZ = "";
    stationQRZ = "";
    mainQRZ = "";
    myLocator = "";
    dxLocator ="";
    myPower = 0.0;

    //lastPower = myPower;
    //lastOperatorQRZ = operatorQRZ;
    //lastStationQRZ = stationQRZ;
    //lastMyLocator = myLocator;


   //qDebug() << "MainWindow::MainWindow: 0007" << endl;
    //entitiesList.clear();
    //propModeList.clear();
    currentEntity = -1; // To optimize the calls to different world methods if the entity does not change. Used in slotQRZTextChanged
    previousEntity = -1;// To optimize the calls to different world methods if the entity does not change.
    realTime=true;
    UTCTime=true;
    keepMyData=true;
    completeWithPrevious=false;
    completedWithPreviousQTH=false;
    completedWithPreviousLocator=false;
    completedWithPreviousName=false;
    completedWithPreviousIOTA=false;
    completedWithPreviousQSLVia=false;

    alwaysADIF=false;
    useDefaultLogFileName=false;
    needToSave=false;
    qrzSmallModDontCalculate=false;
    imperialSystem=false;
    sendQSLWhenRec = true;

    dxClusterShowHF=true;
    dxClusterShowVHF=true;
    dxClusterShowWARC=true;
    dxClusterShowWorked=true;
    dxClusterShowConfirmed=true;
    dxClusterShowAnn=true;
    dxClusterShowWWV=true;
    dxClusterShowWCY=true;

    keepSatPage = false;
   //qDebug() << "MainWindow::MainWindow: 0008" << endl;
    clublogActive = false;
    clublogRealTime = false;
    clublogUser = QString();
    clublogPass = QString();
    clublogEmail = QString();
    elogClublog = new eLogClubLog();
    clublogAnswer = -1;

    defaultColor.setNamedColor("slategrey");
    neededColor.setNamedColor("yellow");
    workedColor.setNamedColor("blue");
    confirmedColor.setNamedColor("red");
    newOneColor.setNamedColor("green");

    //Default band/modes
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW" << "RTTY";

    dxccStatusWidget = new DXCCStatusWidget();
    logWindow = new LogWindow(this);
    searchWidget = new SearchWidget (this);
    infoWidget = new InfoWidget(this);

   //qDebug() << "MainWindow::MainWindow: 0009" << endl;

    //helpHelpDialog = new HelpHelpDialog(softwareVersion);
   //qDebug() << "MainWindow::MainWindow: 00091" << endl;
    //helpAboutDialog = new HelpAboutDialog(softwareVersion);
    aboutDialog = new AboutDialog(softwareVersion);
   //qDebug() << "MainWindow::MainWindow: 00092" << endl;
/*
    searchResultsTreeWidget = new QTreeWidget;
    searchResultsTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    searchResultsTreeWidget->setSortingEnabled(true);    
    searchResultsTreeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    searchResultsTreeWidget->setMouseTracking(true);

    searchBoxClearButton = new QPushButton(tr("&Clear"), this);
    searchBoxExportButton  = new QPushButton(tr("&Export Highlighted), this);
    searchBoxSelectAllButton  = new QPushButton(tr("&Select All"), this);
    searchBoxReSearchButton = new QPushButton(tr("&Search"), this);
    searchAllRadioButton = new QRadioButton (tr("All"), this);
    searchSelectAllClicked = false;
*/
   //qDebug() << "MainWindow::MainWindow: 0010" << endl;

    recalculateAwardsButton = new QPushButton(tr("Recalculate"), this);
    recalculateAwardsButton->setToolTip(tr("Click to recalculate the award status"));

    scoreTextEdit = new QTextEdit;

    //distShortLabelN = new QLabel;
    //distLongLabelN = new QLabel;

    configFileName = util->getCfgFile();
    ctyDatFile = util->getCTYFile();
    //#ifdef Q_OS_WIN
        //qDebug() << "WINDOWS DETECTED!"  << endl;
        //klogDir = QDir::homePath()+"/kontest";  // We create the \kontest for the logs and data
    //    configFileName = klogDir+"/klogrc.cfg";
    //    ctyDatFile = klogDir+"/cty.csv";
    //#else
       //qDebug() << "NO WINDOWS DETECTED!"  << endl;
        //klogDir = QDir::homePath()+"/.kontest";  // We create the ~/.kontest for the logs and data
 //       configFileName = klogDir+"/klogrc";
 //       ctyDatFile = klogDir+"/cty.csv";
  //  #endif

    downloadcty = new DownLoadCTY(klogDir, softwareVersion);
    connect( downloadcty, SIGNAL(done()), this, SLOT(slotWorldReload()) );

   //qDebug() << "MainWindow::MainWindow: logbook: " << QString(util->getKLogDBFile()) << endl;

        bool existingData = QFile::exists(util->getKLogDBFile());

        if (existingData)
        {
         //qDebug() << "MainWindow::MainWindow: existing data" << endl;
           //configured= false;
        }
        else
        {
          //qDebug() << "MainWindow::MainWindow: NOT existing data" << endl;
        }

    statusBarMessage = tr("Starting KLog");

    if (!QDir::setCurrent ( klogDir )){
        QDir d1(klogDir);
        if (d1.mkdir(klogDir))
        {
            QDir::setCurrent ( klogDir );
        }
    }

    //qDebug() << "MainWindow::MainWindow: 3" << endl;


    DBinMemory = false;
    //db = new DataBase(softwareVersion, DBinMemory);
    db = new DataBase(softwareVersion);
  //qDebug() << "MainWindow::MainWindow: 4" << endl;
    world = new World(klogDir, softwareVersion);
    //qDebug() << "MainWindow::MainWindow: 5" << endl;
    if (!db->createConnection())
    {
        //qDebug() << "MainWindow::MainWindow: Conection not created" << endl;
        return;
    }
    else
    {
        db->updateIfNeeded(); // Check if we need to update the DB
        //qDebug() << "MainWindow::MainWindow: DB Updated" << endl;
        if (!existingData)
        {
            //qDebug() << "MainWindow::MainWindow: !existingData" << endl;

            world->create(ctyDatFile);
            //entitiesList = world->getEntitiesNames();
            //createData();
        }else
        {
            //qDebug() << "MainWindow::MainWindow: existingData" << endl;
        }
    }
    //qDebug() << "MainWindow::MainWindow: proxy to be created" << endl;
    dataProxy = new DataProxy_SQLite();
    //propModeList = dataProxy->getPropModeList();

 //qDebug() << "MainWindow::MainWindow: setupDialog to be created" << endl;
    //setupDialog = new SetupDialog(!configured);
    setupDialog = new SetupDialog(configFileName, softwareVersion, 0, !configured);
 //qDebug() << "MainWindow::MainWindow: satTabWidget to be created" << endl;
    satTabWidget = new MainWindowSatTab();
    myDataTabWidget = new MainWindowMyDataTab();
    commentTabWidget = new MainWindowInputComment();
    othersTabWidget = new MainWindowInputOthers();
    eQSLTabWidget = new MainWindowInputEQSL();
    QSLTabWidget = new MainWindowInputQSL();

  //qDebug() << "MainWindow::MainWindow: fileManager to be created" << endl;
    //filemanager = new FileManager(klogDir, softwareVersion, *db);
    filemanager = new FileManager(klogDir, softwareVersion);

 //qDebug() << "MainWindow::MainWindow: locator to be created" << endl;
    locator = new Locator();
 //qDebug() << "MainWindow::MainWindow: awards to be created" << endl;
    awards = new Awards();
    awards->setManageModes(manageMode);
    //qDebug() << "MainWindow::MainWindow: awards already created" << endl;
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
 //qDebug() << "MainWindow::MainWindow: 8" << endl;
    dateTime = new QDateTime();
    selectedYear = (dateTime->currentDateTime()).date().year();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()) );
    timer->start(1000);

    previousQrz = "";
    qrzLineEdit = new QLineEdit;    
    nameLineEdit = new QLineEdit;
    qthLineEdit = new QLineEdit;
    locatorLineEdit = new QLineEdit;
    rstTXLineEdit = new QLineEdit;
    rstRXLineEdit = new QLineEdit;
    STXLineEdit = new QLineEdit;
    SRXLineEdit = new QLineEdit;
    bandComboBox = new QComboBox;
    modeComboBox = new QComboBox;

    dateEdit = new QDateEdit;
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    timeEdit = new QTimeEdit;

    OKButton = new QPushButton(tr("&Add"), this);
    //spotItButton = new QPushButton(tr("&Spot"), this);
    //spotItButton->setEnabled(false);
    clearButton = new QPushButton(tr("&Clear"), this);

    // UI DX
    infoLabel1 = new QLabel(tr("Status bar..."));
    infoLabel2 = new QLabel(tr("DX Entity"));
/*
    bandLabel1 = new QLabel(tr("10M"));
    bandLabel2 = new QLabel(tr("15M"));
    bandLabel3 = new QLabel(tr("20M"));
    bandLabel4 = new QLabel(tr("40M"));
    bandLabel5 = new QLabel(tr("80M"));
    bandLabel6 = new QLabel(tr("160M"));
    bandLabel7 = new QLabel(tr("2M"));
    bandLabel8 = new QLabel(tr("6M"));
    bandLabel9 = new QLabel(tr("12M"));
    bandLabel10 = new QLabel(tr("17M"));
    bandLabel11 = new QLabel(tr("30M"));
    bandLabel12 = new QLabel(tr("70CM"));

    continentLabel = new QLabel;
    prefixLabel = new QLabel;
    cqzLabel = new QLabel;
    ituzLabel = new QLabel;
    gradShortLabel = new QLabel;
    distShortLabel = new QLabel;
    gradLongLabel = new QLabel;
    distLongLabel = new QLabel;
*/
    //logPanel = new QWidget;
    loggWinAct = new QAction(tr("&Log Window"), this);
    scoreeWinAct = new QAction(tr("&Score Window"), this);

    scoreWindow = new QWidget;
    operatorLineEdit = new QLineEdit;
    stationCallSignLineEdit = new QLineEdit;
    myLocatorLineEdit = new QLineEdit;

    //myPowerSpinBox = new QDoubleSpinBox;
    //myPowerSpinBox->setDecimals(2);
    //myPowerSpinBox->setMaximum(9999);

    rxPowerSpinBox = new QDoubleSpinBox;
    rxPowerSpinBox->setDecimals(2);
    rxPowerSpinBox->setMaximum(9999);
    rxPowerSpinBox->setSuffix(tr("Watts"));

    txFreqSpinBox = new QDoubleSpinBox;
    txFreqSpinBox->setDecimals(3);
    txFreqSpinBox->setMaximum(9999);
    txFreqSpinBox->setSuffix(tr("MHz"));

    rxFreqSpinBox = new QDoubleSpinBox;
    rxFreqSpinBox->setDecimals(3);
    rxFreqSpinBox->setMaximum(9999);
    rxFreqSpinBox->setSuffix(tr("MHz"));

    dxccConfirmedQLCDNumber = new QLCDNumber;
    dxccWorkedQLCDNumber = new QLCDNumber;
    wazConfirmedQLCDNumber = new QLCDNumber;
    wazWorkedQLCDNumber = new QLCDNumber;
    localConfirmedQLCDNumber = new QLCDNumber;
    localWorkedQLCDNumber = new QLCDNumber;
    qsoConfirmedQLCDNumber = new QLCDNumber;
    qsoWorkedQLCDNumber = new QLCDNumber;
    dxMarathonDXCCQLCDNumber = new QLCDNumber;
    dxMarathonCQQLCDNumber = new QLCDNumber;
    dxMarathonPointsQLCDNumber = new QLCDNumber;
    operatingYearsComboBox = new QComboBox;

    qsoWorkedQLCDNumber->setDigitCount(7);
    qsoConfirmedQLCDNumber->setDigitCount(7);

    //qslSentComboBox = new QComboBox;
    //qslRecComboBox = new QComboBox;
/*
    //eQSL

    eqslSentComboBox = new QComboBox;
    eqslRecComboBox = new QComboBox;
    lotwSentComboBox = new QComboBox;
    lotwRecComboBox = new QComboBox;
    clublogComboBox = new QComboBox;


    QStringList qsAux;
    qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Validated");
    eqslRecComboBox->addItems(qsAux);
    lotwRecComboBox->addItems(qsAux);

    qsAux.clear();
    qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    eqslSentComboBox->addItems(qsAux);
    lotwSentComboBox->addItems(qsAux);

    qslSentComboBox->addItems(qsAux);

    qsAux.clear();
    qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
    qslRecComboBox->addItems(qsAux);

    qsAux.clear();
    qsAux << tr("Y-Uploaded") << tr("N-Do not upload") << tr("M-Modified");
    clublogComboBox->addItems(qsAux);
*/
    //qslSentViaComboBox = new QComboBox;
    //qslRecViaComboBox = new QComboBox;
/*
    QStringList qsAux;
    qsAux.clear();
    qsAux << tr("B-Bureau") << tr("D-Direct") << tr("E-Electronic") << tr("M-Manager");    
    qslSentViaComboBox->addItems(qsAux);
    qslRecViaComboBox->addItems(qsAux);


    eqslSentQDateEdit = new QDateEdit;
    eqslRecQDateEdit = new QDateEdit;
    lotwSentQDateEdit = new QDateEdit;
    lotwRecQDateEdit = new QDateEdit;
    clublogQDateEdit = new QDateEdit;

    qslSentQDateEdit = new QDateEdit;
    qslRecQDateEdit = new QDateEdit;
    qslmsgTextEdit = new QTextEdit;
    qslViaLineEdit = new QLineEdit;
*/
    //clublogComboBox->setCurrentIndex(1); // Do not upload

    //qslRecComboBox->setCurrentIndex(1); // Not received
    //qslSentComboBox->setCurrentIndex(1); // Not sent
/*
    eqslSentComboBox->setCurrentIndex(1);
    eqslRecComboBox->setCurrentIndex(1);
    lotwSentComboBox->setCurrentIndex(1);
    lotwRecComboBox->setCurrentIndex(1);
*/
    // Check date & time and set them in the UI at the begining
    dateTime->currentDateTime();
    dateEdit->setDate((dateTime->currentDateTime()).date());
    timeEdit->setTime((dateTime->currentDateTime()).time());

    //Search tab
    //searchBoxLineEdit = new QLineEdit;

    // UI DX

    // CLUSTER
 //qDebug() << "MainWindow::MainWindow: dxclusterwidget to be created" << endl;
    dxClusterWidget = new DXClusterWidget(dxclusterServerToConnect , dxclusterServerPort, this);


    // </CLUSTER>
    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);

    // </UI>

//**************************************************

    //createDXClusterUI();
    connect( setupDialog, SIGNAL(exitSignal(int)), this, SLOT(slotExitFromSlotDialog(int)) );

 //qDebug() << "MainWindow::MainWindow:  readconfigdata" << endl;
    readConfigData();
    if (needToEnd)
    {
        //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db->compress();
      //qDebug() << "MainWindow::MainWindow: 12.5" << endl;
       exit(0);
    }

    //qDebug() << "MainWindow::MainWindow:  UI to be created" << endl;


    logWindow->createlogPanel(currentLog);

    createUI();


    //createSearchResultsPanel();
    loggWinAct->setShortcut(Qt::CTRL + Qt::Key_L);
    connect(loggWinAct, SIGNAL(triggered()), this, SLOT(slotLogWinShow()));

    //logPanel->addAction(loggWinAct);
    //logPanel->addAction(scoreeWinAct);

    scoreeWinAct->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(scoreeWinAct, SIGNAL(triggered()), this, SLOT(slotScoreWinShow()));
    scoreWindow->addAction(scoreeWinAct);
    scoreWindow->addAction(loggWinAct);
    createScorePanel();

    setWindowTitle(tr("KLog"));


 //qDebug() << "MainWindow::MainWindow: 16" << endl;
    if (dataProxy->getNumberOfManagedLogs()<1)
    {
      //qDebug() << "MainWindow::MainWindow: 16.1" << endl;
        slotSetup(6);
       //qDebug() << "MainWindow::MainWindow: 16.2" << endl;
    }
   //qDebug() << "MainWindow::MainWindow: 17" << endl;
    checkIfNewBandOrMode();
   //qDebug() << "MainWindow::MainWindow: 18" << endl;
    if ( (contestMode == CQ_WW_SSB) || (contestMode == CQ_WW_CW) )
    {
        //qDebug() << "MainWindow::MainWindow: 18.1" << endl;
    }
    else if ( (contestMode == CQ_WPX_SSB) || (contestMode == CQ_WPX_CW) )
    {
        //qDebug() << "MainWindow::MainWindow: 18.2" << endl;
    }
    else
    {
       //qDebug() << "MainWindow::MainWindow: 18.3" << endl;
        if (dataProxy->getLastQSOid()<=1)
        {
           //qDebug() << "MainWindow::MainWindow: 18.4" << endl;
            operatingYearsComboBox->addItem(QString::number(selectedYear));
        }
        else
        {
            //qDebug() << "MainWindow::MainWindow: 18.5 - currentLog: " << QString::number(currentLog) << endl;
            operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));
            //qDebug() << "MainWindow::MainWindow: 18.5.1 - currentLog: " << QString::number(currentLog) << endl;
            operatingYearsComboBox->setCurrentIndex(operatingYearsComboBox->findText((dataProxy->getOperatingYears(currentLog)).last()));
            //qDebug() << "MainWindow::MainWindow: 18.5.2" << endl;
        }
        //qDebug() << "MainWindow::MainWindow: 18.6." << endl;
        updateQSLRecAndSent();
        //qDebug() << "MainWindow::MainWindow: 18.7" << endl;
        awards->recalculateAwards();
        //qDebug() << "MainWindow::MainWindow: 18.8" << endl;
        showAwards();
        //qDebug() << "MainWindow::MainWindow: 18.9" << endl;
        dxClusterWidget->setCurrentLog(currentLog);
        //qDebug() << "MainWindow::MainWindow: 18.10" << endl;
    }

  //qDebug() << "MainWindow::MainWindow: 19" << endl;
    currentBandShown = dataProxy->getIdFromBandName(bandComboBox->currentText());
    currentModeShown = dataProxy->getIdFromModeName(modeComboBox->currentText());
    currentBand = currentBandShown;
    currentMode = currentModeShown;
   //qDebug() << "MainWindow::MainWindow: 20 - currentMode: " << QString::number(currentMode) << endl;
   //qDebug() << "MainWindow::MainWindow: 21 - currentBand: " << QString::number(currentBand) << endl;
   //qDebug() << "MainWindow::MainWindow: 21.1 - currentModeShown: " << QString::number(currentModeShown) << endl;
   //qDebug() << "MainWindow::MainWindow: 21.2 - currentBandShown: " << QString::number(currentBandShown) << endl;


    slotClearButtonClicked();

    upAndRunning = true;

    //qDebug() << "MainWindow::MainWindow: END: "<<  (QTime::currentTime() - start).toString("hhmmsszzz")<< endl;
   //qDebug() << "MainWindow::MainWindow: "<<  (QTime::currentTime()).toString("hhmmsszzz")<< endl;

    //qDebug() << "MainWindow::MainWindow: Software update to be created" << endl;
    softUpdate = new SoftwareUpdate(softwareVersion);
    //connect(softUpdate, SIGNAL(updateNeededSignal(bool)), this, SLOT(slotShowSoftUpdateResults(bool) ) );
    callingUpdate = false; // to control whether the update is mannually launched or at the begining


   //qDebug() << "MainWindow::MainWindow: calling Software update..." << endl;
    if (checkNewVersions)
    {//reportInfo
        if (reportInfo)
        {
            softUpdate->addCall(stationQRZ);

        }
        softUpdate->needToUpdate();
    }

   //qDebug() << "MainWindow::MainWindow: END" << endl;

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createUI()
{

 //qDebug() << "MainWindow::createUI" << endl;
    createStatusBar();

    if ( (contestMode == CQ_WW_SSB) || (contestMode == CQ_WW_CW) )
    {
        createUICQWW();
        createActionsCommon();
        createActionsCQWW();
        createMenusCommon();
        createMenusCQWW();

    }
    else if ( (contestMode == CQ_WPX_SSB) || (contestMode == CQ_WPX_CW) )
    {

    }
    else
    {
        createUIDX();
        createActionsCommon();
        createActionsDX();
        createMenusCommon();
    }


}

void MainWindow::slotModeComboBoxChanged()
{
   //qDebug() << "MainWindow::slotModeComboBoxChanged: " << QString::number(modeComboBox->currentIndex()) << endl;

    int i;
    i = dataProxy->getSubModeIdFromSubMode(modeComboBox->currentText());
    if (i>=0)
    {
       //qDebug() << "MainWindow::MainWindow: 5 - currentMode: " << QString::number(currentMode) << endl;
        currentMode = i;
       //qDebug() << "MainWindow::MainWindow: 6 - currentMode: " << QString::number(currentMode) << endl;
    }
   //qDebug() << "MainWindow::slotModeComboBoxChanged: i: " << QString::number(i) << endl;
   //qDebug() << "MainWindow::slotModeComboBoxChanged: currentMode: " << QString::number(currentMode) << endl;

    //qDebug() << "MainWindow::slotModeComboBoxChanged: " << QString::number(modeComboBox->currentIndex()) << "/" << QString::number(currentMode) << endl;
    currentBandShown = dataProxy->getIdFromBandName(bandComboBox->currentText());
    currentModeShown = dataProxy->getIdFromModeName(modeComboBox->currentText());
    currentBand = currentBandShown;
   //qDebug() << "MainWindow::MainWindow: 7 - currentMode: " << QString::number(currentMode) << endl;
    currentMode = currentModeShown;
   //qDebug() << "MainWindow::MainWindow: 8 - currentMode: " << QString::number(currentMode) << endl;
   //qDebug() << "MainWindow::MainWindow: 8.1 - currentBand: " << QString::number(currentBand) << endl;
   //qDebug() << "MainWindow::MainWindow: 8.2 - currentModeShown: " << QString::number(currentModeShown) << endl;
   //qDebug() << "MainWindow::MainWindow: 8.3 - currentBandShown: " << QString::number(currentBandShown) << endl;

    //currentBandShown = bandComboBox->currentIndex();
    //currentModeShown = modeComboBox->currentIndex();
    checkIfWorkedB4(currentQrz);

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);
    showStatusOfDXCC(_qs);
}

void MainWindow::slotBandComboBoxChanged(){
   //qDebug() << "MainWindow::slotBandComboBoxChanged: " << QString::number(bandComboBox->currentIndex()) << endl;
    int i;
    i = dataProxy->getIdFromBandName(bandComboBox->currentText());
    if (i>=0)
    {
        currentBand = i;
        //txFreqSpinBox->setValue(dataProxy->getFreqFromBandId(i));
    }

    //qDebug() << "MainWindow::slotBandComboBoxChanged: " << QString::number(bandComboBox->currentIndex()) << "/" << QString::number(currentBand) << endl;

    currentBandShown = dataProxy->getIdFromBandName(bandComboBox->currentText());
    currentModeShown = dataProxy->getIdFromModeName(modeComboBox->currentText());
    currentBand = currentBandShown;
   //qDebug() << "MainWindow::MainWindow: 9 - currentMode: " << QString::number(currentMode) << endl;
    currentMode = currentModeShown;
   //qDebug() << "MainWindow::MainWindow: 9 - currentMode: " << QString::number(currentMode) << endl;
   //qDebug() << "MainWindow::MainWindow: 9.1 - currentMode: " << QString::number(currentMode) << endl;
   //qDebug() << "MainWindow::MainWindow: 9.2 - currentBand: " << QString::number(currentBand) << endl;
   //qDebug() << "MainWindow::MainWindow: 9.3 - currentModeShown: " << QString::number(currentModeShown) << endl;
   //qDebug() << "MainWindow::MainWindow: 9.4 - currentBandShown: " << QString::number(currentBandShown) << endl;


       //currentModeShown = modeComboBox->currentIndex();
    checkIfWorkedB4(currentQrz);

    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << QString::number(currentEntity) << QString::number(currentBandShown) << QString::number(currentModeShown) << QString::number(currentLog);


    showStatusOfDXCC(_qs);
}


void MainWindow::slotQRZReturnPressed()
{
   //qDebug() << "MainWindow::slotQRZReturnPressed: " << qrzLineEdit->text() << " - " << QString::number(bandComboBox->currentIndex()) << "/" << QString::number(modeComboBox->currentIndex()) << endl;
    //int newId = -1;
    int lastId = -1;
    int errorCode = 0;
    QString aux;
    //int _x; //for clublog management

    //bool ret = false;
    QString tqrz = qrzLineEdit->text();
    //currentMode = modeComboBox->currentIndex();
    //currentBandShown = bandComboBox->currentIndex();
    slotBandComboBoxChanged();
    slotModeComboBoxChanged();


    switch (contestMode)
    { // Just to prepare or some tasks before reading DATA from UI
        case CQ_WW_SSB:
        //qDebug() << "MainWindow::slotQRZReturnPressed: CQ-WW-SSB:" << endl;
            if ((SRXLineEdit->text()).toInt() < 1    ){
            return;
            }
        break;
        case CQ_WW_CW:
            //qDebug() << "MainWindow::slotQRZReturnPressed: CQ-WW-CW:" << endl;
            break;
        default:
          //qDebug() << "MainWindow::slotQRZReturnPressed: Default:" << endl;
        break;

        }


        //http://www.sqlite.org/autoinc.html
        // NULL = is the keyword for the autoincrement to generate next value

        QSqlQuery query;
        QString queryString = readDataFromUI();

       //qDebug() << "MainWindow::slotQRZReturnPressed: queryString: " << queryString << endl;

        if (queryString != "NULL") {
            if (!query.exec(queryString))
            {
               //qDebug() << "MainWindow::slotQRZReturnPressed: Query ERROR: (queryString): " << queryString << endl;
                errorCode = query.lastError().number();
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                aux = tr("An unexpected error ocurred when trying to add the QSO to your log. If the problem persists, please contact the developer for analysis: ");
                msgBox.setText(aux + "MW-1#" + QString::number(errorCode));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                int ret = msgBox.exec();
                switch (ret)
                {
                    case QMessageBox::Ok:
                    break;
                    default:
                    // should never be reached
                    break;
                }

                return;
            }
            else
            {
                //TODO: To move the following lines to this part to properly manage the query result!!
                //ret = true;
              //qDebug() << "MainWindow::slotQRZReturnPressed: QSO Added! " << endl;

                needToSave = true;
                if (modify)
                {
                  //qDebug() << "MainWindow::slotQRZReturnPressed: Modifying! " << endl;

                    if(modifyingQSO>0)
                    {
                        awards->setAwards(modifyingQSO);

                        if ((clublogActive) & (clublogRealTime))
                        {
                           //qDebug() << "MainWindow::slotQRZReturnPressed: (Modifiying ClubLog) Lastid: "<< QString::number(lastId) << endl;
                            // Delete QSO in CLubLog
                            elogClublog->deleteQSO(clublogPrevQSO);
                            // Add modified QSO in ClubLog
                            elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(modifyingQSO));

                        }
                        else
                        {
                           //qDebug() << "MainWindow::slotQRZReturnPressed: (No ClubLog) Lastid: "<< QString::number(lastId) << endl;
                        }


                    }

                    // CHECK WHAT WAS THE QSOID to add the awards, if needed
                }
                else
                {
                  //qDebug() << "MainWindow::slotQRZReturnPressed: Not Modifying " << endl;
                    lastId = dataProxy->getLastQSOid();
                    if (lastId>=0)
                    {
                      //qDebug() << "MainWindow::slotQRZReturnPressed: Lastid: "<< QString::number(lastId) << endl;
                        awards->setAwards(lastId);   //Update the DXCC award status

                        // Send to CLUBLOG if enabled

                        if (clublogActive)
                        {
                           //qDebug() << "MainWindow::slotQRZReturnPressed: clublogActive TRUE" << endl;
                        }
                        else
                        {
                           //qDebug() << "MainWindow::slotQRZReturnPressed: clublogActive FALSE" << endl;
                        }
                        if (clublogRealTime)
                        {
                           //qDebug() << "MainWindow::slotQRZReturnPressed: clublogRealTime TRUE" << endl;
                        }
                        else
                        {
                           //qDebug() << "MainWindow::slotQRZReturnPressed: clublogRealTime FALSE" << endl;
                        }


                        if ((clublogActive) & (clublogRealTime))
                        {
                           //qDebug() << "MainWindow::slotQRZReturnPressed: (Sending ClubLog) Lastid: "<< QString::number(lastId) << endl;
                            elogClublog->sendQSO(dataProxy->getClubLogRealTimeFromId(lastId));

                        }
                        else
                        {
                           //qDebug() << "MainWindow::slotQRZReturnPressed: (No ClubLog) Lastid: "<< QString::number(lastId) << endl;
                        }
                        //<CLUBLOG>
                    }
                }
                //slotShowAwards();
                logWindow->refresh();
                dxccStatusWidget->refresh();
                slotClearButtonClicked();
            }
        }
        else   // The QUERY string is NULL
        {
            return;
           //qDebug() << "MainWindow::slotQRZReturnPressed: queryString-NULL: " << queryString << endl;
        }


    modify = false;
    modifyingQSO = -1;
    OKButton->setText(tr("&Add"));
}

QString MainWindow::readDataFromUI()
{

    //qDebug() << "MainWindow::readDataFromUI: " << endl;

    QString tqrz = (qrzLineEdit->text()).toUpper();
    if (tqrz.length()<3)
    {
        return "NULL";
    }

    switch (contestMode)
    {
        case CQ_WW_SSB:
            //return readDataFromUICQWWSSB();
        break;
        case CQ_WW_CW:
            //return readDataFromUICQWWCW();
          break;
        default:
            if (modify)
            {
                return readDataFromUIDXModifying();
            }
            else
            {
                return readDataFromUIDX();
            }
        break;
    }

    //qDebug() << "MainWindow::readDataFromUI: END" << endl;
    return "NULL";
}

QString MainWindow::readDataFromUIDX()
{
/*
If you make any change here, please update also readDataFromUIDXModifying to keep data integrity!
*/
   //qDebug() << "MainWindow::readDataFromUIDX:" << endl;

    QString tqrz = (qrzLineEdit->text()).toUpper();
    if (tqrz.length()<3)
    {
        return "NULL";
    }

    QString stringQuery = "NULL";
    QString aux1, aux2, stringFields, stringData;
    //QString aux, aux2;
    int tband = currentBand;
    int tmode = currentMode;

    QString tdate = (dateEdit->date()).toString("yyyy/MM/dd");
    QString ttime = (timeEdit->time()).toString("hh:mm:ss");

    QString trsttx = rstTXLineEdit->text();
    QString trstrx = rstRXLineEdit->text();

    int dxcc = world->getQRZARRLId(tqrz);
    //int dxcc2 = getDXCCFromComboBox();
    int dxcc2 = world->getQRZARRLId(othersTabWidget->getEntityPrefix());
   //qDebug() << "MainWindow::readDataFromUIDX - DXCC: " << QString::number(dxcc) << endl;
   //qDebug() << "MainWindow::readDataFromUIDX - DXCC2: " << QString::number(dxcc2) << endl;
    dxcc = util->getNormalizedDXCCValue(dxcc);
    dxcc2 = util->getNormalizedDXCCValue(dxcc2);

    if (dxcc!=dxcc2)
    {
        QString dxccn1 = world->getEntityName(dxcc);
        dxccn1 = dxccn1 + " - " + world->getEntityMainPrefix(dxcc);

        QString dxccn2 = world->getEntityName(dxcc2);
        dxccn2 = dxccn2 + " - " + world->getEntityMainPrefix(dxcc2);

        QPushButton *button2 = new QPushButton(this);
        QPushButton *button1 = new QPushButton(this);

        button1->setText(world->getEntityMainPrefix(dxcc));
        button2->setText(world->getEntityMainPrefix(dxcc2));

        int ret;

        QMessageBox msgBox;
        msgBox.setText( tr("You have selected an entity:\n\n")+"- "+dxccn2+"\n\n"+tr("that is different from the KLog proposed entity:\n\n")+ "- "+dxccn1+"\n\n"
                        +tr("Click on the prefix of the correct entity or Cancel to edit the QSO again."));

        msgBox.addButton(button2, QMessageBox::AcceptRole);
        msgBox.addButton(button1, QMessageBox::ActionRole);
        msgBox.addButton(QMessageBox::Cancel);
        ret = msgBox.exec();

        if (ret == QMessageBox::AcceptRole)
        {
            dxcc = dxcc2;
        }
        else if (ret == QMessageBox::Cancel)
        {
            return  "NULL";
        }
        else
        {}
    }

    int cqz = world->getEntityCqz(dxcc);
    int ituz = world->getEntityItuz(dxcc);

    aux1 = nameLineEdit->text();
    if (aux1.length()>1)
    {
        stringFields = stringFields + ", name";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = (locatorLineEdit->text()).toUpper();

    if ( locator->isValidLocator(aux1)  )
    {
        stringFields = stringFields + ", gridsquare";
        stringData = stringData + ", '" + aux1 + "'";
    }

    if ( (txFreqSpinBox->value()) > 0  )
    {
        aux1 = QString::number(txFreqSpinBox->value());

        if (db->isThisFreqInBand(db->getBandNameFromID2(tband), aux1) )
        {
            stringFields = stringFields + ", freq";
            stringData = stringData + ", '" + aux1 + "'";
            //qDebug() << "MainWindow::readDataFromUIDX: FREQ & BAND OK" << endl;
        }
        else
        {
            //qDebug() << "MainWindow::readDataFromUIDX: FREQ & BAND NOK" << endl;
        }
    }

    if ( (rxFreqSpinBox->value()) > 0  )
    {
        aux1 = QString::number(rxFreqSpinBox->value());
        stringFields = stringFields + ", freq_rx";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = qthLineEdit->text();
    if (aux1.length()>2)
    {
        stringFields = stringFields + ", qth";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getOperator();
    //aux1 = operatorLineEdit->text();
    if (aux1.length()>2)
    {
        //lastOperatorQRZ = aux1.toUpper();
        stringFields = stringFields + ", operator";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getStationQRZ();
    //aux1 = (stationCallSignLineEdit->text()).toUpper();
    if (aux1.length()>2)
    {
        //lastStationQRZ = aux1.toUpper();
        stringFields = stringFields + ", station_callsign";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = myDataTabWidget->getMyLocator();
    //aux1 = myLocatorLineEdit->text();
    if (aux1.length()>2)
    {                   

        //lastMyLocator = aux1.toUpper();
        stringFields = stringFields + ", my_gridsquare";
        stringData = stringData + ", '" + aux1 + "'";
    }
    aux1 = commentTabWidget->getComment();
    //aux1 = commentLineEdit->text();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", comment";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QSLTabWidget->getQSLMsg();
    //aux1 = qslmsgTextEdit->toPlainText();
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", qslmsg";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(dxcc);
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", dxcc";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(cqz);
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", cqz";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(ituz);
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", ituz";
        stringData = stringData + ", '" + aux1 + "'";
    }


    aux1 = QSLTabWidget->getQSLVia();
    //aux1 = qslViaLineEdit->text();
    if (aux1.length()>3)
    {
        stringFields = stringFields + ", qsl_via";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(myDataTabWidget->getMyPower());
    if ((aux1.toDouble())>0.0)
    {
        //lastPower = aux1.toDouble();
        stringFields = stringFields + ", tx_pwr";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = QString::number(rxPowerSpinBox->value());
    if ((aux1.toDouble())>0.0)
    {
        stringFields = stringFields + ", rx_pwr";
        stringData = stringData + ", '" + aux1 + "'";
    }

    aux1 = othersTabWidget->getIOTA();
   //qDebug() << "MainWindow::readDataFromUIDX: IOTA: " << aux1 << endl;
    if (aux1.length() == 6) // EU-001
    {
      //qDebug() << "MainWindow::readDataFromUIDX: IOTA to be saved" << endl;
        stringFields = stringFields + ", iota";
        stringData = stringData + ", '" + aux1 + "'";
    }
    else
    {
      //qDebug() << "MainWindow::readDataFromUIDX: IOTA NOT to be saved! Lenght="<<QString::number(aux1.length()) << endl;
    }
/*
    aux1 = iotaNumberLineEdit->text();
    if ( (aux1.toInt()) > 0 )
    {
        aux2 = iotaContinentComboBox->currentText() + "-" + aux1;
        aux1 = awards->checkIfValidIOTA(aux2);
       //qDebug() << "MainWindow::readDataFromUIDX: pre-IOTA-CheckIfValidIOTA-1: " << aux2 << endl;
       //qDebug() << "MainWindow::readDataFromUIDX: post-IOTA-CheckIfValidIOTA-2: " << aux1 << endl;
        if (aux1.length() == 6) // EU-001
        {
           //qDebug() << "MainWindow::readDataFromUIDX: IOTA to be saved!" << endl;
            stringFields = stringFields + ", iota";
            stringData = stringData + ", '" + aux1 + "'";
        }
        else
        {
           //qDebug() << "MainWindow::readDataFromUIDX: IOTA NOT to be saved! Lenght="<<QString::number(aux1.length()) << endl;
        }
    }
    */
    // EQSL-SENT

    aux1 = satTabWidget->getSatName(); //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
   //qDebug() << "MainWindow::readDataFromUIDX: SAT1 " << aux1 << endl;
   //stringFields = stringFields + ", sat_name";
   //stringData = stringData + ", '" + aux1 + "'";
    if (aux1.length()>0)
    {
        stringFields = stringFields + ", sat_name";
        stringData = stringData + ", '" + aux1 + "'";
    }
   // aux1 = satTabWidget->getSatMode();

    aux1 = satTabWidget->getSatMode(); // We are assuming that the SAT_MODE is always well provided. If it is blank, then no SAT QSO
    //stringFields = stringFields + ", sat_mode";
    //stringData = stringData + ", '" + aux1 + "'";

    if (aux1.length()>0)
    {
        stringFields = stringFields + ", sat_mode";
        stringData = stringData + ", '" + aux1 + "'";
    }

    keepSatPage = satTabWidget->getRepeatThis();

    aux1 = othersTabWidget->getPropModeFromComboBox();
    if ((aux1.length()>0) && (aux1 != "Not"))
    {
        stringFields = stringFields + ", prop_mode";
        stringData = stringData + ", '" + aux1 + "'";
    }

    //CLUBLOG

    aux1 = eQSLTabWidget->getClubLogStatus(); //Y, N, M
    if (aux1 == "Y")
    {
        stringFields = stringFields + ", clublog_qso_upload_status";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", clublog_qso_upload_date";
        stringData = stringData + ", '" + (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "N")
    {
        stringFields = stringFields + ", clublog_qso_upload_status";
        stringData = stringData + ", 'N'";
    }
    else if (aux1 == "M")
    {
        stringFields = stringFields + ", clublog_qso_upload_status";
        stringData = stringData + ", 'M'";
        stringFields = stringFields + ", clublog_qso_upload_date";
        stringData = stringData + ", '" + (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd") + "'";
    }
    else //TODO: This should be equivalent to N?
    {
        stringFields = stringFields + ", clublog_qso_upload_status";
        stringData = stringData + ", 'N'";
    }

    //CLUBLOG

    aux1 = eQSLTabWidget->getEQSLSenStatus();
    if (aux1 == "Y")
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", eqsl_qslsdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'R'";
    }
    else if (aux1 == "Q")
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'Q'";
        stringFields = stringFields + ", eqsl_qslsdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", eqsl_qslsdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "'";
    }
    else // N
    {
        stringFields = stringFields + ", eqsl_qsl_sent";
        stringData = stringData + ", 'N'";
    }

    aux1 = eQSLTabWidget->getEQSLRecStatus();
    if (aux1 == "Y")
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", eqsl_qslrdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'R'";
    }
    else if (aux1 == "Q")
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'Q'";
        stringFields = stringFields + ", eqsl_qslrdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", eqsl_qslrdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "'";
    }
    else
    {
        stringFields = stringFields + ", eqsl_qsl_rcvd";
        stringData = stringData + ", 'N'";
    }


    // LOTW-SENT


    aux1 = eQSLTabWidget->getLOTWSenStatus();
    if (aux1 == "Y")
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", lotw_qslsdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'R'";
    }
    else if (aux1 == "Q")
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'Q'";
        stringFields = stringFields + ", lotw_qslsdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", lotw_qslsdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "'";
    }
    else
    {
        stringFields = stringFields + ", lotw_qsl_sent";
        stringData = stringData + ", 'N'";
    }


    // LOTW-RECEPTION    
    //LOTW_QSLRDATE: (only valid if LOTW_RCVD is Y, I, or V)

    aux1 = eQSLTabWidget->getLOTWRecStatus();
    if (aux1 == "Y")
    {
            stringFields = stringFields + ", lotw_qsl_rcvd";
            stringData = stringData + ", 'Y'";
            stringFields = stringFields + ", lotw_qslrdate";
            stringData = stringData + ", '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", lotw_qsl_rcvd";
        stringData = stringData + ", 'R'";
    }
    else if (aux1 == "V")
    {
        stringFields = stringFields + ", lotw_qsl_rcvd";
        stringData = stringData + ", 'V'";
        stringFields = stringFields + ", lotw_qslrdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "'";
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", lotw_qsl_rcvd";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", lotw_qslrdate";
        stringData = stringData + ", '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "'";
    }
    else
    {
        stringFields = stringFields + ", lotw_qsl_rcvd";
        stringData = stringData + ", 'N'";
    }


    //QSLTABWidget

    // QSL SENT: Y/N/R/Q/I
    // QSL_VIA: B/D/E/M

    aux1 = QSLTabWidget->getQSLSenStatus();
    aux2 = QSLTabWidget->getSentVia();
   //qDebug() << "MainWindow::readDataFromUIDX: aux1: " << aux1 << " / aux2: " << aux2 << endl;

    //TODO: the aux2 switch is repeated and could be improved


    if (aux1=="Y")
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", qslsdate";
        stringData = stringData + ", '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "'";
        stringFields = stringFields + ", qsl_sent_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else if (aux1 == "R")
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'R'";
        stringFields = stringFields + ", qsl_sent_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else if (aux1 == "Q")
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'Q'";
        stringFields = stringFields + ", qslsdate";
        stringData = stringData + ", '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "'";
        stringFields = stringFields + ", qsl_sent_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else if (aux1 == "I")
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", qslsdate";
        stringData = stringData + ", '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "'";
        stringFields = stringFields + ", qsl_sent_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else
    {
        stringFields = stringFields + ", qsl_sent";
        stringData = stringData + ", 'N'";
        stringFields = stringFields + ", qsl_sent_via";
        stringData = stringData + ", 'B'";
    }


     // QSL RECEPTION
    //i = qslRecComboBox->currentIndex();
    //ii = qslRecViaComboBox->currentIndex();
    aux1 = QSLTabWidget->getQSLRecStatus(); // Y/N/R/I/V
    aux2 = QSLTabWidget->getRecVia();       // B/D/E/M

    if (aux1 == "Y")
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'Y'";
        stringFields = stringFields + ", qslrdate";
        stringData = stringData + ", '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "'";
        stringFields = stringFields + ", confirmed";
        stringData = stringData + ", '1'";
        stringFields = stringFields + ", qsl_rcvd_via";
        if (aux2 == "D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2 == "E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2 == "M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
         stringData = stringData + ", 'B'";
        }
    }
    else if (aux1 =="R")
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'R'";
        //stringFields = stringFields + ", confirmed";
        //stringData = stringData + ", '0'";
        stringFields = stringFields + ", qsl_rcvd_via";
        if (aux2=="D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2=="E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2=="M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
            stringData = stringData + ", 'B'";
        }
    }
    else if (aux1=="I")
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'I'";
        stringFields = stringFields + ", qslrdate";
        stringData = stringData + ", '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "'";
        //stringFields = stringFields + ", confirmed";
        //stringData = stringData + ", '0'";
        stringFields = stringFields + ", qsl_rcvd_via";
        if (aux2=="D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2=="E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2=="M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
            stringData = stringData + ", 'B'";
        }
    }
    else if (aux1=="V")
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'V'";
        stringFields = stringFields + ", qslrdate";
        stringData = stringData + ", '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "'";
        //TODO: Check if the QSL has been received or not as this "V" could mask a received QSL as a Worked (0)
        //stringFields = stringFields + ", confirmed";
        //stringData = stringData + ", '0'";
        stringFields = stringFields + ", qsl_rcvd_via";
        if (aux2=="D")
        {
            stringData = stringData + ", 'D'";
        }
        else if (aux2=="E")
        {
            stringData = stringData + ", 'E'";
        }
        else if (aux2=="M")
        {
            stringData = stringData + ", 'M'";
        }
        else
        {
            stringData = stringData + ", 'B'";
        }
    }
    else
    {
        stringFields = stringFields + ", qsl_rcvd";
        stringData = stringData + ", 'N'";
        stringFields = stringFields + ", qsl_rcvd_via";
        stringData = stringData + ", 'B'";
    }

    // The data reading finish here. Now, we prepare the data to insert into the DB

    if (stringFields.startsWith(", ") )
    {
        stringFields.remove(0,2);
    }

    stringFields += ", call, bandid, modeid, qso_date, time_on, lognumber, rst_sent, rst_rcvd";

    if (stringFields.startsWith(", ") )
    {
        stringFields.remove(0,2);
    }

    if (stringData.startsWith(", ") )
    {
        stringData.remove(0,1);
    }

    stringData.remove(0,1);
    stringData += QString(", '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8'").arg(tqrz).arg(tband).arg(tmode).arg(tdate).arg(ttime).arg(QString::number(currentLog)).arg(trsttx).arg(trstrx);

    if (stringData.startsWith(", ") )
    {
        stringData.remove(0,2);
    }

    stringQuery = "INSERT INTO log (" + stringFields + ") values (" + stringData +")" ;
    return stringQuery;
}



QString MainWindow::readDataFromUIDXModifying()
{
    //qDebug() << "MainWindow::readDataFromUIDXModifying:" << endl;

/*
UPDATE table_name
SET column1 = value1, column2 = value2...., columnN = valueN
WHERE [condition];
*/
    QString tqrz = (qrzLineEdit->text()).toUpper();
    if (tqrz.length()<3)
    {
        return "NULL";
    }

    QString stringQuery = "NULL";
    QString aux1, aux2;
    //QString aux, aux2;
    int tband = currentBand;
    int tmode = currentMode;

    QString tdate = (dateEdit->date()).toString("yyyy/MM/dd");
    QString ttime = (timeEdit->time()).toString("hh:mm:ss");

    QString trsttx = rstTXLineEdit->text();
    QString trstrx = rstRXLineEdit->text();

    int dxcc = world->getQRZARRLId(tqrz);
    int cqz = world->getEntityCqz(dxcc);
    int ituz = world->getEntityItuz(dxcc);


    /**/

    //int dxcc2 = getDXCCFromComboBox();
    int dxcc2 = world->getQRZARRLId(othersTabWidget->getEntityPrefix());
   //qDebug() << "MainWindow::readDataFromUIDXModifying - DXCC: " << QString::number(dxcc) << endl;
   //qDebug() << "MainWindow::readDataFromUIDXModifying- DXCC2: " << QString::number(dxcc2) << endl;
    dxcc = util->getNormalizedDXCCValue(dxcc);
    dxcc2 = util->getNormalizedDXCCValue(dxcc2);

    if (dxcc!=dxcc2)

    {
        QString dxccn1 = world->getEntityName(dxcc);
        dxccn1 = dxccn1 + " - " + world->getEntityMainPrefix(dxcc);

        QString dxccn2 = world->getEntityName(dxcc2);
        dxccn2 = dxccn2 + " - " + world->getEntityMainPrefix(dxcc2);

        QPushButton *button2 = new QPushButton(this);
        QPushButton *button1 = new QPushButton(this);

        button1->setText(world->getEntityMainPrefix(dxcc));
        button2->setText(world->getEntityMainPrefix(dxcc2));

        int ret;

        QMessageBox msgBox;
        msgBox.setText( tr("You have selected an entity:\n\n")+"- "+dxccn2+"\n\n"+tr("that is different from the KLog proposed entity:\n\n")+ "- "+dxccn1+"\n\n"
                        +tr("Click on the prefix of the right entity or Cancel to correct."));

        msgBox.addButton(button2, QMessageBox::AcceptRole);
        msgBox.addButton(button1, QMessageBox::ActionRole);
        msgBox.addButton(QMessageBox::Cancel);
        ret = msgBox.exec();

        if (ret == QMessageBox::AcceptRole)
        {
            dxcc = dxcc2;
        }
        else if (ret == QMessageBox::Cancel)
        {
            return  "NULL";
        }
        else
        {}
    }


    /**/


    QString updateString = "UPDATE log SET call = '" + tqrz + "', bandid = '" + QString::number(tband) + "', modeid = '" + QString::number(tmode) + "', qso_date = '" + tdate + "', time_on = '" + ttime + "', rst_sent = '" + trsttx + "', rst_rcvd = '" + trstrx + "', lognumber = '" + QString::number(currentLog) + "', ";

    aux1 = nameLineEdit->text();
    if (aux1.length()>1)
    {
        updateString = updateString + "name = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = (locatorLineEdit->text()).toUpper();
    if ( locator->isValidLocator(aux1)  )
    {
        updateString = updateString + "gridsquare = '";
        updateString = updateString + aux1 + "', ";

    }

    if ( (txFreqSpinBox->value()) > 0  )
    {
        aux1 = QString::number(txFreqSpinBox->value());

        if (db->isThisFreqInBand(db->getBandNameFromID2(tband), aux1) )
        {

            updateString = updateString + "freq = '";
            updateString = updateString + aux1 + "', ";

        }
        else
        {
        }

    }

    if ( (rxFreqSpinBox->value()) > 0  )
    {
        aux1 = QString::number(rxFreqSpinBox->value());
        updateString = updateString + "freq_rx = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = qthLineEdit->text();
    if (aux1.length()>2)
    {
        updateString = updateString + "qth = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = myDataTabWidget->getOperator();
    //aux1 = operatorLineEdit->text();
    if (aux1.length()>2)
    {
        updateString = updateString + "operator = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = myDataTabWidget->getStationQRZ();
    //aux1 = (stationCallSignLineEdit->text()).toUpper();
    if (aux1.length()>2)
    {
        updateString = updateString + "station_callsign = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = myDataTabWidget->getMyLocator();
    //aux1 = myLocatorLineEdit->text();
    if (aux1.length()>2)
    {
        updateString = updateString + "my_gridsquare = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = commentTabWidget->getComment();
    //aux1 = commentLineEdit->text();
    if (aux1.length()>0)
    {
        updateString = updateString + "comment = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QSLTabWidget->getQSLMsg();
    //aux1 = qslmsgTextEdit->toPlainText();
    if (aux1.length()>0)
    {
        updateString = updateString + "qslmsg = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QString::number(dxcc);
    //qDebug() << "MainWindow::readDataFromUIDXModifying: DXCC=" << aux1 << endl;
    if (aux1.length()>0)
    {
        updateString = updateString + "dxcc = '";
        updateString = updateString + aux1 + "', ";
        //qDebug() << "MainWindow::readDataFromUIDXModifying: Saving DXCC=" << aux1 << endl;
    }

    aux1 = QString::number(cqz);
    if (aux1.length()>0)
    {
        updateString = updateString + "cqz = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QString::number(ituz);
    if (aux1.length()>0)
    {
        updateString = updateString + "ituz = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QSLTabWidget->getQSLVia();
    //aux1 = qslViaLineEdit->text();
    if (aux1.length()>3)
    {
        updateString = updateString + "qsl_via = '";
        updateString = updateString + aux1 + "', ";
    }

    //aux1 = QString::number(myPowerSpinBox->value());
    aux1 = QString::number(myDataTabWidget->getMyPower());
    if ((aux1.toDouble())>0.0)
    {
        updateString = updateString + "tx_pwr = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = QString::number(rxPowerSpinBox->value());
    if ((aux1.toDouble())>0.0)
    {
        updateString = updateString + "rx_pwr = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = othersTabWidget->getIOTA();
   //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA: " << aux1 << endl;
    if (aux1.length() == 6) // EU-001
    {
       //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA to be saved! " << endl;
        updateString = updateString + "iota = '";
        updateString = updateString + aux1 + "', ";
    }
    else
    {
      //qDebug() << "MainWindow::readDataFromUIDX: Modifyng IOTA NOT to be saved! Lenght="<<QString::number(aux1.length()) << endl;
    }

   aux1 = satTabWidget->getSatName();   //We are assuming that the SAT_NAME is always well provided. If it is blank, then no SAT QSO
   //qDebug() << "MainWindow::readDataFromUIDX: SAT2 modif " << aux1 << endl;
   //updateString = updateString + "sat_name = '";
   //updateString = updateString + aux1 + "', ";

    if (aux1.length()>0)
    {
        updateString = updateString + "sat_name = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = satTabWidget->getSatMode(); // We are assuming that the SAT_MODE is always well provided. If it is blank, then no SAT QSO
    if (aux1.length()>0)
    {
        updateString = updateString + "sat_mode = '";
        updateString = updateString + aux1 + "', ";
    }

    aux1 = othersTabWidget->getPropModeFromComboBox();
    //aux1 = getPropModeFromComboBox();
   //qDebug() << "MainWindow::readDataFromUIDX: PropMode:  " << aux1 << endl;
    if ((aux1.length()>0) && (aux1 != "Not"))
    {
        //qDebug() << "MainWindow::readDataFromUIDX: PropMode(1):  " << aux1 << endl;
        updateString = updateString + "prop_mode = '";
        updateString = updateString + aux1 + "', ";
    }
    else if ((aux1.length()==0) || (aux1 == "Not"))
    {
       //qDebug() << "MainWindow::readDataFromUIDX: PropMode(2):  " << aux1 << endl;
        updateString = updateString + "prop_mode = '',";
        //updateString = updateString + aux1 + "', ";
    }
    else
    {
       //qDebug() << "MainWindow::readDataFromUIDX: PropMode(3):  " << aux1 << endl;
    }

    //CLUBLOG

    aux1 = eQSLTabWidget->getClubLogStatus(); //Y, N, M
    if (aux1 == "Y")
    {
        updateString = updateString + "clublog_qso_upload_status = 'Y', ";
        updateString = updateString + "clublog_qso_upload_date = '" + (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "N")
    {
        updateString = updateString + "clublog_qso_upload_status = 'N', ";
    }
    else if (aux1 == "M")
    {
        updateString = updateString + "clublog_qso_upload_status = 'M', ";
        updateString = updateString + "clublog_qso_upload_date = '" + (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd") + "', ";
    }
    else //TODO: This should be equivalent to N?
    {
        updateString = updateString + "clublog_qso_upload_status = 'N', ";
    }
//CLUBLOG



    // EQSL-SENT
    aux1 = eQSLTabWidget->getEQSLSenStatus();
    if (aux1 == "Y")
    {
        updateString = updateString + "eqsl_qsl_sent = 'Y', ";
        updateString = updateString + "eqsl_qslsdate = '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "eqsl_qsl_sent = 'R', ";
    }
    else if (aux1 == "Q")
    {
        updateString = updateString + "eqsl_qsl_sent = 'Q', ";
        updateString = updateString + "eqsl_qslsdate = '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "eqsl_qsl_sent = 'I', ";
        updateString = updateString + "eqsl_qslsdate = '" + (eQSLTabWidget->getEQSLSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else // N
    {
        updateString = updateString + "eqsl_qsl_sent = 'N', ";
    }


   // EQSL-RECEPTION
    aux1 = eQSLTabWidget->getEQSLRecStatus();
    if (aux1 == "Y")
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'Y', ";
        updateString = updateString + "eqsl_qslrdate = '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'R', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'I', ";
    }
    else if (aux1 == "V")
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'V', ";
        updateString = updateString + "eqsl_qslrdate = '" + (eQSLTabWidget->getEQSLRecDate()).toString("yyyy/MM/dd") + "', ";
    }
    else
    {
        updateString = updateString + "eqsl_qsl_rcvd = 'N', ";
    }


    // LOTW-SENT
    aux1 = eQSLTabWidget->getLOTWSenStatus();
    if (aux1 == "Y")
    {
        updateString = updateString + "lotw_qsl_sent = 'Y', ";
        updateString = updateString + "lotw_qslsdate = '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "lotw_qsl_sent = 'R', ";
    }
    else if (aux1 == "Q")
    {
        updateString = updateString + "lotw_qsl_sent = 'Q', ";
        updateString = updateString + "lotw_qslsdate = '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "lotw_qsl_sent = 'I', ";
        updateString = updateString + "lotw_qslsdate = '" + (eQSLTabWidget->getLOTWSenDate()).toString("yyyy/MM/dd") + "', ";
    }
    else
    {
        updateString = updateString + "lotw_qsl_sent = 'N', ";
    }


    // LOTW-RECEPTION
    //LOTW_QSLRDATE: (only valid if LOTW_RCVD is Y, I, or V)
    aux1 = eQSLTabWidget->getLOTWRecStatus();
    if (aux1 == "Y")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'Y', ";
        updateString = updateString + "lotw_qslrdate = '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "R")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'R', ";
    }
    else if (aux1 == "V")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'V', ";
        updateString = updateString + "lotw_qslrdate = '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "', ";
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "lotw_qsl_rcvd = 'I', ";
        updateString = updateString + "lotw_qslrdate = '" + (eQSLTabWidget->getLOTWRecDate()).toString("yyyy/MM/dd") + "', ";
    }
    else
    {
        updateString = updateString + "lotw_qsl_rcvd = 'N', ";
    }

    // QSL SENT
    //qsAux << tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
    //int i = qslSentComboBox->currentIndex();
    //int ii = qslSentViaComboBox->currentIndex();
    aux1 = QSLTabWidget->getQSLSenStatus();
    aux2 = QSLTabWidget->getSentVia();
   //qDebug() << "MainWindow::readDataFromUIDXModifying: aux1: " << aux1 << " / aux2: " << aux2 << endl;

    if (aux1 == "Y")
    {
        updateString = updateString + "qsl_sent = 'Y', ";
        updateString = updateString + "qslsdate = '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "', ";

        if (aux2 == "D")
        {
            updateString = updateString + "qsl_sent_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_sent_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_sent_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_sent_via = 'B', ";
        }


    }
    else if (aux1 == "R")
    {
        updateString = updateString + "qsl_sent = 'R', ";
        //updateString = updateString + "qslsdate = '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "', ";
        if (aux2 == "D")
        {
            updateString = updateString + "qsl_sent_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_sent_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_sent_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_sent_via = 'B', ";
        }
    }
    else if (aux1 == "Q")
    {
        updateString = updateString + "qsl_sent = 'Q', ";
        updateString = updateString + "qslsdate = '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "', ";
        if (aux2 == "D")
        {
            updateString = updateString + "qsl_sent_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_sent_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_sent_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_sent_via = 'B', ";
        }
    }
    else if (aux1 == "I")
    {
        updateString = updateString + "qsl_sent = 'I', ";
        updateString = updateString + "qslsdate = '" + (QSLTabWidget->getQSLSenDate()).toString("yyyy/MM/dd") + "', ";
        if (aux2 == "D")
        {
            updateString = updateString + "qsl_sent_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_sent_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_sent_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_sent_via = 'B', ";
        }
    }
    else
    {
        updateString = updateString + "qsl_sent = 'N', ";
        updateString = updateString + "qsl_sent_via = 'B', ";
    }


     // QSL RECEPTION
    //i = qslRecComboBox->currentIndex();
    //ii = qslRecViaComboBox->currentIndex();
    aux1 = QSLTabWidget->getQSLRecStatus();
    aux2 = QSLTabWidget->getRecVia();

    if (aux1 == "Y")
    {
        updateString = updateString + "qsl_rcvd = 'Y', ";
        updateString = updateString + "qslrdate = '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "', ";
        //updateString = updateString + "confirmed = '1', ";

        if (aux2 == "D")
        {
            updateString = updateString + "qsl_rcvd_via = 'D', ";
        }
        else if (aux2 == "E")
        {
            updateString = updateString + "qsl_rcvd_via = 'E', ";
        }
        else if (aux2 == "M")
        {
            updateString = updateString + "qsl_rcvd_via = 'M', ";
        }
        else
        {
            updateString = updateString + "qsl_rcvd_via = 'B', ";
        }

    }
    else if (aux1 == "R")
    {
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'R', ";
            //updateString = updateString + "confirmed = '0', ";
            if (aux2 == "D")
            {
                updateString = updateString + "qsl_rcvd_via = 'D', ";
            }
            else if (aux2 == "E")
            {
                updateString = updateString + "qsl_rcvd_via = 'E', ";
            }
            else if (aux2 == "M")
            {
                updateString = updateString + "qsl_rcvd_via = 'M', ";
            }
            else
            {
                updateString = updateString + "qsl_rcvd_via = 'B', ";
            }
    }
    else if (aux1 == "I")
    {
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'I', ";
            updateString = updateString + "qslrdate = '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "', ";
            //updateString = updateString + "confirmed = '0', ";

            if (aux2 == "D")
            {
                updateString = updateString + "qsl_rcvd_via = 'D', ";
            }
            else if (aux2 == "E")
            {
                updateString = updateString + "qsl_rcvd_via = 'E', ";
            }
            else if (aux2 == "M")
            {
                updateString = updateString + "qsl_rcvd_via = 'M', ";
            }
            else
            {
                updateString = updateString + "qsl_rcvd_via = 'B', ";
            }
    }
    else if (aux1 == "V")
    {
        //QSL received date
        //(only valid if QSL_RCVD is Y, I, or V)
            updateString = updateString + "qsl_rcvd = 'V', ";
            updateString = updateString + "qslrdate = '" + (QSLTabWidget->getQSLRecDate()).toString("yyyy/MM/dd") + "', ";
            //updateString = updateString + "confirmed = '1', ";

            if (aux2 == "D")
            {
                updateString = updateString + "qsl_rcvd_via = 'D', ";
            }
            else if (aux2 == "E")
            {
                updateString = updateString + "qsl_rcvd_via = 'E', ";
            }
            else if (aux2 == "M")
            {
                updateString = updateString + "qsl_rcvd_via = 'M', ";
            }
            else
            {
                updateString = updateString + "qsl_rcvd_via = 'B', ";
            }
    }

    else
    {
        updateString = updateString + "qsl_rcvd = 'N', ";
        updateString = updateString + "qsl_rcvd_via = 'B', ";
    }


    keepSatPage = satTabWidget->getRepeatThis();


    // The data reading finish here. Now, we prepare the data to insert into the DB

    if ( updateString.endsWith(", ") )
    {
        updateString.chop(2);
    }

    //stringQuery = "INSERT INTO log (" + stringFields + ") values (" + stringData +")" ;
   // updateString = "UPDATE log SET call = '" + tqrz + "', bandid = '" + QString::number(tband) + "', modeid = '" + QString::number(tmode) + "', qso_date = '" + tdate + "', time_on = '" + ttime + "', lognumber = '" + QString::number(currentLog) + "', " + updateString;

    stringQuery = updateString + " WHERE id = " + "'" + QString::number(modifyingQSO) + "'";
    //qDebug() << "MainWindow::readDataFromUIDXModifying: queryCreated: " << stringQuery << endl;
    return stringQuery;
}

/*
void MainWindow::createSearchResultsPanel()
{

    searchBoxClearButton->setToolTip(tr("Clear the searchs"));
    searchBoxExportButton->setToolTip(tr("Export the search result to an ADIF file"));
    searchBoxSelectAllButton->setToolTip(tr("Select/Unselect all the QSO of the box"));
    searchBoxReSearchButton->setToolTip(tr("Search in the log"));
    searchAllRadioButton->setToolTip(tr("Search in all logs"));

     searchBoxLineEdit->setToolTip(tr("Enter the QRZ to search"));
     searchResultsTreeWidget->setToolTip(tr("Search results"));

     QStringList labels;
     if (stationCallSignShownInSearch)
     {
         labels << tr("QRZ") << tr("Date/Time") << tr("Band") << tr("Mode") << tr("QSL Sent") << tr("QSL Rcvd") << tr("Station Callsign") << tr("Id") ;
         searchResultsTreeWidget->setColumnCount(8);
     }
     else
     {
        labels << tr("QRZ") << tr("Date/Time") << tr("Band") << tr("Mode") << tr("QSL Sent") << tr("QSL Rcvd") << tr("Id") ;
        searchResultsTreeWidget->setColumnCount(7);
     }


     searchResultsTreeWidget->setHeaderLabels(labels);
     //QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
    (searchResultsTreeWidget->header())->resizeSections(QHeaderView::ResizeToContents);


     searchResultsTreeWidget->clear();
     //searchResultsTreeWidget->collapseAll();
     searchResultsTreeWidget->setSortingEnabled(true);
     //searchResultsTreeWidget->setItemsExpandable(false);


    switch (contestMode) {

        case CQ_WW_SSB:
        break;
        case CQ_WW_CW:
        break;
        default:


        break;
    }

}


*/

void MainWindow::createScorePanel()
{


    QVBoxLayout *scoreLayout = new QVBoxLayout;
    scoreLayout->addWidget(scoreTextEdit);
    scoreTextEdit->setPlainText("Test TEXT");

    scoreWindow->setLayout(scoreLayout);

}


void MainWindow::createUICQWW()
{
   //qDebug() << "MainWindow::createUICQWW" << endl;
/*
    QSqlQuery query("SELECT name FROM band");
    while (query.next()) {
        bands << query.value(0).toString();
    }
    QSqlQuery query1("SELECT name FROM mode");
    while (query1.next()) {
        modes << query1.value(0).toString();
    }

*/
    //bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    //modes << "SSB" << "CW" << "RTTY";
    bandComboBox->addItems(bands);
  //qDebug() << "MainWindow::createUICQWW - 1-" << QString::number(modes.count()) << endl;
    modeComboBox->addItems(modes);

    qrzLineEdit->setToolTip(tr("QRZ of the QSO"));
    rstTXLineEdit->setToolTip(tr("TX RST"));
    rstRXLineEdit->setToolTip(tr("RX RST"));
    STXLineEdit->setToolTip(tr("TX Exchange"));
    SRXLineEdit->setToolTip(tr("RX Exchange"));
    bandComboBox->setToolTip(tr("Band of the QSO"));
    modeComboBox->setToolTip(tr("Mode of the QSO"));
    dateEdit->setToolTip(tr("Date of the QSO"));
    timeEdit->setToolTip(tr("Time of the QSO"));
    //statusBar->setToolTip(tr("Misc information"));
    //qsoStatusBar->setToolTip(tr("QSO information"));
    OKButton->setToolTip(tr("Add the QSO to the log"));
    //spotItButton->setToolTip(tr("Spots this QSO to the DX Cluster"));
    clearButton->setToolTip(tr("Clear the box"));

    gridGroupBox = new QGroupBox(tr("Input"));
    QGridLayout *layout = new QGridLayout;

    //slotUpdateStatusBar(tr("Ready"));
    //updateQSOStatusBar(tr("Ready"));

    rstTXLineEdit->setInputMask("#990");
    rstRXLineEdit->setInputMask("#990");
    rstTXLineEdit->setText("59");
    rstRXLineEdit->setText("59");
    rstTXLineEdit->setMaxLength(3);
    rstRXLineEdit->setMaxLength(3);

    QGroupBox *RSTrxgroupBox = new QGroupBox(tr("RSTrx"));
    RSTrxgroupBox->setFlat(true);
    QVBoxLayout *RSTrxvbox = new QVBoxLayout;
    RSTrxvbox->addWidget(rstRXLineEdit);
    RSTrxvbox->addStretch(1);
    RSTrxgroupBox->setLayout(RSTrxvbox);

    QGroupBox *RSTtxgroupBox = new QGroupBox(tr("RSTtx"));
    RSTtxgroupBox->setFlat(true);
    QVBoxLayout *RSTtxvbox = new QVBoxLayout;
    RSTtxvbox->addWidget(rstTXLineEdit);
    RSTtxvbox->addStretch(1);
    RSTtxgroupBox->setLayout(RSTtxvbox);


    //QGroupBox *qrzgroupBox = new QGroupBox(tr("QRZ"));
    qrzgroupBox = new QGroupBox(tr("QRZ"));
    qrzgroupBox->setFlat(true);
    QVBoxLayout *qrzvbox = new QVBoxLayout;
    qrzvbox->addWidget(qrzLineEdit);
    qrzvbox->addStretch(1);
    qrzgroupBox->setLayout(qrzvbox);

    QGroupBox *stxgroupBox = new QGroupBox(tr("STX"));
    stxgroupBox->setFlat(true);
    QVBoxLayout *stxvbox = new QVBoxLayout;
    stxvbox->addWidget(STXLineEdit);
    stxvbox->addStretch(1);
    stxgroupBox->setLayout(stxvbox);

    QGroupBox *srxgroupBox = new QGroupBox(tr("SRX"));
    srxgroupBox->setFlat(true);
    QVBoxLayout *srxvbox = new QVBoxLayout;
    srxvbox->addWidget(SRXLineEdit);
    srxvbox->addStretch(1);
    srxgroupBox->setLayout(srxvbox);

    QHBoxLayout *RSTLayout = new QHBoxLayout;
    RSTLayout->addWidget(RSTtxgroupBox);
    RSTLayout->addWidget(RSTrxgroupBox);
    RSTLayout->addWidget(stxgroupBox);
    RSTLayout->addWidget(srxgroupBox);

    QHBoxLayout *TimeLayout = new QHBoxLayout;
    TimeLayout->addWidget(dateEdit);
    TimeLayout->addWidget(timeEdit);

    QHBoxLayout *BandModeLayout = new QHBoxLayout;
    BandModeLayout->addWidget(bandComboBox);
    BandModeLayout->addWidget(modeComboBox);

    //QHBoxLayout *statusBarLayout = new QHBoxLayout;
    //statusBarLayout->addWidget(statusBar);
    //statusBarLayout->addWidget(qsoStatusBar);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(OKButton);
    //buttonsLayout->addWidget(spotItButton);
    buttonsLayout->addWidget(clearButton);

    QDateTimeEdit *dateEdit = new QDateTimeEdit(QDate::currentDate());
    dateEdit->setDisplayFormat("yyyy/MM/dd");
    timeEdit->setDisplayFormat("HH:mm:ss");

    layout->addWidget(qrzgroupBox, 1, 0);
    layout->addLayout(RSTLayout, 1, 1);
    layout->addLayout(TimeLayout, 2, 0);
    layout->addLayout(BandModeLayout, 2, 1);
    layout->addLayout(buttonsLayout,3, 1);
    //layout->addLayout(statusBarLayout, 4, 0, 2 , -1);

    gridGroupBox->setLayout(layout);
    gridGroupBox->resize(gridGroupBox->minimumSize());
    mainWidget->setLayout(layout);

 }

void MainWindow::slotOKButtonClicked(){
  //qDebug() << "MainWindow::slotOKButtonClicked: "  << endl;
  slotQRZReturnPressed();
}

void MainWindow::createActionsCommon(){
// Functional widgets connections
//TODO: Reimplement the possibility to enter a QSO with enter inthe following widgets:
    //connect(qslViaLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

// Return pressed = QSO ENTRY
    connect(qrzLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(SRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(STXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(rstTXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(rstRXLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(operatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(stationCallSignLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );    

    connect(myLocatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(qthLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    connect(locatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLocatorTextChanged() ) );
    connect(myLocatorLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotMyLocatorTextChanged() ) );

    connect(txFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqTXChanged()) ) ;
    connect(rxFreqSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotFreqRXChanged()) ) ;


//connect(bandComboBox, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
//connect(dateEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
//connect(timeEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

//Actions to pass the focus between QRZ / SRX
    connect(qrzLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQRZTextChanged() ) );
    connect(SRXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSRXTextChanged() ) );
    connect(STXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSTXTextChanged() ) );
    connect(rstTXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotrstTXTextChanged() ) );
    connect(rstRXLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotrstRXTextChanged() ) );

    //connect(qslViaLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQSLViaTextChanged() ) );

    connect(bandComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotBandComboBoxChanged() ) ) ;
    connect(modeComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotModeComboBoxChanged() ) ) ;


//Buttons Actions
    connect(OKButton, SIGNAL(clicked()), this, SLOT(slotOKButtonClicked() ) );
    //connect(spotItButton, SIGNAL(clicked()), this, SLOT(slotSpotItButtonClicked() ) );
    connect(clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked() ) );


// SEARCH BOX VIEW

    //connect(searchBoxLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotSearchBoxTextChanged() ) );

    //connect(searchAllRadioButton, SIGNAL(toggled(bool)), this, SLOT(slotSearchBoxSelectAllButtonClicked() ) );

    //connect(searchResultsTreeWidget, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButtonSearch( const QPoint& ) ) );
    //connect(searchResultsTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotDoubleClickSearch(QTreeWidgetItem *, int)));
    //connect(searchResultsTreeWidget, SIGNAL(itemSelectionChanged( ) ), this, SLOT(slotSearchBoxSelectionChanged( ) ) );

    //connect(searchBoxExportButton, SIGNAL(clicked()), this, SLOT(slotSearchExportButtonClicked() ) );
    //connect(searchBoxClearButton, SIGNAL(clicked()), this, SLOT(slotSearchClearButtonClicked() ) );
    //connect(searchBoxSelectAllButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxSelectAllButtonClicked() ) );
    //connect(searchBoxReSearchButton, SIGNAL(clicked()), this, SLOT(slotSearchBoxReSearchButtonClicked() ) );

    connect(operatingYearsComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotOperatingYearComboBoxChanged() ) ) ;
    connect(recalculateAwardsButton, SIGNAL(clicked()), this, SLOT(slotRecalculateAwardsButtonClicked() ) );

    // LOGVIEW
    connect(logWindow, SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(logWindow, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );
    connect(logWindow, SIGNAL(updateSearchText()), this, SLOT(slotSearchBoxTextChanged() ) ); //When a QSO is deleted

    //CLUSTER
    //void clusterSpotToLog(const QStringList _qs);
    //SIGNAL dxspotclicked(const QStringList _qs)
    connect(dxClusterWidget, SIGNAL(dxspotclicked(QStringList)), this, SLOT(slotAnalyzeDxClusterSignal(QStringList) ) );

    // CLUBLOG
    connect (elogClublog, SIGNAL (showMessage(QString)), this, SLOT (slotElogClubLogShowMessage(QString)));
    connect (elogClublog, SIGNAL (actionReturnDownload(int, int)), this, SLOT (slotElogClubLogProcessAnswer(int, int)));
    connect (elogClublog, SIGNAL (disableClubLogAction(bool)), this, SLOT (slotElogClubLogDisable(bool)));
	// SATELLITE TAB
    connect (satTabWidget, SIGNAL (satBandTXChanged(QString)), this, SLOT (slotSatBandTXComboBoxChanged(QString)));
    // QSL TAB
    connect(QSLTabWidget, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed()) );
    // SEARCH TAB
    connect(searchWidget, SIGNAL(actionQSODoubleClicked ( int ) ), this, SLOT(slotDoubleClickLog( const int ) ) );
    connect(searchWidget, SIGNAL(updateAwards() ), this, SLOT(slotShowAwards() ) );
    connect(searchWidget, SIGNAL(logRefresh() ), this, SLOT(slotLogRefresh() ) );
    connect(searchWidget, SIGNAL(toStatusBar(QString) ), this, SLOT(slotUpdateStatusBar(QString) ) );
    connect(searchWidget, SIGNAL(requestBeingShown() ), this, SLOT(slotShowSearchWidget() ) );
    connect(searchWidget, SIGNAL(actionQSODelete( int ) ), this, SLOT(slotQSODelete(int) ) );
}

void MainWindow::slotSearchBoxTextChanged()
{
    searchWidget->slotSearchBoxTextChanged();
}

void MainWindow::slotQSODelete(const int _id)
{
    elogClublog->deleteQSO(dataProxy->getClubLogRealTimeFromId(_id));
}

void MainWindow::slotShowSearchWidget()
{
    //dxUpRightTab->addTab(searchWidget, tr("Search"));

    dxUpRightTab->setCurrentIndex(dxUpRightTab->indexOf(searchWidget));
}

void MainWindow::slotLogRefresh()
{
    logWindow->refresh();
}

void MainWindow::slotElogClubLogDisable(const bool _b)
{
   //qDebug() << "MainWindow::slotElogClubLogDisable: " << endl;
    if (_b)
    {
        clublogActive = false;
        setupDialog->setClubLogActive(false);

    }
    else
    {
        clublogActive = true;
        setupDialog->setClubLogActive(true);
    }
    //TODO: Disable clublog in the klogrc file
    //bool FileManager::modifySetupFile(const QString& _filename, const QString _field, const QString _value)
    filemanager->modifySetupFile(configFileName, "ClubLogActive", "False");

}

void MainWindow::slotElogClubLogShowMessage(const QString _s)
{
    //qDebug() << "MainWindow::slotElogClubLogShowMessage: " << _s << endl;
    slotUpdateStatusBar(_s);
}

void MainWindow::slotElogClubLogProcessAnswer(const int _i, const int _qID)
{
   //qDebug() << "MainWindow::slotElogClubLogProcessAnswer: " <<QString::number(_i) << endl;

    clublogAnswer = _i;

    if (clublogAnswer == 0) // NO ERROR
    {

        dataProxy->setClubLogSent(_qID, "Y", (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd"));
    }
    else
    {
        dataProxy->setClubLogSent(_qID, "M", (eQSLTabWidget->getClubLogDate()).toString("yyyy/MM/dd"));
    }


}

void MainWindow::slotRecalculateAwardsButtonClicked()
{
    //qDebug() << "MainWindow::recalculateAwardsButtonClicked: " << endl;

    awards->recalculateAwards();
    showAwards();

}

void MainWindow::slotExitFromSlotDialog(const int exitID)
{
    //qDebug() << "MainWindow::slotExitFromSlotDialog: " << QString::number(exitID) << endl;

    if (exitID == 2)
    {
        needToEnd = true;
        close();
    }

}

void MainWindow::createActionsCQWW(){
// Functional widgets connections

}

void MainWindow::createActionsDX(){
// Functional widgets connections
    connect(nameLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(qthLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );
    connect(locatorLineEdit, SIGNAL(returnPressed()), this, SLOT(slotQRZReturnPressed() ) );

    //connect(iotaContinentComboBox, SIGNAL(activated ( int)), this, SLOT(slotIOTAComboBoxChanged() ) )  ;

    //QSL Actions
    //connect(qslSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotQSLSentComboBoxChanged() ) )  ;
    //connect(qslRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotQSLRecvComboBoxChanged() ) ) ;

//TODO REMOVE EQSL
    //connect(eqslSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(sloteQSLSentComboBoxChanged() ) )  ;
    //connect(eqslRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(sloteQSLRecvComboBoxChanged() ) ) ;
    //connect(lotwSentComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotLotwSentComboBoxChanged() ) )  ;
    //connect(lotwRecComboBox, SIGNAL(currentIndexChanged ( int)), this, SLOT(slotLotwRecvComboBoxChanged() ) ) ;

    connect(satTabWidget, SIGNAL(setPropModeSat(QString)), this, SLOT(slotSetPropMode(QString)) ) ;


}

bool MainWindow::checkContest(){
//qDebug() << "MainWindow::checkContest: " << QString::number(contestMode) << endl;
//contestNames << "No-Contest" <<"CQ-WW-DX-SSB" << "CQ-WW-DX-CW" << "CQ-WPX-SSB" << "CQ-WPX-CW";
    QStringList qs;
    qs.clear();
    QString qsoStatus, aux;
    int currentEntity = world->getQRZARRLId(currentQrz);
    int tband = 1 + bandComboBox->currentIndex();

    //int tmode = 1 + modeComboBox->currentIndex();

    switch (contestMode) {

        case CQ_WW_SSB:
            //qDebug() << "MainWindow::checkContest: CQ-WW-SSB:" << QString::number(currentEntity) << "/" << SRXLineEdit->text() << "/" << QString::number(tband) << endl;
            if ( currentEntity < 1) {
                return false;
            }

                //Multiplier: qs << DX-Entity << DXCQz << DX-band;


                qs << QString::number(currentEntity) << SRXLineEdit->text() << QString::number(tband);


                if (contest->isMultiplier(qs)){
                    qrzgroupBox->setTitle(tr("NEW MULT"));
                    //qsoStatus = tr("MULT");
                    aux = " + (M + ";
                    qsoMultiplier = 1;
                }else{
                    //qsoStatus = tr("NO MULT");
                    aux.clear();

                    qsoMultiplier = 0;
                }

                // Points: //_qs << DX-Entity << DX-Continent
                qs.clear();

                qs << QString::number(currentEntity) << QString::number(world->getContinentNumber(currentEntity));

                qsoPoints = contest->getQSOPoints(qs);
                if (aux == " + (M + ")
                {
                    qsoStatus = "Total: " + QString::number(contest->getTotalScore()) + aux + QString::number(qsoPoints) + " points)"; // qsoStatus + " / " + QString::number(qsoPoints) + tr(" points");
                }
                else
                {
                    qsoStatus = "Total: " + QString::number(contest->getTotalScore()) + " ( " + QString::number(qsoPoints) + " points)"; // qsoStatus + " / " + QString::number(qsoPoints) + tr(" points");
                }


                //qDebug() << "MainWindow::checkContest Points: " << QString::number(contest->getQSOPoints(qs)) << endl;
                //qDebug() << "MainWindow::checkContest Continent: " << world->getQRZContinentNumber(qrzLineEdit->text()) << endl;



        break;
        case CQ_WW_CW:
            //qDebug() << "MainWindow::checkContest: CQ-WW-CW:" << endl;
        break;
        default:
            //qDebug() << "MainWindow::checkContest: Default:" << endl;
        break;

    }

    slotUpdateStatusBar(qsoStatus);
    //statusBar()->showMessage(qsoStatus);
    //updateQSOStatusBar(qsoStatus);

    return false;
}
/*
void MainWindow::slotQSLViaTextChanged()
{
    //qDebug() << "MainWindow::slotQSLViaTextChanged: " << qslViaLineEdit->text() << " / Length: " << QString::number((qslViaLineEdit->text()).size()) << endl;
    qslViaLineEdit->setText((qslViaLineEdit->text()).toUpper());
}
*/

bool MainWindow::validCharactersInCall(const QString _qrz)
{
    for (int i = 0; i<_qrz.size();i++)
    {
        if (!( ((_qrz.at(i)).isLetterOrNumber()) || (_qrz.at(i)=='\\')  || (_qrz.at(i)=='/')  ))
        {
            return false;
        }

    }

    return true;
}

void MainWindow::slotQRZTextChanged()
{    
  qDebug()<< "MainWindow::slotQRZTextChanged: " << qrzLineEdit->text() << " / Length: " << QString::number((qrzLineEdit->text()).size()) << "###### START ######" << endl;
  qrzLineEdit->setText((qrzLineEdit->text()).toUpper());
    if (cleaning)
    {
        qDebug()<< "MainWindow::slotQRZTextChanged: Cleaning" << endl;
        return;
    }

    if (qrzAutoChanging)
    {
        qDebug()<< "MainWindow::slotQRZTextChanged: qrzAutoChanging" << endl;
        qrzAutoChanging = false;
        return;
    }

    qrzAutoChanging = true;
    int cursorP = qrzLineEdit->cursorPosition();

    if ( (qrzLineEdit->text()).endsWith(' ') )
    {/*Remove the space and moves the focus to SRX to write the RX exchange*/
        previousQrz = (qrzLineEdit->text()).simplified();
        qrzLineEdit->setText(previousQrz);
        SRXLineEdit->setFocus();
        qDebug()<< "MainWindow::slotQRZTextChanged: Space detected" << endl;
    }

    qDebug()<< "MainWindow::slotQRZTextChanged: Simplifiying & Capitalizing" << endl;
    qrzLineEdit->setText(((qrzLineEdit->text())).simplified());    
    qrzLineEdit->setText((qrzLineEdit->text()).remove(" "));
    //qrzLineEdit->setText((qrzLineEdit->text()).toUpper());

    if (!validCharactersInCall(qrzLineEdit->text()))
    {
        infoLabel1->setText(tr("Invalid characters used in the QRZ"));
        InValidCharsInPrevCall = true;
        return;
    }
    if (((qrzLineEdit->text()).length() < 1))
    { // If QRZ box is blank, Information labels should be cleared.
        infoLabel1->clear();
        infoLabel2->clear();
        slotClearButtonClicked();
        return;
    }

    if ((modify) || ((qrzLineEdit->text()).length() < 1) || (qrzSmallModDontCalculate))
    {
        //qDebug() << "MainWindow::slotQRZTextChanged: MODIFY or Lenght < 1" << endl;
        qrzSmallModDontCalculate=false;
        return;
    }

    qrzSmallModDontCalculate = true; // A kind of flag to prevent multiple calls to this method.
    //int i;
    int dx_CQz = -1;
    int dxE_CQz = -1;
    int dx_ITUz = -1;
    int dxE_ITUz = -1;

    currentQrz = qrzLineEdit->text();

    if ((currentQrz).count('\\')){ // Replaces \ by / to ease operation.
        currentQrz.replace(QChar('\\'), QChar('/'));
        qrzLineEdit->setText(currentQrz);
    }

    currentQrz = qrzLineEdit->text();

    if (cursorP>currentQrz.length())
    {// A Space that has been removed without updating the cursor
        //qDebug()<< "MainWindow::slotQRZTextChanged: cursorP > currentQRZ.length" << endl;
    }
    else
    {
        if ((currentQrz.at(cursorP-1)).isSpace())
        {
            previousQrz = currentQrz.remove(cursorP-1, 1);
            cursorP--;
            qrzLineEdit->setText(previousQrz);
        }
    }

    currentQrz = qrzLineEdit->text();
    currentEntity = world->getQRZARRLId(currentQrz);
    //selectCorrectComboBoxEntity(currentEntity);
   //qDebug() << "MainWindow::slotQRZTextChanged: " << QString::number(currentEntity) << endl;
    othersTabWidget->setEntity(currentEntity);


   //qDebug() << "MainWindow::slotQRZTextChanged: DXCC/ISOname: " << QString::number(currentEntity) << "/" << dataProxy->getISOName(currentEntity) << endl;

   //qDebug() << "MainWindow::slotQRZTextChanged: Entity: " << QString::number(currentEntity) << endl;

    dxE_CQz = world->getEntityCqz(currentEntity);
    dx_CQz = world->getQRZCqz(currentQrz);
    dx_ITUz = world->getQRZItuz(currentQrz);
    dxE_ITUz = world->getEntityItuz(currentEntity);
   //qDebug()<< "MainWindow::slotQRZTextChanged: CQ: " << QString::number(dx_CQz) << endl;
   //qDebug()<< "MainWindow::slotQRZTextChanged: CQe: " << QString::number(dxE_CQz) << endl;
   //qDebug()<< "MainWindow::slotQRZTextChanged: ITU: " << QString::number(dx_ITUz) << endl;
   //qDebug()<< "MainWindow::slotQRZTextChanged: ITUe: " << QString::number(dxE_ITUz) << endl;

    if (dx_CQz == dxE_CQz)
    {
        dx_CQz = dxE_CQz;
    }

    if (dx_ITUz == dxE_ITUz)
    {
        dx_ITUz = dxE_ITUz;
    }


    QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
    _qs.clear();
    _qs << QString::number(currentEntity) << QString::number(currentBand) << QString::number(currentMode) << QString::number(currentLog);

    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        dxLocator = (locatorLineEdit->text()).toUpper();
    }
    else
    {
        dxLocator = world->getLocator(currentEntity);
    }

   // NOW ONLY SPECIFIC ACTIONS DEPENDING ON THE RUNNING MODE

    switch (contestMode) {

        case CQ_WW_SSB:
            //qDebug() << "MainWindow::slotQRZTextChanged: CQ-WW-SSB:" << endl;

            STXLineEdit->setText(QString::number(my_CQz));  // My Own CQZ
            if (dx_CQz > 0)
            {
                //if(QString::number(world->getQRZCqz(qrzLineEdit->text())) > 0 ){
                SRXLineEdit->setText(QString::number(dx_CQz));
            }else{
                //qDebug() << "MainWindow::checkContest  CQZ < 0"<< endl;
            }
            if (currentEntity>0){
                slotUpdateStatusBar(world->getEntityName(currentEntity) + "  -  CQ: " + QString::number(dx_CQz) + "  -  ITU: " + QString::number(dx_ITUz));
            }
            else
            {
                slotUpdateStatusBar(tr("Ready..."));
            }
            checkIfWorkedB4(currentQrz); // Has the QSO id if worked before
            checkContest();
        break;
        case CQ_WW_CW:
            //qDebug() << "MainWindow::slotQRZTextChanged: CQ-WW-CW:" << endl;
        break;
        default:
            //qDebug() << "MainWindow::slotQRZTextChanged: Default:" << endl;
            //qDebug() << "MainWindow::slotQRZTextChanged: - current/previous" << QString::number(currentEntity) << "/" << QString::number(previousEntity) << endl;
        if  ( (currentEntity != previousEntity) || ((infoLabel2->text()).length() < 1) || (InValidCharsInPrevCall) || (dx_CQz != dxE_CQz) || (dx_ITUz != dxE_ITUz))
        {
            //qDebug() << "MainWindow::slotQRZTextChanged: currentEntity=" << QString::number(currentEntity) << "/previousEntity=" << QString::number(previousEntity)  << endl;
                previousEntity = currentEntity;
                InValidCharsInPrevCall = false;
                infoLabel2->setText(world->getEntityName(currentEntity));
                infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
                infoWidget->showDistanceAndBearing(myLocator, dxLocator);



                showStatusOfDXCC(_qs);
                showDXMarathonNeeded(currentEntity, dx_CQz, dateEdit->date().year(), currentLog);
                othersTabWidget->setIOTAContinentFromEntity(currentEntity);
/*
                i = (world->getContinentNumber(currentEntity));
                if (  i > 0 )
                {
                    iotaContinentComboBox->setCurrentIndex( i - 1 );
                }
                else
                {
                    iotaContinentComboBox->setCurrentIndex( 0 );
                }
*/
        }
        else if ((dx_CQz == dxE_CQz) || (dx_ITUz = dxE_ITUz))
        {
            infoLabel2->setText(world->getEntityName(currentEntity));            

            infoWidget->showEntityInfo(currentEntity, dx_CQz, dx_ITUz);
        }
        else
        {
                //qDebug() << "MainWindow::slotQRZTextChanged: Default: else" << endl;
        }

        break;
    }
    qrzSmallModDontCalculate = false; // If the text has not been modified in this method
    qrzLineEdit->setCursorPosition(cursorP);    
    completeWithPreviousQSO(currentQrz);
    qrzAutoChanging = false;
    //qDebug() << "MainWindow::slotQRZTextChanged: END" << endl;
}


/*
void MainWindow::slotSearchBoxReSearchButtonClicked()
{
    slotSearchBoxTextChanged();
}


void MainWindow::slotSearchBoxOnItemChanged( QTreeWidgetItem * item, int column)
{
    //qDebug() << "MainWindow::slotSearchBoxOnItemChanged: " << (item->data (0, Qt::DisplayRole)).toString() << QString::number(column) << endl;
    //searchResultsTreeWidget
    //item->data (0, Qt::DisplayRole)

}
*/

/*
void MainWindow::slotSearchClearButtonClicked()
{
    //qDebug() << "MainWindow::slotSearchClearButtonClicked: " << endl;
    searchResultsTreeWidget->clear();
    searchBoxLineEdit->clear();
    searchSelectAllClicked = false;
    qslingNeeded = false;
}


void MainWindow::slotSearchBoxSelectAllButtonClicked()
{
    //qDebug() << "MainWindow::slotSearchBoxSelectAllButtonClicked: " << endl;
    if (searchSelectAllClicked)
    {
        //qDebug() << "MainWindow::slotSearchBoxSelectAllButtonClicked: UN-SELECTING" << endl;
        searchSelectAllClicked = false;
        searchResultsTreeWidget->clearSelection();
        searchBoxSelectAllButton->setText(tr("&Select All"));


    }
    else
    {
        //qDebug() << "MainWindow::slotSearchBoxSelectAllButtonClicked: SELECTING" << endl;
        searchSelectAllClicked = true;
        searchResultsTreeWidget->selectAll();
        searchBoxSelectAllButton->setText(tr("&Clear selection"));

    }

}


void MainWindow::slotSearchExportButtonClicked()
{
    //qDebug() << "MainWindow::slotSearchExportButtonClicked: " << endl;
    // MARK QSOs
    // SAVE MARKED QSOs TO FILE
    // UNMARK QSOs

    int _qsoId = 0;
    int i = 0;
    QString stringQuery;
    QSqlQuery query;
    QTreeWidgetItem *item = searchResultsTreeWidget->topLevelItem(i);


    while (i <= searchResultsTreeWidget->topLevelItemCount() )
    {
        item = searchResultsTreeWidget->topLevelItem(i);
        if (item == 0)
        {
            i = searchResultsTreeWidget->topLevelItemCount() + 1;
            //qDebug() << "MainWindow::slotSearchExportButtonClicked: ITEM = 0" << endl;
        }
        else
        {
            if (stationCallSignShownInSearch)
            {
                _qsoId = ((item)->text(7)).toInt();
            }
            else
            {
                _qsoId = ((item)->text(6)).toInt();
            }

            if ((item)->isSelected())
            {

                stringQuery = QString("UPDATE log SET marked = 'X' WHERE id='%1'").arg(_qsoId);
                query.exec(stringQuery);
                //TODO: Prepare this while/query execution
                i++;
                //qDebug() << "MainWindow::slotSearchExportButtonClicked: ITEM MARKED: " << QString::number(_qsoId) << endl;

            }
            else
            {

                stringQuery = QString("UPDATE log SET marked = 'N' WHERE id='%1'").arg(_qsoId);
                query.exec(stringQuery);
                //TODO: Prepare this while/query execution
                i++;
                //qDebug() << "MainWindow::slotSearchExportButtonClicked: ITEM NOT MARKED: " << QString::number(_qsoId) << endl;
            }
        }
    }

    //qDebug() << "MainWindow::slotSearchExportButtonClicked: to Ask filename" << endl;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               util->getHomeDir(),
                               tr("ADIF (*.adi *.adif)"));

    //qDebug() << "MainWindow::slotSearchExportButtonClicked: to call save file" << endl;
    filemanager->adifLogExportMarked(fileName);
    //qDebug() << "MainWindow::slotSearchExportButtonClicked: unmarking..." << endl;

}

*/

/*
void MainWindow::slotSearchBoxTextChanged()
{

   //qDebug() << "MainWindow::slotSearchBoxTextChanged: "  << searchBoxLineEdit->text() << endl;
    //int nameCol;
    QString _id, _call, _dateTime, _band, _bandid, _mode, _qsltx, _qslrx, _stationcallsign;
    QStringList q;
    bool searchAll = searchAllRadioButton->isChecked();
    int i = -1;
    int cursorP = searchBoxLineEdit->cursorPosition();

    searchBoxLineEdit->setText((searchBoxLineEdit->text()).toUpper());

    if ((searchBoxLineEdit->text()).length() < 2)
    {
        searchResultsTreeWidget->clear();
        return;
    }
    searchResultsTreeWidget->clear();

    qslingNeeded = false; // If I am searching I am not longer looking for QSO to QSL

    QString theCall = searchBoxLineEdit->text();



    QSqlQuery query;
    QString queryString, aux;

    if (searchAll)
    {
        queryString = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_rcvd, qsl_sent, station_callsign, id FROM log WHERE call LIKE '%%1%'").arg(theCall);
    }
    else
    {
        queryString = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_rcvd, qsl_sent, station_callsign, id FROM log WHERE call LIKE '%%1%' AND lognumber='%2'").arg(theCall).arg(currentLog);
    }




    //qDebug() << "MainWindow::slotSearchBoxTextChanged: queryString"  << queryString << endl;
    query.exec(queryString);

    QSqlRecord rec = query.record();
    int nameCol = -1;



    //qDebug() << "MainWindow::slotSearchBoxTextChanged: queryString EXECUTED!"  << endl;
    QColor color;

    //QSqlRecord rec = query.record();
    QFont font;
    font.setBold(true);

    while (query.next())
    {
        if (query.isValid())
        {


            nameCol = rec.indexOf("call");
            _call= (query.value(nameCol)).toString();
            //nameCol = rec.indexOf("call");
            //_call= (query.value(0)).toString();

            nameCol = rec.indexOf("qso_date");
            _dateTime = (query.value(nameCol)).toString();
            //_dateTime = (query.value(1)).toString();

            nameCol = rec.indexOf("time_on");
            _dateTime = _dateTime + "-" +(query.value(nameCol)).toString();
            //_dateTime = _dateTime + "-" +(query.value(2)).toString();

            nameCol = rec.indexOf("bandid");
            _bandid = (query.value(nameCol)).toString();
            //qDebug() << "MainWindow::slotSearchBoxTextChanged: band: " << QString::number((query.value(3)).toInt()) << endl;
            _band = dataProxy->getNameFromBandId(_bandid.toInt());
            //qDebug() << "MainWindow::slotSearchBoxTextChanged: _band: " << _band << endl;

            nameCol = rec.indexOf("modeid");
            //qDebug() << "MainWindow::slotSearchBoxTextChanged: mode: " << QString::number((query.value(nameCol)).toInt()) << endl;
            //_mode = dataProxy->getNameFromModeId((query.value(4)).toInt());
            _mode = dataProxy->getNameFromSubModeId((query.value(nameCol)).toInt());

            //_mode = query.value(4).toString();

            //qDebug() << "MainWindow::slotSearchBoxTextChanged: mode(b) :  " << _mode << endl;

            //nameCol = rec.indexOf("qsl_sent");
            //_qsltx = (query.value(nameCol)).toString();
            nameCol = rec.indexOf("qsl_rcvd");

            _qsltx = (query.value(nameCol)).toString();
            if (_qsltx.length()<1)
            {
                _qsltx = "N";
            }

            //nameCol = rec.indexOf("qsl_rcvd");
            //_qslrx = (query.value(nameCol)).toString();
            nameCol = rec.indexOf("qsl_sent");
            _qslrx = (query.value(nameCol)).toString();
            if (_qslrx.length()<1)
            {
                _qslrx = "N";
            }
            if (stationCallSignShownInSearch)
            {
                nameCol = rec.indexOf("station_callsign");

                _stationcallsign = (query.value(nameCol)).toString();
                //if (_stationcallsign.length()<3)
                //{
                //    _stationcallsign = stationQRZ;
                //}
                nameCol = rec.indexOf("id");
                _id = (query.value(nameCol)).toString();
            }
            else
            {
                nameCol = rec.indexOf("id");
                _id = (query.value(nameCol)).toString();
            }

                q.clear();
                q << _call << _bandid << _mode << QString::number(currentLog);



                //QColor color = Qt::red;

                //color = Qt::red;
                //TODO: Optimize the awards->getQRZDXStatusColor because is TOO slow
                color =  awards->getQRZDXStatusColor(q);
                //_mode = dataProxy->getSubModeFromId(_mode.toInt());
                //_mode = db->getModeNameFromNumber(_mode.toInt());

                QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
                i = world->getQRZARRLId(_call);
                aux = world->getEntityName(i) + " - CQ: " + QString::number(world->getEntityCqz(i));
                item->setToolTip(0, aux);
                item->setToolTip(1, aux);
                item->setToolTip(2, aux);
                item->setToolTip(3, aux);
                item->setToolTip(4, aux);
                item->setToolTip(5, aux);
                item->setToolTip(6, aux);

                //item->setToolTip(0, world->getQRZEntityName(_call));
                item->setText(0, _call);
                item->setFont(0, font);
                item->setText(1, _dateTime);
                item->setText(2, _band);
               //qDebug() << "MainWindow::slotSearchBoxTextChanged: mode(c) :  " << _mode << endl;
                item->setText(3, _mode);
                item->setText(4, _qslrx);
                item->setText(5, _qsltx);
                if (stationCallSignShownInSearch)
                {
                    item->setText(6, _stationcallsign);
                    item->setText(7, _id);
                    item->setToolTip(7, aux);

                }
                else
                {
                    item->setText(6, _id);
                }


            item->setForeground(0, QBrush(color));
        } // Closes if next.isValid
    } // Closes While
    (searchResultsTreeWidget->header())->resizeSections(QHeaderView::ResizeToContents);


    searchBoxLineEdit->setCursorPosition(cursorP);

}
*/


/*
 * void MainWindow::slotSearchBoxSelectionChanged()
{// Detects when a selection has been done in the search box and changes
 // The button to clear selection
    //qDebug() << "MainWindow::slotSearchBoxSelectionChanged: "  << endl;

    if ((searchResultsTreeWidget->selectedItems()).size() > 0 )
    {
        searchBoxSelectAllButton->setText(tr("&Clear selection"));
        searchSelectAllClicked = true;
    }
    else
    {
        searchBoxSelectAllButton->setText(tr("&Select All"));
        searchSelectAllClicked = false;
    }
}

*/
void MainWindow::slotQRZSpacePressed()
{
     //qDebug() << "MainWindow::slotQRZSpacePressed: "  << endl;
}

void MainWindow::slotSRXTextChanged()
{
    //qDebug() << "MainWindow::slotSRXTextChanged: " << SRXLineEdit->text()  << endl;

    srx = SRXLineEdit->text();
    //int i = srx.size();

    if ( srx.endsWith(' ')  )
    {
        /*Remove the space and moves the focus to QRZ*/

        srx = srx.simplified();
        SRXLineEdit->setText(srx);
        qrzLineEdit->setFocus(Qt::OtherFocusReason);

    }

}

void MainWindow::slotSTXTextChanged()
{
    //qDebug() << "MainWindow::slotSTXTextChanged: " << STXLineEdit->text()  << endl;

    stx = STXLineEdit->text();
    //int i = stx.size();

    if ( stx.endsWith(' ')  )
    {
        /*Remove the space and moves the focus to QRZ*/

        stx = stx.simplified();
        STXLineEdit->setText(stx);
        SRXLineEdit->setFocus(Qt::OtherFocusReason);


    }
}

void MainWindow::slotrstTXTextChanged()
{

}

void MainWindow::slotrstRXTextChanged()
{
}

void MainWindow::slotSpotItButtonClicked()
{
    if (!dxClusterWidget->isConnected())
    {
        return;
    }

}

void MainWindow::slotClearButtonClicked()
{
   //qDebug()() << "MainWindow::slotClearButtonClicked" << endl;
    cleaning = true;
    modify = false;
    OKButton->setText(tr("&Add"));
    modifyingQSO = -1;
    qrzLineEdit->clear();
    qrzLineEdit->setFocus(Qt::OtherFocusReason);

    rstTXLineEdit->setText("59");
    rstRXLineEdit->setText("59");
    qthLineEdit->clear();

   //qDebug() << "MainWindow::slotClearButtonClicked: - band: " << QString::number(currentBand) << endl;
   //qDebug() << "MainWindow::slotClearButtonClicked: - mode: " << QString::number(currentMode) << endl;
    bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(currentBand)));
   //qDebug() << "MainWindow::MainWindow: 12 - currentMode: " << QString::number(currentMode) << endl;
    modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(currentMode)));

    qsoPoints = 0;
    qsoMultiplier = 0;
    clublogAnswer = -1;
    clublogPrevQSO.clear();


    switch (contestMode) {

        case CQ_WW_SSB:
            SRXLineEdit->clear();
            STXLineEdit->setText( QString::number( world->getQRZCqz(stationQRZ) ) );

            qrzgroupBox->setTitle(tr("QRZ"));

        break;
        case CQ_WW_CW:
            SRXLineEdit->clear();
            STXLineEdit->setText( QString::number( world->getQRZCqz(stationQRZ) ) );
            qrzgroupBox->setTitle(tr("QRZ"));

        break;
        default:
            SRXLineEdit->setText("59");
            STXLineEdit->setText("59");

            nameLineEdit->clear();
            locatorLineEdit->clear();
            txFreqSpinBox->setValue(0);
            rxFreqSpinBox->setValue(0);
            //freqQLCDNumber->display(0);
            //notesTextEdit->clear();
            commentTabWidget->clear();
            //commentLineEdit->clear();
            infoLabel1->clear();
            infoLabel2->clear();

            rxPowerSpinBox->setValue(0);

            //eQSL

            //clublogComboBox->setCurrentIndex(1);
            //qslSentViaComboBox->setCurrentIndex(0); // has to be changed before the qslSentComboBox to avoid calling the slot
            //qslRecViaComboBox->setCurrentIndex(0); // has to be changed before the qslRecComboBox to avoid calling the slot

            //qslSentComboBox->setCurrentIndex(0);    // has to be changed before the qslSentComboBox to avoid calling the slot
            //qslRecComboBox->setCurrentIndex(1); // Not received
            //qslSentComboBox->setCurrentIndex(1); // Not sent
            //eqslSentComboBox->setCurrentIndex(1);
            //eqslRecComboBox->setCurrentIndex(1);
            //lotwSentComboBox->setCurrentIndex(1);
            //lotwRecComboBox->setCurrentIndex(1);

            //qslmsgTextEdit->clear();
            //qslViaLineEdit->clear();

            eQSLTabWidget->clear();
            QSLTabWidget->clear();
            othersTabWidget->clear();
            //entityNameComboBox->setCurrentIndex(0);
            //propModeComboBox->setCurrentIndex(0);
            //iotaContinentComboBox->setCurrentIndex(0);
            //iotaNumberLineEdit->setText("000");
            //continentLabel->setText("");
            //prefixLabel->setText("");
            //cqzLabel->setText("0");
            //ituzLabel->setText("0");
/*
            if (!keepMyData)
            {
                //myPowerSpinBox->setValue(lastPower);
                operatorLineEdit->setText(lastOperatorQRZ.toUpper());
                stationCallSignLineEdit->setText(lastStationQRZ.toUpper());
                myLocatorLineEdit->setText(lastMyLocator);
            }
            else
            {
                myPowerSpinBox->setValue(myPower);
                operatorLineEdit->setText(operatorQRZ.toUpper());
                stationCallSignLineEdit->setText(stationQRZ.toUpper());
                myLocatorLineEdit->setText(myLocator);
            }

*/
            satTabWidget->clear();
            myDataTabWidget->clear(keepMyData);
            
            infoWidget->clear();
            //infoWidget->clearInfoFromLocators();
            //infoWidget->clearBandLabels();
        break;
    }
    statusBar()->clearMessage();
    cleaning = false;
}

/*
void MainWindow::clearBandLabels()
{
    bandLabel1->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel2->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel3->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel4->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel5->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel6->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel7->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel8->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel9->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel10->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel11->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
    bandLabel12->setStyleSheet("* { background-color: " + defaultColor.name() + "; }");
}
*/

void MainWindow::slotUpdateTime()
{
   // //qDebug() << "MainWindow::slotUpdateTime: " << (dateTime->currentDateTime()).toString("yyyy-MM-dd - hh:mm:ss") << endl;
//    ((dateTime->currentDateTimeUtc()).date()).toString()
    //(dateTime->currentDateTime()).date()
    dateTime->currentDateTime();

    if ( (!modify) && (realTime)  )
    {

        //dateTime->currentDateTime();

        dateEdit->setDate((dateTime->currentDateTime()).date());


        if (UTCTime)
        {
            timeEdit->setTime((dateTime->currentDateTime().toUTC()).time());
        }
        else
        {
            timeEdit->setTime((dateTime->currentDateTime()).time());
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //qDebug() << "MainWindow::closeEvent" << endl;

    if (maybeSave())
    {
        db->unMarkAllQSO();
        //slotFileClose();
        dataProxy->compressDB();
        //db->compress();
        event->accept();
    }
    else
    {
        event->ignore();
    }


}

bool MainWindow::maybeSave()
{
    //qDebug() << "MainWindow::maybeSave" << endl;

    if ((alwaysADIF) || (DBinMemory) )
    {
        if (needToSave)
        {
            QMessageBox::StandardButton ret;
            ret = QMessageBox::warning(this, tr("KLog"),
                     tr("The logfile has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            if (ret == QMessageBox::Save)
            {
                if (useDefaultLogFileName)
                {
                    return saveFile(defaultADIFLogFile);
                }
                else
                {
                    return saveFileAs();
                }
            }

         else if (ret == QMessageBox::Cancel)
             return false;
        }
    }
    return true;
}



void MainWindow::createMenusCommon()
{
    //qDebug() << "MainWindow::createMenusCommon" << endl;
    fileMenu = menuBar()->addMenu(tr("&File"));

    openAct = new QAction(tr("&New..."), this);
    fileMenu->addAction(openAct);
    //openAct->setShortcut(Qt::CTRL + Qt::Key_N);
    connect(openAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    fileMenu->addAction(openAct);
    openAct->setShortcut(Qt::CTRL + Qt::Key_O);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    ADIFImport = new QAction(tr("&Import from ADIF..."), this);
    fileMenu->addAction(ADIFImport);
    //ADIFImport->setMenuRole(QAction::ApplicationSpecificRole);
    connect(ADIFImport, SIGNAL(triggered()), this, SLOT(slotADIFImport()));
    ADIFImport->setToolTip(tr("Import an ADIF file into the current log"));

    fileMenu->addSeparator();

    saveAct = new QAction(tr("&Save As..."), this);
    fileMenu->addAction(saveAct);
    saveAct->setShortcut(Qt::CTRL + Qt::Key_S);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    fileMenu->addSeparator();

    ADIFExport = new QAction(tr("&Export to ADIF..."), this);
    fileMenu->addAction(ADIFExport);
    //ADIFExport->setMenuRole(QAction::ApplicationSpecificRole);
    connect(ADIFExport, SIGNAL(triggered()), this, SLOT(slotADIFExport()));
    ADIFExport->setToolTip(tr("Export the current log to an ADIF logfile"));

    ADIFExportAll = new QAction(tr("&Export all logs to ADIF..."), this);
    fileMenu->addAction(ADIFExportAll);
    //ADIFExport->setMenuRole(QAction::ApplicationSpecificRole);
    connect(ADIFExportAll, SIGNAL(triggered()), this, SLOT(slotADIFExportAll()));
    ADIFExportAll->setToolTip(tr("Export ALL the QSOs into one ADIF file, merging QSOs from all the logs."));

    ReqQSLExport = new QAction(tr("&Export Requested QSL to ADIF..."), this);
    fileMenu->addAction(ReqQSLExport);
    connect(ReqQSLExport, SIGNAL(triggered()), this, SLOT(slotRQSLExport()));
    ReqQSLExport->setToolTip(tr("Export all QSOs requesting QSLs to an ADIF file (e.g. to import it into a QSL tag printing program"));

    fileMenu->addSeparator();

    printLogAct = new QAction(tr("&Print Log..."), this);
    fileMenu->addAction(printLogAct);
    printLogAct->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(printLogAct, SIGNAL(triggered()), this, SLOT(slotFilePrint()));

    fileMenu->addSeparator();

    klogFolderAct = new QAction(tr("KLog folder"), this);
    fileMenu->addAction(klogFolderAct);
    connect(klogFolderAct, SIGNAL(triggered()), this, SLOT(slotOpenKLogFolder()));

    fileMenu->addSeparator();

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    exitAct->setMenuRole(QAction::QuitRole);
    exitAct->setShortcut(Qt::CTRL + Qt::Key_X);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    fillQsoAct = new QAction(tr("Fill in QSO data"), this);
    toolMenu->addAction(fillQsoAct);
    //fillQsoAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(fillQsoAct, SIGNAL(triggered()), this, SLOT(fillQSOData()));
    fillQsoAct->setToolTip(tr("Go through the log reusing previous QSOs to fill missing information in other QSOs"));

    toolMenu->addSeparator();

    findQSO2QSLAct = new QAction(tr("&Find QSO to QSL"), this);
    toolMenu->addAction(findQSO2QSLAct);
    //findQSO2QSLAct->setMenuRole(QAction::ApplicationSpecificRole);

    connect(findQSO2QSLAct, SIGNAL(triggered()), this, SLOT(slotSearchToolNeededQSLToSend()));
    findQSO2QSLAct->setToolTip(tr("Shows QSOs for which you should send your QSL and request the DX QSL"));


    findRequestedQSLAct = new QAction(tr("Find My-QSLs pending to send"), this);
    toolMenu->addAction(findRequestedQSLAct);
    //findQSO2QSLAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(findRequestedQSLAct, SIGNAL(triggered()), this, SLOT(slotToolSearchRequestedQSLToSend()));
    findRequestedQSLAct->setToolTip(tr("Shows the QSOs with pending requests to send QSLs. You should keep this queue empty!"));

    findQSLPendingToReceiveAct = new QAction(tr("&Find DX-QSLs pending to receive"), this);
    toolMenu->addAction(findQSLPendingToReceiveAct);
    connect(findQSLPendingToReceiveAct, SIGNAL(triggered()), this, SLOT(slotToolSearchNeededQSLPendingToReceive()));
    findQSLPendingToReceiveAct->setToolTip(tr("Shows the DX-QSL that has been requested or QSLs has been sent with no answer"));

    findQSLDXRequestedAct = new QAction(tr("&Find requested pending to receive"), this);
    toolMenu->addAction(findQSLDXRequestedAct);
    connect(findQSLDXRequestedAct, SIGNAL(triggered()), this, SLOT(slotToolSearchNeededQSLRequested()));
    findQSLDXRequestedAct->setToolTip(tr("Shows the DX-QSL that has been requested"));

    toolMenu->addSeparator();

    downloadCTYAct = new QAction (tr("&Update CTY.CSV"), this);
    toolMenu->addAction(downloadCTYAct);
    //downloadCTYAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(downloadCTYAct, SIGNAL(triggered()), this, SLOT(slotUpdateCTYDAT()));
    downloadCTYAct->setToolTip(tr("For updated DX-Entity data, update cty.csv"));

    toolMenu->addSeparator();

    setupMenu = menuBar()->addMenu(tr("&Setup"));

    setupAct = new QAction(tr("&Setup..."), this);
    setupMenu->addAction(setupAct);
    setupAct->setMenuRole(QAction::PreferencesRole);
    connect(setupAct, SIGNAL(triggered()), this, SLOT(slotSetup()));

    //TODO: To be added once the help dialog has been implemented
    helpMenu = menuBar()->addMenu(tr("&Help"));

    updateAct = new QAction(tr("Check updates..."), this);
    helpMenu->addAction(updateAct);
    updateAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(updateAct, SIGNAL(triggered()), this, SLOT(slotHelpCheckUpdatesAction()));

    aboutAct = new QAction(tr("&About..."), this);
    helpMenu->addAction(aboutAct);
    aboutAct->setMenuRole(QAction::AboutRole);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(slotHelpAboutAction()));
    
    aboutQtAct = new QAction(tr("About Qt..."), this);
    helpMenu->addAction(aboutQtAct);
    aboutQtAct->setMenuRole(QAction::AboutQtRole);
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(slotAboutQt()));
    
    
 }

void MainWindow::slotSearchToolNeededQSLToSend()
{
    searchWidget->searchToolNeededQSLToSend();
}

void MainWindow::slotToolSearchRequestedQSLToSend()
{
    searchWidget->slotToolSearchRequestedQSLToSend();
}

void MainWindow::slotToolSearchNeededQSLPendingToReceive()
{
    searchWidget->slotToolSearchNeededQSLPendingToReceive();
}

void MainWindow::slotToolSearchNeededQSLRequested()
{
    searchWidget->slotToolSearchNeededQSLRequested();
}

void MainWindow::slotAboutQt()
{
    //qDebug() << "MainWindow::slotAboutQt"  << endl;
   QMessageBox::aboutQt(this,tr("About..."));
   
}
/*
void MainWindow::slotHelpHelpAction()
{
    //qDebug() << "MainWindow::slotHelpHelpAction"  << endl;

    //helpHelpDialog->exec();
    //aboutDialog->exec();

}
*/

void MainWindow::slotHelpAboutAction()
{
    //qDebug() << "MainWindow::slotHelpAboutAction "  << endl;
   // QMessageBox::about(this, tr("About KLog"),
    //            tr("<b>KLog</b> "
   //                "Find the last release at http://jaime.robles.es/klog."));


    aboutDialog->exec();
    //helpAboutDialog->exec();
}

void MainWindow::slotHelpCheckUpdatesAction()
{
   //qDebug() << "MainWindow::slotHelpCheckUpdatesAction" << endl;
    callingUpdate = true;
    softUpdate->addCall(stationQRZ);
    softUpdate->needToUpdate(true);
    //callingUpdate = false;
}

void MainWindow::slotShowSoftUpdateResults(const bool _b)
{

   //qDebug() << "MainWindow::slotShowSoftUpdateResults: " << endl;
    if (_b == true)
    {
   //qDebug() << "MainWindow::slotShowSoftUpdateResults _b = TRUE " << endl;
    }
    else
    {
       //qDebug() << "MainWindow::slotShowSoftUpdateResults _b = FALSE " << endl;

    }

    if (callingUpdate == true)
    {
        if (_b == false)
        {
           //qDebug() << "MainWindow::slotShowSoftUpdateResults: UPDATE NOT NEEDED" << endl;
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setWindowTitle(tr("KLog update checking result"));
            msgBox.setText(tr("Congratulations!\n\nYou already have the latest version."));
            msgBox.exec();
        }
        else
        {
           //qDebug() << "MainWindow::slotShowSoftUpdateResults: UPDATE NEEDED" << endl;
        }
    }
    callingUpdate = false;
}


void MainWindow::createMenusCQWW()
{
    //qDebug() << "MainWindow::createMenusCQWW" << endl;
/*
    logWinAct = new QAction(tr("&Log Window"), this);
    logWinAct->setCheckable(true);
    logWinAct->setShortcut(Qt::CTRL + Qt::Key_L);
    viewMenu->addAction(logWinAct);
    connect(logWinAct, SIGNAL(triggered()), this, SLOT(slotLogWinShow()));

    scoreWinAct = new QAction(tr("&Points Window"), this);
    scoreWinAct->setCheckable(true);
    scoreWinAct->setShortcut(Qt::CTRL + Qt::Key_P);
    viewMenu->addAction(scoreWinAct);
    connect(scoreWinAct, SIGNAL(triggered()), this, SLOT(slotScoreWinShow()));



    CabrilloExport = new QAction(tr("&Export to Cabrillo..."), this);
    toolMenu->addAction(CabrilloExport);
    connect(CabrilloExport, SIGNAL(triggered()), this, SLOT(slotCabrilloExport()));
    */

 }



void MainWindow::slotLogWinShow()
{
    //qDebug() << "MainWindow::slotLogWinShow: "  << endl;

    if (!(logWindow->isVisible()) )
    {
        logWinAct->setChecked ( true );
        logWindow->show();
    }else
    {
        logWinAct->setChecked ( false );
        logWindow->hide();
    }
}

void MainWindow::slotScoreWinShow()
{
    //qDebug() << "MainWindow::slotScoreWinShow: "  << endl;

    if (!(scoreWindow->isVisible()) )
    {
        scoreWinAct->setChecked ( true );
        scoreWindow->show();

    }else
    {
        scoreWinAct->setChecked ( false );
        scoreWindow->hide();
    }
}

void MainWindow::slotSetup(const int _page)
{
   //qDebug() << "MainWindow::slotSetup - 01"  << endl;

    if (!needToEnd)
    {
        setupDialog->setData(configFileName, softwareVersion, _page, !configured);
        setupDialog->exec();

        if (needToEnd)
        {
            return;
        }
        else
        {
            readConfigData();
        }


       //qDebug() << "MainWindow::MainWindow: logmodel to be created-2" << endl;
        logWindow->createlogPanel(currentLog);

    }
    defineStationCallsign();

    db->reConnect();

}

void MainWindow::openFile()
{

    int lastLog = currentLog;

    slotSetup(6);

    if (lastLog == currentLog)
    { // It seems that the user didn't really want a new log
        return;
    }

    logWindow->refresh();

}

bool MainWindow::saveFile(const QString _fileName)
{
  //qDebug() << "MainWindow::saveFile: " << _fileName  << endl;


    QString fileName = _fileName;

    if (fileName.endsWith(".adi", Qt::CaseInsensitive))
    {
        //qDebug() << "MainWindow::saveFile: 1"  << endl;
        needToSave = !(filemanager->adifLogExport(fileName, currentLog));
    }
    else if (fileName.endsWith(".log", Qt::CaseInsensitive))
    {
        //qDebug() << "MainWindow::saveFile: 2"  << endl;
        needToSave = !(filemanager->cabrilloLogExport(fileName, contestMode, currentLog));
        //contest->saveFileToSend(fileName);
    }
    else
    {
        //qDebug() << "MainWindow::saveFile: 3"  << endl;
        //TODO: Message "You must select a proper file format
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Information);
       msgBox.setText(tr("Nothing has been saved. You have to select a valid file type."));
       msgBox.exec();
       return false;
    }
    //qDebug() << "MainWindow::saveFile: 4"  << endl;
    return needToSave;

}

bool MainWindow::saveFileAs()
{
   //qDebug() << "MainWindow::saveFileAs"  << endl;
    //QFileDialog dialog(this);

    QStringList filters;
    filters << "ADIF files (*.adi *.adif)"
            << "Cabrillo files (*.log)"
            << "Any files (*)";

//    dialog.setNameFilters(filters);

/*
 QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                            "/home/jana/untitled.png",
                            tr("Images (*.png *.xpm *.jpg)"));
*/

   // klogDir+"/"+defaultADIFLogFile,
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                   util->getHomeDir(),
                        tr("ADIF files (*.adi *.adif);;Cabrillo files (*.log);;Any file (*.*)"));

    if ( (fileName.endsWith(".adi", Qt::CaseInsensitive)) || (fileName.endsWith(".log", Qt::CaseInsensitive)) )
    {
        useDefaultLogFileName = true;
        defaultADIFLogFile = fileName;

        return saveFile(fileName);
    }
    else if (fileName.length()==0)
    {
        // The user clicked on cancel, no msg to be shown
        return false;
    }
    else
    {

        //TODO: Message "You must select a proper file format
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Information);
       msgBox.setText(tr("Nothing has been saved. You have to select a valid file type."));
       msgBox.exec();
       return false;
    }

    return false;

}


void MainWindow::newFile()
{
     //qDebug() << "MainWindow::newFile"  << endl;
     //TODO: Ask for a confirmation to the user
    //TODO: Clean the DB & query.exec("VACUUM");

    int lastLog = currentLog;

    slotSetup(6);

    if (lastLog == currentLog)
    { // It seems that the user didn't really want a new log
        return;
    }

    points = 0;
    multipliers = 0;
    qsoPoints = 0;
    qsoMultiplier = 0;
    logWindow->refresh();
    slotClearButtonClicked();
    searchWidget->clear();
    //searchResultsTreeWidget->clear();


/*
    if (dataProxy->clearLog())
    {



    }
    else
    {
        //TODO: An error to create a new file has ocurred. Manage it!
    }
*/

}

bool MainWindow::slotOpenKLogFolder()
{
   //qDebug() << "MainWindow::slotOpenKLogFolder: " << configFileName << endl;

    //configFileName = klogDir+"/klogrc.cfg";
    QString _aux = "<a href=file://" + util->getHomeDir() + ">file://" + util->getHomeDir() + "</a>";
    QString _text = tr("You can find the KLog data folder here: ") + _aux;
/*
    int ret = QMessageBox::information(this, tr("KLog"),
                                   _text,
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);
*/
    QMessageBox::information(this, tr("KLog"),
                                   _text,
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);
    return true;

}


void MainWindow::slotUpdateStatusBar(const QString statusm)
{
    //qDebug() << "MainWindow::slotUpdateStatusBar: " << statusm  << endl;
    statusBar()->showMessage(statusm, 2000);
}




bool MainWindow::readCtyFile()
{
    return false;
}

void MainWindow::slotDoubleClickLog(const int _qsoID)
{
   //qDebug() << "MainWindow::slotDoubleClickLog: QSOid: " << QString::number(_qsoID) << endl;

    //int row = _qsoID.row();
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());
    qsoToEdit(_qsoID);

    //TODO: To be added to the logWindow and create an action that emist the QSO id to be edited
}

/*
void MainWindow::slotDoubleClickSearch(QTreeWidgetItem * item, int)
{
    //qDebug() << "MainWindow::slotDoubleClickSearch"  << endl;
    int number = -1;
    if (item){
        if (stationCallSignShownInSearch)
        {
            number = (item->text(7)).toInt();
        }
        else
        {
            number = (item->text(6)).toInt();
        }


        qsoToEdit(number);
    }
    else
    {}
}

*/
/*
void MainWindow::slotRighButtonSearch(const QPoint& pos)
{
    //qDebug() << "MainWindow::slotRighButtonSearch"  << endl;


    QTreeWidgetItem *item = searchResultsTreeWidget->itemAt(pos);
    int _qsoID = 0;

    if (item)
    {
        //qDebug() << "MainWindow::slotRighButtonSearch ITEM=true"  << endl;
        // 6 is the column in the searchResultsTreeWidget where the id is saved
        if (stationCallSignShownInSearch)
        {
            //qDebug() << "MainWindow::slotRighButtonSearch stationCallSignShownInSearch = true"  << endl;
            _qsoID = ((item)->text(7)).toInt();
            //qDebug() << "MainWindow::slotRighButtonSearch QSO1: " << QString::number(_qsoID)  << endl;
        }
        else
        {
            //qDebug() << "MainWindow::slotRighButtonSearch stationCallSignShownInSearch = false"  << endl;
            _qsoID = ((item)->text(6)).toInt();
            //qDebug() << "MainWindow::slotRighButtonSearch QSO2: " << QString::number(_qsoID)  << endl;
        }
        //qDebug() << "MainWindow::slotRighButtonSearch QSO: " << QString::number(_qsoID)  << endl;
        showMenuRightButtonSearchCreateActions();
        //qDebug() << "MainWindow::slotRighButtonSearch -05"   << endl;
        righButtonSearchMenu(_qsoID);
        //qDebug() << "MainWindow::slotRighButtonSearch -06"   << endl;
    }else
    {
        //qDebug() << "MainWindow::slotRighButtonSearch ITEM=false"  << endl;
        return;
    }

    //qDebug() << "MainWindow::slotRighButtonSearch: "  << QString::number(_qsoID) << endl;

}

*/

/*
void MainWindow::righButtonSearchMenu(const int trow)
{
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu:  " << QString::number(trow) << endl;


    bool qslReceived = logWindow->isQSLReceived(trow);
    bool qslSent = logWindow->isQSLSent(trow);

    QMenu menu(this);

    menu.addAction(delQSOFromSearchAct);
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -03"  << endl;
    delQSOFromSearchAct->setData(trow);
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -04"  << endl;

    menu.addAction(qsoToEditFromSearchAct);
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -05"  << endl;
    qsoToEditFromSearchAct->setData(trow);
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -06"  << endl;

    menu.addSeparator();
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -07"  << endl;
    if (qslSent)
    {
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSLSent"  << endl;
    }
    else
    {
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Sent"  << endl;
        QMenu *menuSentQsl = menu.addMenu(tr("QSL Send"));
        menuSentQsl->addAction(qslSentViaBureauFromSearchAct);
        menuSentQsl->addAction(qslSentViaDirectFromSearchAct);
        menuSentQsl->addAction(qslSentRequestedAct);
        if (!qslReceived)
        {
            menuSentQsl->addAction(qslSentViaBureauMarkRcvReqFromSearchAct);
            menuSentQsl->addAction(qslSentViaDirectMarkRcvReqFromSearchAct);
            qslSentViaBureauMarkRcvReqFromSearchAct->setData(trow);
            qslSentViaDirectMarkRcvReqFromSearchAct->setData(trow);
        }
        qslSentViaBureauFromSearchAct->setData(trow);
        qslSentViaDirectFromSearchAct->setData(trow);
        qslSentRequestedAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -end qsl not sent"  << endl;

    }

    if (qslReceived)
    {
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSLRec"  << endl;
    }
    else
    {
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec"  << endl;
        QMenu *menuRecQsl = menu.addMenu(tr("QSL Rcvd"));
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 01"  << endl;
        menuRecQsl->addAction(qslRecViaBureauFromSearchAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 02"  << endl;
        menuRecQsl->addAction(qslRecViaBureauMarkReqFromSearchAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 03"  << endl;
        menuRecQsl->addAction(qslRecViaDirectFromSearchAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 04"  << endl;
        menuRecQsl->addAction(qslRecViaDirectMarkReqFromSearchAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 05"  << endl;
        menuRecQsl->addAction(qslRecRequestedAct);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 06"  << endl;
        
        qslRecViaBureauFromSearchAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 07"  << endl;
        qslRecViaBureauMarkReqFromSearchAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 08"  << endl;
        qslRecViaDirectFromSearchAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 09"  << endl;
        qslRecViaDirectMarkReqFromSearchAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -QSL Not Rec - 10"  << endl;
        qslRecRequestedAct->setData(trow);
        //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -end qsl not rec"  << endl;
    }
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -end qsl"  << endl;
    menu.exec(QCursor::pos());
    //qDebug() << "MainWindow::slotshowRighButtonSearchMenu: -END"  << endl;
}



void MainWindow::showMenuRightButtonSearchCreateActions()
{
 //qDebug() << "MainWindow::showMenuRightButtonSearchCreateActions" << endl;

    delQSOFromSearchAct = new QAction(tr("&Delete"), this);
    delQSOFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_D);
    delQSOFromSearchAct->setStatusTip(tr("Delete a QSO"));
    connect(delQSOFromSearchAct, SIGNAL(triggered()), this, SLOT(slotQsoDeleteFromSearch()));

    qsoToEditFromSearchAct = new QAction(tr("&Edit QSO"), this);
    qsoToEditFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_E);
    qsoToEditFromSearchAct->setStatusTip(tr("Edit this QSO"));
    connect(qsoToEditFromSearchAct, SIGNAL(triggered()), this, SLOT(slotQSOToEditFromSearch()));

    qslSentViaBureauFromSearchAct = new QAction(tr("Via &bureau"), this);
    qslSentViaBureauFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_B);
    qslSentViaBureauFromSearchAct->setStatusTip(tr("Send this QSL via bureau"));
    connect(qslSentViaBureauFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureauFromSearch() ));

    qslSentViaDirectFromSearchAct = new QAction(tr("D&irect"), this);
    qslSentViaDirectFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_I);
    qslSentViaDirectFromSearchAct->setStatusTip(tr("Send this QSL via direct"));
    connect(qslSentViaDirectFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectFromSearch()   ));


    qslSentRequestedAct = new QAction(tr("&Request my QSL"), this);
    qslSentRequestedAct->setShortcut(Qt::CTRL + Qt::Key_R);
    qslSentRequestedAct->setStatusTip(tr("Mark my QSL as requested"));
    connect(qslSentRequestedAct, SIGNAL(triggered()), this, SLOT( slotQSLSentMarkAsRequested()   ));

    qslSentViaDirectMarkRcvReqFromSearchAct = new QAction(tr("Via Direct && mark DX QSL as requested"), this);
    qslSentViaDirectMarkRcvReqFromSearchAct->setStatusTip(tr("Send this QSL via direct & mark DX QSL as requested"));
    connect(qslSentViaDirectMarkRcvReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectMarkDXReqFromSearch() ));

    qslSentViaBureauMarkRcvReqFromSearchAct = new QAction(tr("Via Bureau && mark DX QSL as requested"), this);
    qslSentViaBureauMarkRcvReqFromSearchAct->setStatusTip(tr("Send this QSL via bureau & mark DX QSL as requested"));
    connect(qslSentViaBureauMarkRcvReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureuMarkDXReqFromSearch() ));


    qslRecRequestedAct = new QAction(tr("&Request the QSL"), this);
    qslRecRequestedAct->setStatusTip(tr("Mark the QSL as requested"));
    connect(qslRecRequestedAct, SIGNAL(triggered()), this, SLOT( slotQSLRecMarkAsRequested()   ));


    qslRecViaBureauMarkReqFromSearchAct = new QAction(tr("Via bureau && mark my QSL as requested"), this);
    qslRecViaBureauMarkReqFromSearchAct->setStatusTip(tr("QSL received via bureau & mark my QSL as requested"));
    connect(qslRecViaBureauMarkReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauMarkReqFromSearch() ));

    qslRecViaBureauFromSearchAct = new QAction(tr("Via bureau"), this);
    qslRecViaBureauFromSearchAct->setStatusTip(tr("QSL received via bureau"));
    //qslRecViaBureauFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(qslRecViaBureauFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauFromSearch() ));

    qslRecViaDirectMarkReqFromSearchAct = new QAction(tr("Direc&t && mark as my QSL requested"), this);
    qslRecViaDirectMarkReqFromSearchAct->setStatusTip(tr("QSL received via direct & mark my QSL as requested"));
    connect(qslRecViaDirectMarkReqFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectMarkReqFromSearch() ));

    qslRecViaDirectFromSearchAct = new QAction(tr("Direc&t"), this);
    qslRecViaBureauFromSearchAct->setStatusTip(tr("QSL received via direct"));
    //qslRecViaDirectFromSearchAct->setShortcut(Qt::CTRL + Qt::Key_T);
    connect(qslRecViaDirectFromSearchAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectFromSearch() ));
}


void MainWindow::slotQSLSentViaBureuMarkDXReqFromSearch()
{
    //qDebug() << "slotQSLSentViaBureuMarkDXReqFromSearch: " << (qslSentViaBureauMarkRcvReqFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaBureauMarkRcvReqFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslSentViaBureauMarkRcvReqFromSearchAct->data()).toInt();

    dataProxy->qslSentViaBureau(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));
    dataProxy->qslRecAsRequested(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));


    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.

}


void MainWindow::slotQSLSentViaDirectMarkDXReqFromSearch()
{
    //qDebug() << "slotQSLSentViaDirectMarkDXReqFromSearch: " << (qslSentViaDirectMarkRcvReqFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaDirectMarkRcvReqFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;

    int _qsoId = (qslSentViaDirectMarkRcvReqFromSearchAct->data()).toInt();

    dataProxy->qslSentViaDirect(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));
    dataProxy->qslRecAsRequested(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));


    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}


void MainWindow::slotQSLSentViaBureauFromSearch()
{
   // //qDebug() << "MainWindow::slotQSLSentViaBureauFromSearch: " << (qslSentViaBureauFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslSentViaBureauFromSearchAct->data()).toInt();

    logWindow->qslSentViaBureau(_qsoId);
    //qslSentViaBureau(_qsoId);
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}


void MainWindow::slotQSLSentViaDirectFromSearch()
{
    //qDebug() << "MainWindow::slotQSLSentViaDirectFromSearch: " << (qslSentViaDirectFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((qslSentViaDirectFromSearchAct->data()).toInt());
    dataProxy->qslSentViaDirect(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }

    //qslSentViaDirect(_qsoId);

}


void MainWindow::slotQSLSentMarkAsRequested()
{
   // bool qslSentAsRequested(const int _qsoId, const QString _updateDate);  

    int _qsoId = (qslSentRequestedAct->data()).toInt();
    dataProxy->qslSentAsRequested(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}


void MainWindow::slotQSLRecMarkAsRequested()
{
    int _qsoId = (qslRecRequestedAct->data()).toInt();
    dataProxy->qslRecAsRequested(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"));
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
}


void MainWindow::slotQSLRecViaBureauFromSearch()
{
    //qDebug() << "MainWindow::slotQSLRecViaBureauFromLog: " << "- Id = " << QString::number( ((logModel->index( ( (qslRecViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;

    int _qsoId = (qslRecViaBureauFromSearchAct->data()).toInt();
    logWindow->qslRecViaBureau(_qsoId);
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }

}


void MainWindow::slotQSLRecViaBureauMarkReqFromSearch()
{
    //qDebug() << "MainWindow::slotQSLRecViaBureauMarkReqFromLog: " << "- Id = " << QString::number( ((logModel->index( ( (qslRecViaBureauFromSearchAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;

    int _qsoId = (qslRecViaBureauMarkReqFromSearchAct->data()).toInt();
    qslRecViaBureauMarkReq(_qsoId);
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }

}


void MainWindow::slotQSLRecViaDirectFromSearch()
{
    //qDebug() << "MainWindow::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslRecViaDirectFromSearchAct->data()).toInt();
    logWindow->qslRecViaDirect(_qsoId);
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}


void MainWindow::slotQSLRecViaDirectMarkReqFromSearch()
{
    //qDebug() << "MainWindow::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromSearchAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = (qslRecViaDirectMarkReqFromSearchAct->data()).toInt();
    qslRecViaDirectMarkReq(_qsoId);
    if(qslingNeeded)
    {
        searchToolNeededQSLToSend();
    }
    else
    {
        slotSearchBoxTextChanged();
    }
    // Mark Sent, Bureau, date, update log.
}

*/
void MainWindow::qslRecViaBureauMarkReq(const int _qsoId)
{
   // //qDebug() << "MainWindow::qslRecViaBureau: " << QString::number(_qsoId) << "/" << (dateTime->currentDateTime()).toString("yyyy/MM/dd") << endl;
    //setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed);
    dataProxy->qslRecViaBureau(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"), true);
    awards->setAwards(_qsoId);   //Update the DXCC award status

    logWindow->refresh();
    showAwards();

}


void MainWindow::qslRecViaDirectMarkReq(const int _qsoId)
{
    //qDebug() << "MainWindow::qslRecViaDirect: " << QString::number(_qsoId) << endl;

    dataProxy->qslRecViaDirect(_qsoId, (dateTime->currentDateTime()).toString("yyyy/MM/dd"), true);
    awards->setAwards(_qsoId);
    //setAwards(const int _dxcc, const int _waz, const int _band, const int _mode, const int _workedOrConfirmed);

    logWindow->refresh();
    showAwards();
}

/*
void MainWindow::slotQSOToEditFromSearch()
{
    //qDebug() << "slotQSOToEditFromSearch: " << (qsoToEditFromSearchAct->data()).toString() << endl;

    qsoToEdit((qsoToEditFromSearchAct->data()).toInt());

}

void MainWindow::slotQsoDeleteFromSearch()
{
    //qDebug() << "MainWindow::slotQsoDeleteFromSearch: " << (delQSOFromSearchAct->data()).toString() << endl;


    int QSOid = (delQSOFromSearchAct->data()).toInt();
    //int x = -1;

    QString _qrz = dataProxy->getCallFromId(QSOid);
    if (_qrz.length()>=3)
    {

        QString message = QString(tr("You have requested to delete the QSO with: %1")).arg(_qrz);

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(message);
        msgBox.setInformativeText(tr("Are you sure?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Yes:
            elogClublog->deleteQSO(dataProxy->getClubLogRealTimeFromId(QSOid));
            if(dataProxy->deleteQSO(QSOid))
            {
                if(qslingNeeded)
                {
                    searchToolNeededQSLToSend();
                }
                else
                {
                    slotSearchBoxTextChanged();
                }
                slotShowAwards();
                //dxccStatusWidget->refresh();
                //awards->recalculateAwards();
                //logWindow->refresh();
                //showAwards();

            }
            else
            {
                //TODO: The QSO could not be removed...
            }

            break;
            case QMessageBox::No:
              // No was clicked
            break;
            default:
              // should never be reached
            break;
        }

    }
    else
    {
         // TODO: The QSO to be removed was not found in the log
    }

}

*/



void MainWindow::keyPressEvent(QKeyEvent *event){

    /*
    if (!isStarted || isPaused || curPiece.shape() == NoShape) {
        QFrame::keyPressEvent(event);
        return;
    }
    */
    switch (event->key()) {
    case Qt::Key_Return:
        // ENTER PRESSED
        //slotQRZReturnPressed();
        break;
    case Qt::Key_Enter:

        // ENTER PRESSED
       // slotQRZReturnPressed();
        break;

    default:
        //QFrame::keyPressEvent(event);
        ;

    }
}

void MainWindow::checkIfWorkedB4(const QString _qrz)
{
    //qDebug() << "MainWindow::checkIfWorkedB4: " << _qrz << endl;

    int i = dataProxy->isWorkedB4(_qrz, currentLog); // Gets the QSO id if worked before

    switch (contestMode) {

        case CQ_WW_SSB:
            if (i>=0)
            {
                qrzgroupBox->setTitle(tr("DUPE"));
            }
            else
            {
                qrzgroupBox->setTitle(tr("QRZ"));
            }
        break;
        case CQ_WW_CW:
            if (i>=0)
            {
                qrzgroupBox->setTitle(tr("DUPE"));
            }
            else
            {
                qrzgroupBox->setTitle(tr("QRZ"));
            }
        break;
        default:
            //if (i>=0)
            //{
                //TODO: Change colors?
            //}

        break;

    }

    //return i;
}


void MainWindow::readConfigData()
{
   //qDebug() << "MainWindow::slotReadConfigData - 01" << endl;



    if (needToEnd)
    {
        return;
    }
    QFile file(configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //qDebug() << "MainWindow::slotReadConfigData: File not found" << configFileName << endl;
        if (configured)
        {
            //qDebug() << "MainWindow::readConfigData: configured = true" << endl;
        }
        else
        {
            //qDebug() << "MainWindow::readConfigDataw: configured = false" << endl;
        }
        slotSetup();

        return;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        processConfigLine(line);
    }

    defineStationCallsign();

    //qDebug() << "MainWindow::readConfigData: " << defaultADIFLogFile << endl;

    if ((useDefaultLogFileName) && (defaultADIFLogFile.length()>0))
    {
        useDefaultLogFileName = true;
    }
    else
    {
        useDefaultLogFileName = false;
    }

    infoWidget->setImperialSystem(imperialSystem);
/*
    if (imperialSystem)
    {
        distShortLabelN->setText(tr("Miles"));
        distLongLabelN->setText(tr("Miles"));
        //distShortLabel->setText( QString::number( Km2Mile(imperialSystem, (distShortLabel->text()).toInt() )) );
        //distLongLabel->setText( QString::number(Km2Mile(imperialSystem, (distLongLabel->text()).toInt()) ) );
    }
    else
    {
        distShortLabelN->setText(tr("Km"));
        distLongLabelN->setText(tr("Km"));
    }
    */
    infoLabel2->setText(world->getEntityName(currentEntity));
    infoWidget->showEntityInfo(currentEntity);

    //lastPower = myPower;
    //lastOperatorQRZ = operatorQRZ;
    //lastStationQRZ = stationQRZ;
    //lastMyLocator = myLocator;

    configured = true;
    awards->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    dxClusterWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    dxClusterWidget->setDXClusterSpotConfig(dxClusterShowHF, dxClusterShowVHF, dxClusterShowWARC, dxClusterShowWorked, dxClusterShowConfirmed, dxClusterShowAnn, dxClusterShowWWV, dxClusterShowWCY );
    dxClusterWidget->setMyQRZ(stationQRZ);

    initialContestModeConfiguration();

    if (upAndRunning)
    { // Next actions will not be executed in the first run
        slotClearButtonClicked();
        //createSearchResultsPanel();
    }

    // I need to init the CLUBLOG
    if (clublogActive)
    {
        elogClublog->setCredentials(clublogUser, clublogEmail, clublogPass, false);
    }
    else
    {

    }
    //qDebug() << "MainWindow::readConfigData: calling checkIfNewBandOrMode" << endl;
    checkIfNewBandOrMode();
    util->setVersion(softwareVersion);
    searchWidget->setVersion(softwareVersion);
    searchWidget->setCurrentLog(currentLog);
    infoWidget->setCurrentLog(currentLog);
    searchWidget->setColors (newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());
    infoWidget->setColors(newOneColor.name(), neededColor.name(), workedColor.name(), confirmedColor.name(), defaultColor.name());

//qDebug() << "MainWindow::slotReadConfigData - END" << endl;

}

bool MainWindow::processConfigLine(const QString _line){
    //qDebug() << "MainWindow::processConfigLine: " << _line << endl;

    QString line = _line.simplified();
    //line.simplified();
    QString aux;

    QStringList values = line.split("=", QString::SkipEmptyParts);


    if (line.startsWith('#')){
        //qDebug() << "MainWindow::processConfigLine: notes Line!" << endl;
        return true;
    }
    if (!( (line.contains('=')) && (line.contains(';')))){
        //qDebug() << "MainWindow::processConfigLine: Wrong Line!" << endl;
        return false;
    }
    QString field = (values.at(0)).toUpper();
    QString value = values.at(1);

    int endValue = value.indexOf(';');
    if (endValue>-1){

        value = value.left(value.length() - (value.length() - endValue));
    }


    if (field == "CALLSIGN"){
        mainQRZ = value;
    }else if (field=="CQZ"){
        my_CQz = value.toInt();
    }else if (field=="ITUZ"){
        my_ITUz = value.toInt();
    }else if (field=="CONTEST"){
        //qDebug() << "MainWindow::processConfigLine: CONTEST: " << endl;
        if (value=="CQ-WW-SSB"){
            contestMode = CQ_WW_SSB;
        } else if (value=="CQ-WW-CW"){
            contestMode = CQ_WW_CW;
        } else if (value=="CQ-WPX-SSB"){
            contestMode = CQ_WPX_SSB;
        } else if (value=="CQ-WPX-CW"){
            contestMode = CQ_WPX_CW;
        } else if (value=="CQ-WPX-RTTY"){
            contestMode = CQ_WPX_RTTY;
        } else{
            contestMode = DX;
        }

    }else if (field=="MODES"){
        readActiveModes(value.split(", ", QString::SkipEmptyParts));
    }else if (field=="BANDS"){
       //qDebug() << "MainWindow::processConfigLine: BANDS: " << value << endl;
        readActiveBands(value.split(", ", QString::SkipEmptyParts));
    }else if (field=="REALTIME"){
        //qDebug() << "MainWindow::processConfigLine: REALTIME: " << value.toUpper() << endl;

        realTime = util->trueOrFalse(value);

    }
    else if (field=="INMEMORY")
    {
    //qDebug() << "MainWindow::processConfigLine: INMEMORY: " << value.toUpper() << endl;

         DBinMemory = util->trueOrFalse(value);

    }
    else if (field =="DXCLUSTERSERVERTOUSE"){
        aux = value;  //dxfun.com:8000
        if (aux.contains(':'))
        {
            dxclusterServerToConnect = (aux.split(':', QString::SkipEmptyParts)).at(0);
            dxclusterServerPort = ((aux.split(':', QString::SkipEmptyParts)).at(1)).toInt();
        }

        if ((dxclusterServerToConnect.length()< 3) || (dxclusterServerPort <= 0))
        {            
            dxclusterServerToConnect = "dxfun.com";
            dxclusterServerPort = 8000;
        }
        dxClusterWidget->setDXClusterServer(dxclusterServerToConnect, dxclusterServerPort);       
    }


    else if(field=="POWER")
    {
        if (value.toDouble()>0.0)
        {
            myPower = value.toDouble();

        }

    }
    else if (field=="USEDEFAULTNAME")
    {
         useDefaultLogFileName = util->trueOrFalse(value);
    }

    else if (field=="IMPERIALSYSTEM")
    {
        imperialSystem = util->trueOrFalse(value);
    }
    else if (field=="SENDQSLWHENREC")
    {
        sendQSLWhenRec = util->trueOrFalse(value);
    }

    else if (field=="SHOWCALLSIGNINSEARCH")
    {
        searchWidget->setShowCallInSearch(util->trueOrFalse(value));
        //stationCallSignShownInSearch = util->trueOrFalse(value);
    }

    else if (field=="CHECKNEWVERSIONS"){
        checkNewVersions = util->trueOrFalse(value);
    }

    else if (field=="PROVIDEINFO"){
        reportInfo = util->trueOrFalse(value);
    }

    else if (field=="ALWAYSADIF")
    {
        alwaysADIF = util->trueOrFalse(value);
    }
    else if (field=="UTCTIME")
    {
        //qDebug() << "MainWindow::processConfigLine: UTCTIME: " << value.toUpper() <<endl;
        UTCTime = util->trueOrFalse(value);
    }
    else if (field=="KEEPMYDATA")
    {
        keepMyData  = util->trueOrFalse(value);
    }
    else if (field=="COMPLETEWITHPREVIOUS")
    {
        completeWithPrevious  = util->trueOrFalse(value);
    }
    else if (field=="DXCLUSTERSHOWHF")
    {
        dxClusterShowHF  = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWVHF")
    {
        dxClusterShowVHF = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWWARC")
    {
        dxClusterShowWARC  = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWWORKED")
    {
        dxClusterShowWorked = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWCONFIRMED")
    {
        dxClusterShowConfirmed = util->trueOrFalse(value);
    }

    else if (field=="DXCLUSTERSHOWANN")
    {
        dxClusterShowAnn = util->trueOrFalse(value);
    }


    else if (field=="DXCLUSTERSHOWWWV")
    {
        dxClusterShowWWV = util->trueOrFalse(value);
    }


    else if (field=="DXCLUSTERSHOWWCY")
    {
        dxClusterShowWCY = util->trueOrFalse(value);
    }

    else if (field=="DEFAULTADIFFILE")
    {
        defaultADIFLogFile = value.toLower();
        //qDebug() << "MainWindow::processConfigLine: " << defaultADIFLogFile << endl;
    }

    else if (field=="STATIONLOCATOR")
    {

        if ( locator->isValidLocator(value) )
        {
            myLocator = value.toUpper();
        }
    }
    else if(field=="NEWONECOLOR")
    {
        newOneColor.setNamedColor(value);
    }
    else if(field=="NEEDEDCOLOR")
    {
        neededColor.setNamedColor(value);
    }
    else if(field=="WORKEDCOLOR")
    {
        workedColor.setNamedColor(value);
    }
    else if(field=="CONFIRMEDCOLOR")
    {
        confirmedColor.setNamedColor(value);
    }
    else if(field=="DEFAULTCOLOR")
    {
        defaultColor.setNamedColor(value);
    }else if(field=="SELECTEDLOG")
    {
        currentLog = value.toInt();

        if (dataProxy->doesThisLogExist(currentLog))
        {

        }
        else
        {
            currentLog = 1;
            while(!dataProxy->doesThisLogExist(currentLog))
            {
                currentLog++;
            }
        }
        dxClusterWidget->setCurrentLog(currentLog);
        dxccStatusWidget->setCurrentLog(currentLog);
        //qDebug() << "MainWindow::processConfigLine: currentLog: " << value << endl;
    }else if(field=="CLUBLOGACTIVE")
    {
      //qDebug() << "MainWindow::processConfigLine: clublogActive: " << value << endl;
        clublogActive = util->trueOrFalse(value);
    }
    else if(field=="CLUBLOGREALTIME")
    {
       //qDebug() << "MainWindow::processConfigLine: clublogRealTime: " << value << endl;
        clublogRealTime = util->trueOrFalse(value);
    }
    else if(field=="CLUBLOGCALL")
    {
        clublogUser = value;
    }
    else if(field=="CLUBLOGPASS")
    {
        clublogPass = value;

    }
    else if(field=="CLUBLOGEMAIL")
    {
        clublogEmail = value;
    }
    else
    {
        //qDebug() << "MainWindow::processConfigLine: NONE: " << endl;
    }



    // Lines are: Option = value;


    return true;


}


/*********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************/


void MainWindow::checkIfNewBandOrMode()
{//Checks the log to see if there is a QSO with a band/mode
//that is not currently selected as active
    //qDebug() << "MainWindow::checkIfNewBandOrMode" << endl;
//    modes
//    bands

    setupDialog->checkIfNewBandOrMode(); // Update the Setup dialog with new bands or modes
    //qDebug() << "MainWindow::checkIfNewBandOrMode after setupDialog" << endl;

   //qDebug() << "MainWindow::checkIfNewBandOrMode - bands: " << endl;
    util->printQString(bands);
    //qDebug() << "MainWindow::checkIfNewBandOrMode - modes: " << endl;
    util->printQString(modes);

    QStringList bandsInLog = dataProxy->getBandsInLog(currentLog);
   //qDebug() << "MainWindow::checkIfNewBandOrMode - bandsInLog-1: " << endl;
    util->printQString(bandsInLog);
   //qDebug() << "MainWindow::checkIfNewBandOrMode - bandsInLog-2: " << endl;
    QStringList modesInLog = dataProxy->getModesInLog(currentLog);
    //qDebug() << "MainWindow::checkIfNewBandOrMode - modesInLog: " << endl;
    //util->printQString(modesInLog);

    //if (bandsInLog.length()>0)
    //{
        //bands.clear();

    //}
    QStringList qsTemp;
    qsTemp.clear();
    bands << bandsInLog;
    qsTemp << dataProxy->sortBandNamesBottonUp(bands);
    bands.clear();
    bands << qsTemp;

    //QSet<QString> set = bands.toSet();
    QSet<QString> set;
    //bands.clear();
    //bands << set.toList();


    //set.clear();
    if (modesInLog.length()>0)
    {
       modes << modesInLog;
    }

    set = modes.toSet();
    modes.clear();
    modes << set.toList();


/*
    if (bands.length()<1)
    {
        //Default band/modes
        bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
        modes << "SSB" << "CW" << "RTTY";
    }
*/
   //qDebug() << "MainWindow::checkIfNewBandOrMode - bands -" << QString::number(bands.length()) << endl;
    bandComboBox->clear();
    bandComboBox->addItems(bands);
	
	satTabWidget->addBands(bands);

   //qDebug() << "MainWindow::checkIfNewBandOrMode - modes -" << QString::number(modes.length()) << endl;
    modeComboBox->clear();
   //qDebug() << "MainWindow::checkIfNewBandOrMode - 1-" << QString::number(modeComboBox->count()) << endl;
    modeComboBox->addItems(modes);
   //qDebug() << "MainWindow::checkIfNewBandOrMode - 2-" << QString::number(modeComboBox->count()) << endl;

   //qDebug() << "MainWindow::checkIfNewBandOrMode - CurrentBand/CurrentBandShown: " << QString::number(currentBand) << "/" << QString::number(currentBandShown) << endl;
    dxccStatusWidget->setBands(bands);


   //qDebug() << "MainWindow::checkIfNewBandOrMode - bands2: " << endl;
    util->printQString(bands);
   //qDebug() << "MainWindow::checkIfNewBandOrMode - modes2: " << endl;
    util->printQString(modes);

   //qDebug() << "MainWindow::checkIfNewBandOrMode END" << endl;
}

/*********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
 **********************************************************************
**********************************************************************/

void MainWindow::readActiveBands (const QStringList actives)
{ // Checks a "10m, 12m" QString, checks if  they are valid bands and import to the
    // bands used in the program
   //qDebug() << "MainWindow::readActiveBands: "  << endl;
    util->printQString(actives);

    QString aux;
    bool atLeastOne = false;
    //QStringList values = actives;
    //QStringList values = actives.split(", ", QString::SkipEmptyParts);

    for (int i = 0; i < actives.size() ; i++)
    {
        if (db->isValidBand(actives.at(i)))
        {
            if (!atLeastOne)
            {
               //qDebug() << "MainWindow::readActiveBands (at least One!): " << actives.at(i) << endl;
                atLeastOne = true;
                bands.clear();
            }
            aux = actives.at(i);
            if (aux.length()>0)
            {
               bands << aux;
            }
            //bands << actives.at(i);

           //qDebug() << "MainWindow::readActiveBands: " << actives.at(i) << endl;
        }

    }
    bands.removeDuplicates();
   //qDebug() << "MainWindow::readActiveBands - END"  << endl;
}

void MainWindow::readActiveModes (const QStringList actives)
{
  //qDebug() << "MainWindow::readActiveModes: " << actives << endl;

    bool atLeastOne = false;
    QString aux;


    //QStringList values = actives.split(", ", QString::SkipEmptyParts);

    for (int i = 0; i < actives.size() ; i++)
    {
        if (db->isValidMode(actives.at(i), false))
        {
            if (!atLeastOne)
            {
                atLeastOne = true;
                //modes.clear();
            }
            aux = actives.at(i);
            if (aux.length()>0)
            {
               modes << aux;
            }

           // modes << actives.at(i);
        }

    }
    modes.removeDuplicates();

}



void MainWindow::createData()
{
    //qDebug() << "MainWindow::createData " << endl;

}


void MainWindow::createUIDX()
{
    //qDebug() << "MainWindow::createUIDX" << endl;
/*
    QStringList continents;
    QSqlQuery query2("SELECT shortname FROM continent");
    while (query2.next()) {
        if (query2.isValid())
        {
            continents << query2.value(0).toString();
        }
    }

    iotaContinentComboBox->addItems(continents);
    iotaNumberLineEdit = new QLineEdit;
    iotaNumberLineEdit->setInputMask("000");
    iotaNumberLineEdit->setText("000");
*/

    //bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    //modes << "SSB" << "CW" << "RTTY";
    bandComboBox->addItems(bands);
   //qDebug() << "MainWindow::createUIDX - 1-" << QString::number(modes.count()) << endl;
    modeComboBox->addItems(modes);
    txFreqSpinBox->setToolTip(tr("TX Frequency in MHz"));
    rxFreqSpinBox->setToolTip(tr("RX Frequency in MHz"));
    //myPowerSpinBox->setToolTip(tr("Power used for the QSO in watts"));
    rxPowerSpinBox->setToolTip(tr("Power used by the DX"));
    operatorLineEdit->setToolTip(tr("Logging operator's callsign"));
    stationCallSignLineEdit->setToolTip(tr("Callsign used over the air"));
    myLocatorLineEdit->setToolTip(tr("My QTH locator"));
    nameLineEdit->setToolTip(tr("Name of the DX"));
    qthLineEdit->setToolTip(tr("QTH of the DX"));
    locatorLineEdit->setToolTip(tr("Locator of the DX"));
    //freqQLCDNumber->setToolTip(tr("Frequency of the QSO"));
    qrzLineEdit->setToolTip(tr("QRZ of the QSO"));
    rstTXLineEdit->setToolTip(tr("TX RST"));
    rstRXLineEdit->setToolTip(tr("RX RST"));
    STXLineEdit->setToolTip(tr("TX Exchange"));
    SRXLineEdit->setToolTip(tr("RX Exchange"));
    bandComboBox->setToolTip(tr("Band of the QSO"));
    modeComboBox->setToolTip(tr("Mode of the QSO"));
    dateEdit->setToolTip(tr("Date of the QSO"));
    timeEdit->setToolTip(tr("Time of the QSO"));
    //statusBar->setToolTip(tr("Misc information"));
    //qsoStatusBar->setToolTip(tr("QSO information"));
    OKButton->setToolTip(tr("Add the QSO to the log"));
    //spotItButton->setToolTip(tr("Spots this QSO to the DX Cluster - This function is still not implemented"));
    clearButton->setToolTip(tr("Clears the box"));

//TODO REMOVE eQSL
/*
    clublogComboBox->setToolTip(tr("Status on ClubLog"));
    eqslSentComboBox->setToolTip(tr("Status of the eQSL sending"));
    eqslRecComboBox->setToolTip(tr("Status of the eQSL reception"));
    lotwSentComboBox->setToolTip(tr("Status of the LotW sending"));
    lotwRecComboBox->setToolTip(tr("Status of the LotW reception"));
    clublogQDateEdit->setToolTip(tr("Date of the ClubLog upload"));
    eqslSentQDateEdit->setToolTip(tr("Date of the eQSL sending"));
    eqslRecQDateEdit->setToolTip(tr("Date of the eQSL reception"));
    lotwSentQDateEdit->setToolTip(tr("Date of the LotW sending"));
    lotwRecQDateEdit->setToolTip(tr("Date of the LotW reception"));

    qslSentComboBox->setToolTip(tr("Status of the QSL sending"));
    qslRecComboBox->setToolTip(tr("Status of the QSL reception"));
    qslSentViaComboBox->setToolTip(tr("QSL sending information"));
    qslRecViaComboBox->setToolTip(tr("QSL reception information"));

    qslSentQDateEdit->setToolTip(tr("Date of the QSL sending"));
    qslRecQDateEdit->setToolTip(tr("Date of the QSL reception"));
    qslmsgTextEdit->setToolTip(tr("Message of the QSL"));
    qslViaLineEdit->setToolTip(tr("QSL via information"));
*/
    dxccConfirmedQLCDNumber->setToolTip(tr("Number of confirmed DXCC entities"));
    dxccWorkedQLCDNumber->setToolTip(tr("Number of worked DXCC entities"));
    wazConfirmedQLCDNumber->setToolTip(tr("Number of confirmed WAZ zones"));
    wazWorkedQLCDNumber->setToolTip(tr("Number of worked WAZ zones"));
    localConfirmedQLCDNumber->setToolTip(tr("Number of confirmed local references"));
    localWorkedQLCDNumber->setToolTip(tr("Number of worked local references"));
    qsoConfirmedQLCDNumber->setToolTip(tr("Number of confirmed QSOs"));
    qsoWorkedQLCDNumber->setToolTip(tr("Number of worked QSOs"));
    dxMarathonDXCCQLCDNumber->setToolTip(tr("Number of DXCC worked on the selected year"));
    dxMarathonCQQLCDNumber->setToolTip(tr("Number of CQ Zones worked on the selected year"));
    dxMarathonPointsQLCDNumber->setToolTip(tr("Score for the DXMarathon on the selected year"));
    operatingYearsComboBox->setToolTip(tr("Select the year you want to check"));

    infoLabel1->setToolTip(tr("Status of the DX entity"));
    infoLabel2->setToolTip(tr("Name of the DX entity"));

    //entityPrimDivComboBox->setToolTip(tr("Select the primary division for this QSO"));
    //entitySecDivComboBox->setToolTip(tr("Select the secondary division for this QSO"));
    //entityNameComboBox->setToolTip(tr("Select the propagation mode for this current QSO"));
    //propModeComboBox->setToolTip(tr("Select the propagation mode for this current QSO"));

    //QGridLayout *layout = new QGridLayout;

    dxUpLeftInputFrame = new QFrame;

    //dxUpRightOutputFrame = new QFrame;
    //dxUpRightOutputFrame->setFrameShadow(QFrame::Raised);
    //dxUpRightOutputFrame->setFrameStyle(QFrame::StyledPanel);

    dxUpLeftTab = new QTabWidget;
    dxUpRightTab = new QTabWidget;
    dxBottonTab = new QTabWidget;

    dxUpLeftTab->setTabPosition(QTabWidget::North);
    dxUpRightTab->setTabPosition(QTabWidget::South);

    QWidget *qsoInputTabWidget = new QWidget;
    //QFormLayout *qsoInputTabWidgetLayout = new QFormLayout;
    QLabel *nameLabel = new QLabel(qsoInputTabWidget);
    nameLabel->setText(tr("Name"));
    nameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *qthLabel = new QLabel(qsoInputTabWidget);
    qthLabel->setText(tr("QTH"));
    qthLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    QLabel *locLabel = new QLabel(qsoInputTabWidget);
    locLabel->setText(tr("Locator"));
    locLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rxPowerSpinBoxLabelN = new QLabel(tr("Power(rx)"));
    rxPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);


    QLabel *rstTxLabelN = new QLabel(tr("RST(tx)"));
    rstTxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *rstRxLabelN = new QLabel(tr("RST(rx)"));
    rstRxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    rstTXLineEdit->setInputMask("#990");
    rstRXLineEdit->setInputMask("#990");
    rstTXLineEdit->setText("59");
    rstRXLineEdit->setText("59");
    rstTXLineEdit->setMaxLength(3);
    rstRXLineEdit->setMaxLength(3);
    //rstTXLineEdit->setFixedWidth(30);
    //rstRXLineEdit->setFixedWidth(30);

    QGridLayout *RSTLayout = new QGridLayout;
    RSTLayout->addWidget(rstTxLabelN, 0, 0);
    RSTLayout->addWidget(rstTXLineEdit, 1, 0);
    RSTLayout->addWidget(rstRxLabelN, 0, 1);
    RSTLayout->addWidget(rstRXLineEdit, 1, 1);

    //QLabel *spacerLabel = new QLabel(tr("SPACERQTH"));

    QHBoxLayout *qthHLayout = new QHBoxLayout;
    qthHLayout->addWidget(qthLabel);
    //qthHLayout->addSpacerItem(new QSpacerItem(50,1));
    //qthHLayout->addWidget(spacerLabel);

    QVBoxLayout *qthLayout = new QVBoxLayout;
    qthLayout->addLayout(qthHLayout);
    qthLayout->addWidget(qthLineEdit);

    QVBoxLayout *rstQTHLayout = new QVBoxLayout;
    rstQTHLayout->addLayout(RSTLayout);
    rstQTHLayout->addLayout(qthLayout);

    QLabel *txfreqLabelN = new QLabel(tr("Freq TX"));
    txfreqLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    QLabel *rxfreqLabelN = new QLabel(tr("Freq RX"));
    rxfreqLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QGridLayout *freqLayout = new QGridLayout;
    freqLayout->addWidget(txfreqLabelN, 0, 0);
    freqLayout->addWidget(rxfreqLabelN, 0, 1);
    freqLayout->addWidget(txFreqSpinBox, 1, 0);
    freqLayout->addWidget(rxFreqSpinBox, 1, 1);

    QVBoxLayout *locVLayout = new QVBoxLayout;
    locVLayout->addWidget(locLabel);
    locVLayout->addWidget(locatorLineEdit);

    //QLabel *spacerLocLabel = new QLabel(tr("SPACERLoc"));
    QHBoxLayout *locLayout = new QHBoxLayout;
    //locLayout->addSpacerItem(new QSpacerItem(50,1));
    //locLayout->addWidget(spacerLocLabel);
    locLayout->addLayout(locVLayout);

    QVBoxLayout *freqLocLayout = new QVBoxLayout;
    freqLocLayout->addLayout(freqLayout);
    freqLocLayout->addLayout(locLayout);

    QHBoxLayout *rstfreqLayout = new QHBoxLayout;
    rstfreqLayout->addLayout(rstQTHLayout);
    rstfreqLayout->addLayout(freqLocLayout);

    QVBoxLayout *rxPowerLayout = new QVBoxLayout;
    rxPowerLayout->addWidget(rxPowerSpinBoxLabelN);
    rxPowerLayout->addWidget(rxPowerSpinBox);



    //QLabel *spacerNameLabel = new QLabel(tr("SPACERName"));
    QHBoxLayout *nameHLayout = new QHBoxLayout;
    nameHLayout->addWidget(nameLabel);
    //nameHLayout->addSpacerItem(new QSpacerItem(50,1));
    //nameHLayout->addWidget(spacerNameLabel );

    QVBoxLayout *nameLayout = new QVBoxLayout;
    nameLayout->addLayout(nameHLayout);
    nameLayout->addWidget(nameLineEdit);


    QVBoxLayout *rxPwrLayout = new QVBoxLayout;
    rxPwrLayout->addWidget(rxPowerSpinBoxLabelN);
    rxPwrLayout->addWidget(rxPowerSpinBox);

    //QLabel *spacerHBottomLabel = new QLabel(tr("SPACERHBottom"));
    QHBoxLayout *namePwrHLayout = new QHBoxLayout;
    namePwrHLayout->addLayout(nameLayout);
    //namePwrHLayout->addSpacerItem(new QSpacerItem(50,1));
    //namePwrHLayout->addWidget(spacerHBottomLabel);
    namePwrHLayout->addLayout(rxPwrLayout);


    //QLabel *spacerBottomLabel = new QLabel(tr("SPACERBottom"));
    QVBoxLayout *namePwrLayout = new QVBoxLayout;
    //namePwrLayout->addSpacerItem(new QSpacerItem(1,50));
    //namePwrLayout->addWidget(spacerBottomLabel);
    namePwrLayout->addLayout(namePwrHLayout);

    QVBoxLayout *qsoInputTabWidgetMainLayout = new QVBoxLayout;
    qsoInputTabWidgetMainLayout->addLayout(rstfreqLayout);
    qsoInputTabWidgetMainLayout->addLayout(namePwrLayout);

    qsoInputTabWidget->setLayout(qsoInputTabWidgetMainLayout);

    //QWidget *qslInputTabWidget = new QWidget;
    //QWidget *eqslInputTabWidget = new QWidget;
    //QWidget *commentInputTabWidget = new QWidget;
    //QWidget *othersInputTabWidget = new QWidget;
    //QWidget *myDataInputTabWidget = new QWidget;

    dxUpLeftTab->addTab(qsoInputTabWidget, tr("QSO"));

    // QSL Tab definition starts here

    QLabel *QSLSentLabelN = new QLabel(tr("QSL Sent"));
    QSLSentLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *QSLRecLabelN = new QLabel(tr("QSL Rec"));
    QSLRecLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *QSLViaLabelN = new QLabel(tr("QSL Via"));
    QSLViaLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    /*
    entityNameComboBox = new QComboBox;entitiesList
    if (entitiesList.size()>1)
    {
        entitiesList.prepend("00-Not Identified (000)");
        entityNameComboBox->addItems(entitiesList);
    }

    if (propModeList.size()>1)
    {
        propModeList.prepend("00 - Not - Not Identified");
        propModeComboBox->addItems(propModeList);
    }


    QGridLayout *QSLLayout = new QGridLayout;
    QSLLayout->addWidget(QSLSentLabelN, 0, 0);
    QSLLayout->addWidget(QSLRecLabelN, 1, 0);
    QSLLayout->addWidget(QSLViaLabelN, 2, 0);

    QSLLayout->addWidget(qslSentComboBox, 0, 1);
    QSLLayout->addWidget(qslRecComboBox, 1, 1);
    QSLLayout->addWidget(qslViaLineEdit, 2, 1, 1, -1);

    QSLLayout->addWidget(qslSentQDateEdit, 0, 2);
    QSLLayout->addWidget(qslRecQDateEdit, 1, 2);
    QSLLayout->addWidget(qslSentViaComboBox, 0, 3);
    QSLLayout->addWidget(qslRecViaComboBox, 1, 3);


    qslInputTabWidget->setLayout(QSLLayout);
*/
/*

    // eQSL Tab definition starts here

    QLabel *clublogLabelN = new QLabel(tr("ClubLog"));
    clublogLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *eQSLSentLabelN = new QLabel(tr("eQSL Sent"));
    eQSLSentLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *eQSLRecLabelN = new QLabel(tr("eQSL Rec"));
    eQSLRecLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *lotWSentLabelN = new QLabel(tr("LotW Sent"));
    lotWSentLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *lotWRecLabelN = new QLabel(tr("LotW Rec"));
    lotWRecLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);


    QHBoxLayout *eqslSentLayout = new QHBoxLayout;
    eqslSentLayout->addWidget(eqslSentComboBox);
    eqslSentLayout->addWidget(eqslSentQDateEdit);
    QHBoxLayout *eqslRecLayout = new QHBoxLayout;
    eqslRecLayout->addWidget(eqslRecComboBox);
    eqslRecLayout->addWidget(eqslRecQDateEdit);
    QHBoxLayout *lotwSentLayout = new QHBoxLayout;
    lotwSentLayout->addWidget(lotwSentComboBox);
    lotwSentLayout->addWidget(lotwSentQDateEdit);
    QHBoxLayout *lotwRecLayout = new QHBoxLayout;
    lotwRecLayout->addWidget(lotwRecComboBox);
    lotwRecLayout->addWidget(lotwRecQDateEdit);

    QFormLayout *eqslInputTabWidgetLayout = new QFormLayout;

    eqslInputTabWidgetLayout->addRow(eQSLSentLabelN, eqslSentLayout);
    eqslInputTabWidgetLayout->addRow(eQSLRecLabelN, eqslRecLayout);
    eqslInputTabWidgetLayout->addRow(lotWSentLabelN, lotwSentLayout);
    eqslInputTabWidgetLayout->addRow(lotWRecLabelN, lotwRecLayout);
*/
/*
    QGridLayout *eqslInputTabWidgetLayout = new QGridLayout;
    eqslInputTabWidgetLayout->addWidget(clublogLabelN, 0, 0);
    eqslInputTabWidgetLayout->addWidget(eQSLSentLabelN, 1, 0);
    eqslInputTabWidgetLayout->addWidget(eQSLRecLabelN, 2, 0);
    eqslInputTabWidgetLayout->addWidget(lotWSentLabelN, 3, 0);
    eqslInputTabWidgetLayout->addWidget(lotWRecLabelN, 4, 0);

    eqslInputTabWidgetLayout->addWidget(clublogComboBox, 0, 1);
    eqslInputTabWidgetLayout->addWidget(eqslSentComboBox, 1, 1);
    eqslInputTabWidgetLayout->addWidget(eqslRecComboBox, 2, 1);
    eqslInputTabWidgetLayout->addWidget(lotwSentComboBox, 3, 1);
    eqslInputTabWidgetLayout->addWidget(lotwRecComboBox, 4, 1);

    eqslInputTabWidgetLayout->addWidget(clublogQDateEdit, 0, 2);
    eqslInputTabWidgetLayout->addWidget(eqslSentQDateEdit, 1, 2);
    eqslInputTabWidgetLayout->addWidget(eqslRecQDateEdit, 2, 2);
    eqslInputTabWidgetLayout->addWidget(lotwSentQDateEdit, 3, 2);
    eqslInputTabWidgetLayout->addWidget(lotwRecQDateEdit, 4, 2);

    eqslInputTabWidget->setLayout(eqslInputTabWidgetLayout);

    dxUpLeftTab->addTab(eqslInputTabWidget, tr("eQSL-old"));
*/
/*
    // NOTES tab starts here
    QGridLayout *notesInputTabWidgetLayout = new QGridLayout;
    notesInputTabWidgetLayout->addWidget(notesTextEdit, 0, 0);
    notesInputTabWidget->setLayout(notesInputTabWidgetLayout);
    i = dxUpLeftTab->addTab(notesInputTabWidget, tr("Notes"));
*/
    //dxUpLeftTab->addTab(qslInputTabWidget, tr("QSL"));
    dxUpLeftTab->addTab(QSLTabWidget, tr("QSL"));
    dxUpLeftTab->addTab(eQSLTabWidget, tr("eQSL"));

    // COMMENT tab starts here


    dxUpLeftTab->addTab(commentTabWidget, tr("Comment"));

    //QGridLayout *commentInputTabWidgetLayout = new QGridLayout;
    //commentInputTabWidgetLayout->addWidget(commentLineEdit, 0, 0);
    //commentInputTabWidget->setLayout(commentInputTabWidgetLayout);
    //i = dxUpLeftTab->addTab(commentInputTabWidget, tr("Comment"));

    //entityPrimLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    //entitySecLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    //iotaAwardLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    //entityNameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    //propModeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

// Others Tab starts here
/*
    QGridLayout *othersInputTabWidgetLayout = new QGridLayout;
    othersInputTabWidgetLayout->addWidget(entityNameLabel, 0, 0);
    othersInputTabWidgetLayout->addWidget(entityNameComboBox, 0, 1, 1, 2);

    othersInputTabWidgetLayout->addWidget(entityPrimLabel, 1, 0);
    othersInputTabWidgetLayout->addWidget(entityPrimDivComboBox, 1, 1, 1, 2);
    othersInputTabWidgetLayout->addWidget(entitySecLabel, 2, 0);
    othersInputTabWidgetLayout->addWidget(entitySecDivComboBox, 2, 1, 1, 2);
    othersInputTabWidgetLayout->addWidget(iotaAwardLabel, 3, 0);
    othersInputTabWidgetLayout->addWidget(iotaContinentComboBox, 3, 1);
    othersInputTabWidgetLayout->addWidget(iotaNumberLineEdit, 3, 2);
    othersInputTabWidgetLayout->addWidget(propModeLabel, 4, 0);
    othersInputTabWidgetLayout->addWidget(propModeComboBox, 4, 1, 1, 2);
    othersInputTabWidget->setLayout(othersInputTabWidgetLayout);
*/
    //i = dxUpLeftTab->addTab(othersInputTabWidget, tr("Others"));
    othersTabWidget->setEntitiesList(world->getEntitiesNames());
    dxUpLeftTab->addTab(othersTabWidget, tr("Others"));




// MyData tab starts here
/*
    QLabel *myPowerSpinBoxLabelN = new QLabel(tr("Power"));
    myPowerSpinBoxLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *operatorLabelN = new QLabel(tr("Operator"));

    operatorLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    QLabel *stationCallSignLabelN = new QLabel(tr("Station Callsign"));

    stationCallSignLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    QLabel *myLocatorLabelN = new QLabel(tr("My Locator"));

    myLocatorLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QGridLayout *myDataInputTabWidgetLayout = new QGridLayout;
    myDataInputTabWidgetLayout->addWidget(myPowerSpinBoxLabelN, 0, 0);
    myDataInputTabWidgetLayout->addWidget(operatorLabelN, 1, 0);
    myDataInputTabWidgetLayout->addWidget(stationCallSignLabelN, 2, 0);
    myDataInputTabWidgetLayout->addWidget(myLocatorLabelN, 3, 0);

    myDataInputTabWidgetLayout->addWidget(myPowerSpinBox, 0, 1);
    myDataInputTabWidgetLayout->addWidget(operatorLineEdit, 1, 1);
    myDataInputTabWidgetLayout->addWidget(stationCallSignLineEdit, 2, 1);
    myDataInputTabWidgetLayout->addWidget(myLocatorLineEdit, 3, 1);

    myDataInputTabWidget->setLayout(myDataInputTabWidgetLayout);
    i = dxUpLeftTab->addTab(myDataInputTabWidget, tr("My Data"));
*/
    dxUpLeftTab->addTab(myDataTabWidget, tr("My Data"));
  // MyData Tab finishes here

    // Sat Tab starts hre
    dxUpLeftTab->addTab(satTabWidget, tr("Satellite"));


    // Sat Tab finishes here


    QHBoxLayout *TimeLayout = new QHBoxLayout;
    TimeLayout->addWidget(dateEdit);
    TimeLayout->addWidget(timeEdit);

    QHBoxLayout *BandModeLayout = new QHBoxLayout;
    BandModeLayout->addWidget(bandComboBox);
    BandModeLayout->addWidget(modeComboBox);

    QHBoxLayout *QrzBandModeLayout = new QHBoxLayout;
    QrzBandModeLayout->addWidget(qrzLineEdit);
    QrzBandModeLayout->addLayout(BandModeLayout);


    qrzgroupBox = new QGroupBox(tr("QRZ"));
    qrzgroupBox->setFlat(true);
    QVBoxLayout *qrzvbox = new QVBoxLayout;
    //qrzvbox->addWidget(qrzLineEdit);
    qrzvbox->addLayout(QrzBandModeLayout);
    //qrzvbox->addLayout(BandModeLayout);
    qrzvbox->addStretch(1);
    qrzgroupBox->setLayout(qrzvbox);


    //QHBoxLayout *statusBarLayout = new QHBoxLayout;
    //statusBarLayout->addWidget(statusBar);
    //statusBarLayout->addWidget(qsoStatusBar);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(OKButton);
    //buttonsLayout->addWidget(spotItButton);
    buttonsLayout->addWidget(clearButton);

    QDateTimeEdit *dateEdit = new QDateTimeEdit(QDate::currentDate());
    dateEdit->setDisplayFormat("yyyy/MM/dd");
    timeEdit->setDisplayFormat("HH:mm:ss");


    QGridLayout *dxUpLeftInputFrameLayout = new QGridLayout;
    dxUpLeftInputFrameLayout->addWidget(qrzgroupBox, 0, 0, 1, 0);
    dxUpLeftInputFrameLayout->addLayout(TimeLayout, 1, 0);
    //dxUpLeftInputFrameLayout->addLayout(BandModeLayout, 1, 1);
    dxUpLeftInputFrameLayout->addLayout(buttonsLayout,1, 1);

    dxUpLeftInputFrame->setLayout(dxUpLeftInputFrameLayout);

    QSplitter *upLeftSplitter = new QSplitter (this);
    upLeftSplitter->addWidget(dxUpLeftInputFrame);
    upLeftSplitter->addWidget(dxUpLeftTab);
    upLeftSplitter->setOrientation(Qt::Vertical);

    //QHBoxLayout *line2Layout = new QHBoxLayout;
    //line2Layout->addWidget(flagIcon);
    //line2Layout->addWidget(infoLabel2);

    QVBoxLayout *dxUpRightFixLayout = new QVBoxLayout;
    dxUpRightFixLayout->addWidget(infoLabel1);
    dxUpRightFixLayout->addWidget(infoLabel2);
    //dxUpRightFixLayout->addLayout(line2Layout);


    infoLabel1->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    infoLabel2->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

/*
    bandLabel1->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel2->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel3->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel4->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel5->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel6->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel7->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel8->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel9->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel10->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel11->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    bandLabel12->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *continentLabelN = new QLabel(tr("Continent"));
    continentLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    continentLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *prefixLabelN = new QLabel(tr("Prefix"));
    prefixLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    prefixLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *cqzLabelN = new QLabel(tr("CQ"));
    cqzLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    cqzLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *ituzLabelN = new QLabel(tr("ITU"));
    ituzLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    ituzLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);


    QLabel *shortLabelN = new QLabel(tr("Short Path"));
    shortLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *longLabelN = new QLabel(tr("Long Path"));
    longLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *gradShortLabelN = new QLabel(tr("Grad"));
    gradShortLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);    
    gradShortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    distShortLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *gradLongLabelN = new QLabel(tr("Grad"));
    gradLongLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    gradLongLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    distShortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    distLongLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    distLongLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QGridLayout *dxUpRightInfoTabmini1Layout = new QGridLayout;
    dxUpRightInfoTabmini1Layout->addWidget(continentLabelN, 0, 0);
    dxUpRightInfoTabmini1Layout->addWidget(continentLabel, 1, 0);
    dxUpRightInfoTabmini1Layout->addWidget(prefixLabelN, 0, 1);
    dxUpRightInfoTabmini1Layout->addWidget(prefixLabel, 1, 1);
    dxUpRightInfoTabmini1Layout->addWidget(cqzLabelN, 0, 2);
    dxUpRightInfoTabmini1Layout->addWidget(cqzLabel, 1, 2);
    dxUpRightInfoTabmini1Layout->addWidget(ituzLabelN, 0, 3);
    dxUpRightInfoTabmini1Layout->addWidget(ituzLabel, 1, 3);

    QGridLayout *dxUpRightInfoTabmini2Layout = new QGridLayout;
    dxUpRightInfoTabmini2Layout->addWidget(shortLabelN, 0, 0, 1, 0);
    dxUpRightInfoTabmini2Layout->addWidget(gradShortLabelN, 1, 0);
    dxUpRightInfoTabmini2Layout->addWidget(gradShortLabel, 1, 1);
    dxUpRightInfoTabmini2Layout->addWidget(distShortLabelN, 1, 2);
    dxUpRightInfoTabmini2Layout->addWidget(distShortLabel, 1, 3);

    QGridLayout *dxUpRightInfoTabmini3Layout = new QGridLayout;
    dxUpRightInfoTabmini3Layout->addWidget(longLabelN, 0, 0, 1, 0);
    dxUpRightInfoTabmini3Layout->addWidget(gradLongLabelN, 1, 0);
    dxUpRightInfoTabmini3Layout->addWidget(gradLongLabel, 1, 1);
    dxUpRightInfoTabmini3Layout->addWidget(distLongLabelN, 1, 2);
    dxUpRightInfoTabmini3Layout->addWidget(distLongLabel, 1, 3);

    QHBoxLayout *dxUpRightInfoTabmini4Layout = new QHBoxLayout;
    dxUpRightInfoTabmini4Layout->addLayout(dxUpRightInfoTabmini2Layout);
    dxUpRightInfoTabmini4Layout->addLayout(dxUpRightInfoTabmini3Layout);
*/

    //QWidget *infoTabWidget = new QWidget;
/*
    QGridLayout *dxUpRightInfoBandsTabLayout = new QGridLayout;
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel1, 0, 0);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel2, 0, 1);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel3, 0, 2);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel4, 0, 3);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel5, 0, 4);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel6, 0, 5);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel7, 1, 0);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel8, 1, 1);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel9, 1, 2);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel10, 1, 3);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel11, 1, 4);
    dxUpRightInfoBandsTabLayout->addWidget(bandLabel12, 1, 5);

    QVBoxLayout *dxUpRightInfoTabLayout = new QVBoxLayout;
    dxUpRightInfoTabLayout->addLayout(dxUpRightInfoBandsTabLayout);
    dxUpRightInfoTabLayout->addLayout(dxUpRightInfoTabmini1Layout);
    dxUpRightInfoTabLayout->addLayout(dxUpRightInfoTabmini4Layout);


    infoTabWidget->setLayout(dxUpRightInfoTabLayout);
*/
/*
addLayout ( QLayout * layout, int row, int column,
int rowSpan, int columnSpan, Qt::Alignment alignment = 0 )
*/

    QLabel *dxMarathonTopDXCCLabelN = new QLabel(tr("DXCC"));
    QLabel *dxMarathonTopCQLabelN = new QLabel(tr("CQ"));
    QLabel *dxMarathonTopScoreLabelN = new QLabel(tr("Score"));
    QLabel *dxMarathonLabelN = new QLabel(tr("DX-Marathon"));

    dxMarathonTopDXCCLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    dxMarathonTopCQLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    dxMarathonTopScoreLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    dxMarathonLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    //QWidget *searchTabWidget = new QWidget;
   // dxUpRightTab->addTab(infoTabWidget, tr("Info-Old"));
    dxUpRightTab->addTab(infoWidget, tr("Info"));


    QWidget *awardsTabWidget = new QWidget;

    QLabel *awardLabelN = new QLabel(tr("Award"));    
    awardLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);    

    QLabel *confirmedLabelN = new QLabel(tr("Confirmed"));
    confirmedLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *workedLabelN = new QLabel(tr("Worked"));    
    workedLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *dxccLabelN = new QLabel(tr("DXCC"));

    //dxccLabelN->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    dxccLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *wazLabelN = new QLabel(tr("WAZ"));    
    wazLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *localLabelN = new QLabel(tr("Local"));    
    localLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    QLabel *qsoNLabelN = new QLabel(tr("QSOs"));
    qsoNLabelN->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

    //QVBoxLayout *dxMarathonTLayout = new QVBoxLayout;
    //dxMarathonTLayout->addWidget();
    //dxMarathonTLayout->addWidget();
    QGridLayout *dxMarathonDLayout = new QGridLayout;
/*
    dxMarathonDLayout->addWidget(dxMarathonTopDXCCLabelN, 0, 0);
    dxMarathonDLayout->addWidget(dxMarathonTopCQLabelN, 0, 1);
    dxMarathonDLayout->addWidget(dxMarathonTopScoreLabelN, 0, 2);
    dxMarathonDLayout->addWidget(dxMarathonDXCC, 1, 0);
    dxMarathonDLayout->addWidget(dxMarathonCQ, 1, 1);
    dxMarathonDLayout->addWidget(dxMarathonPoints, 1, 2);
*/
    dxMarathonDLayout->addWidget(dxMarathonTopDXCCLabelN, 0, 0);
    dxMarathonDLayout->addWidget(dxMarathonTopCQLabelN, 0, 1);
    dxMarathonDLayout->addWidget(dxMarathonTopScoreLabelN, 0, 2);
    dxMarathonDLayout->addWidget(dxMarathonDXCCQLCDNumber, 1, 0);
    dxMarathonDLayout->addWidget(dxMarathonCQQLCDNumber, 1, 1);
    dxMarathonDLayout->addWidget(dxMarathonPointsQLCDNumber, 1, 2);

    QVBoxLayout *dxMarathonTLayout = new QVBoxLayout;
    dxMarathonTLayout->addWidget(dxMarathonLabelN);
    dxMarathonTLayout->addWidget(operatingYearsComboBox);

    QGridLayout *dxUpRightAwardsTabLayout = new QGridLayout;
    dxUpRightAwardsTabLayout->addWidget(awardLabelN, 0, 0);
    dxUpRightAwardsTabLayout->addWidget(workedLabelN, 0, 1);
    dxUpRightAwardsTabLayout->addWidget(confirmedLabelN, 0, 2);
    dxUpRightAwardsTabLayout->addWidget(dxccLabelN, 1, 0);
    dxUpRightAwardsTabLayout->addWidget(dxccWorkedQLCDNumber, 1, 1);
    dxUpRightAwardsTabLayout->addWidget(dxccConfirmedQLCDNumber, 1, 2);
    dxUpRightAwardsTabLayout->addWidget(wazLabelN, 2, 0);
    dxUpRightAwardsTabLayout->addWidget(wazWorkedQLCDNumber, 2, 1);
    dxUpRightAwardsTabLayout->addWidget(wazConfirmedQLCDNumber, 2, 2);
    dxUpRightAwardsTabLayout->addWidget(localLabelN, 3, 0);
    dxUpRightAwardsTabLayout->addWidget(localWorkedQLCDNumber, 3, 1);
    dxUpRightAwardsTabLayout->addWidget(localConfirmedQLCDNumber, 3, 2);
    dxUpRightAwardsTabLayout->addWidget(qsoNLabelN, 4, 0);
    dxUpRightAwardsTabLayout->addWidget(qsoWorkedQLCDNumber, 4, 1);
    dxUpRightAwardsTabLayout->addWidget(qsoConfirmedQLCDNumber, 4, 2);

    dxUpRightAwardsTabLayout->addLayout(dxMarathonTLayout, 5, 0);
    dxUpRightAwardsTabLayout->addLayout(dxMarathonDLayout, 5, 1, 1, -1);
    dxUpRightAwardsTabLayout->addWidget(recalculateAwardsButton, 6, 1);

    awardsTabWidget->setLayout(dxUpRightAwardsTabLayout);

    dxUpRightTab->addTab(awardsTabWidget, tr("Awards"));

/*
    searchgroupBox = new QGroupBox(tr("Search"));
    searchgroupBox->setFlat(true);
    QVBoxLayout *searchvbox = new QVBoxLayout;
    searchvbox->addWidget(searchBoxLineEdit);
    searchvbox->addStretch(1);
    searchgroupBox->setLayout(searchvbox);
*/
/*
    QHBoxLayout *dxUpRightButtonsLayout = new QHBoxLayout;
    dxUpRightButtonsLayout->addWidget(searchBoxReSearchButton);
    dxUpRightButtonsLayout->addWidget(searchBoxClearButton);
    dxUpRightButtonsLayout->addWidget(searchBoxSelectAllButton);
    dxUpRightButtonsLayout->addWidget(searchBoxExportButton);
*/
/*
    QGridLayout *dxUpRightSearchTabLayout = new QGridLayout;
    //dxUpRightSearchTabLayout->addWidget(searchgroupBox, 0, 0 );
    //dxUpRightSearchTabLayout->addLayout(dxUpRightLineAndButtonsLayout, 0, 1 );
    dxUpRightSearchTabLayout->addWidget(searchBoxLineEdit, 0, 0, -1, 0);
    dxUpRightSearchTabLayout->addLayout(dxUpRightButtonsLayout, 1, 0, -1, 0);
    dxUpRightSearchTabLayout->addWidget(searchResultsTreeWidget, 2, 0, -1, 0 );
*/

/*
    QHBoxLayout *dxUpRightSearchTopLayout = new QHBoxLayout;
    dxUpRightSearchTopLayout->addWidget(searchBoxLineEdit);
    dxUpRightSearchTopLayout->addWidget(searchAllRadioButton);

    QVBoxLayout *dxUpRightSearchTabLayout = new QVBoxLayout;
    //dxUpRightSearchTabLayout->addWidget(searchgroupBox, 0, 0 );
    //dxUpRightSearchTabLayout->addLayout(dxUpRightLineAndButtonsLayout, 0, 1 );
    //dxUpRightSearchTabLayout->addWidget(searchBoxLineEdit);
    dxUpRightSearchTabLayout->addLayout(dxUpRightSearchTopLayout);
    //dxUpRightSearchTabLayout->addLayout(dxUpRightButtonsLayout);
    dxUpRightSearchTabLayout->addWidget(searchResultsTreeWidget);

    searchTabWidget->setLayout(dxUpRightSearchTabLayout);
*/
    //searchTabWidget->setLayout(dxUpRightSearchALLLayout);

    //dxUpRightTab->addTab(searchTabWidget, tr("Search-Old"));
    dxUpRightTab->addTab(searchWidget, tr("Search"));

    //QWidget *logTabWidget = new QWidget;
    //QWidget *dxClusterTabWidget = new QWidget;



    //QHBoxLayout *dxClusterTabWidgetLayout = new QHBoxLayout;

    //QHBoxLayout *logTabWidgetLayout = new QHBoxLayout;
    //logTabWidgetLayout->addWidget(logWindow);//
    //logTabWidget->setLayout(logTabWidgetLayout);

    //dxClusterTabWidgetLayout->addWidget(dxClusterWidget);
    //dxClusterTabWidget->setLayout(dxClusterTabWidgetLayout);

    //QWidget *logWTabWidget = new QWidget;
    //QHBoxLayout *logWTabWidgetLayout = new QHBoxLayout;
    //logWTabWidgetLayout->addWidget(logWindow);
    //logWTabWidget->setLayout(logWTabWidgetLayout);

    dxBottonTab->addTab(logWindow, tr("Log"));
    //dxBottonTab->addTab(logTabWidget, tr("Log-old"));
    dxBottonTab->addTab(dxClusterWidget, tr("DX-Cluster"));
    dxBottonTab->addTab(dxccStatusWidget, tr("DXCC"));



    QVBoxLayout *dxUpRightLayout = new QVBoxLayout;
    dxUpRightLayout->addLayout(dxUpRightFixLayout);
    dxUpRightLayout->addWidget(dxUpRightTab);

    QWidget *upRightWidget = new QWidget;
    upRightWidget->setLayout(dxUpRightLayout);

    //QWidget *upLeftWidget = new QWidget;
    //upLeftWidget->setLayout(dxUpLeftLayout);

    ////////////
    QSplitter *upSplitter = new QSplitter (this);
    //upSplitter->addWidget(upLeftWidget);
    upSplitter->addWidget(upLeftSplitter);
    upSplitter->addWidget(upRightWidget);


    //QHBoxLayout *upperLayout = new QHBoxLayout;
    //upperLayout->addLayout(dxUpLeftLayout);
    //upperLayout->addLayout(dxUpRightLayout);

    //QWidget *upper = new QWidget;
    //upper->setLayout(upperLayout);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(upSplitter);
    splitter->addWidget(dxBottonTab);
    splitter->setOrientation(Qt::Vertical);

    QHBoxLayout *mLayout = new QHBoxLayout;
    mLayout->addWidget(splitter);

    mainWidget->setLayout(mLayout);

    //layout->addLayout(dxUpLeftLayout, 0, 0);
    //layout->addLayout(dxUpRightLayout, 0, 1);
    //layout->addWidget(dxBottonTab, 1, 0, 1, 2);

    //mainWidget->setLayout(layout);

    // Disabled widgets to be enabled as functionality is deployed
    //iotaContinentComboBox->setEnabled(false);
    //iotaNumberLineEdit->setEnabled(false);
    //myPowerSpinBox->setEnabled(false);

    //locatorLineEdit->setEnabled(false);
    //eqslSentComboBox->setEnabled(false);
    //eqslRecComboBox->setEnabled(false);
    //lotwSentComboBox->setEnabled(false);
    //lotwRecComboBox->setEnabled(false);

    //eqslSentQDateEdit->setEnabled(false);
    //eqslRecQDateEdit->setEnabled(false);
    //lotwSentQDateEdit->setEnabled(false);
    //lotwRecQDateEdit->setEnabled(false);

    //qslSentCheckBox->setEnabled(false);
    //qslRecCheckBox->setEnabled(false);
    //qslSentQDateEdit->setEnabled(false);
    //qslRecQDateEdit->setEnabled(false);

    //qslSentViaComboBox->setEnabled(false);
    //qslRecViaComboBox->setEnabled(false);
    //qslmsgTextEdit->setEnabled(false);
    //qslViaLineEdit->setEnabled(false);
    //entityPrimDivComboBox->setEnabled(false);
    //entitySecDivComboBox->setEnabled(false);
    //entityNameComboBox->setEnabled(true);
    //propModeComboBox->setEnabled(true);

//qDebug() << "MainWindow::createUIDX - OS DETECTION"  << endl;

#ifdef Q_OS_WIN
    //qDebug() << "MainWindow::createUIDX - WINDOWS DETECTED!"  << endl;

    //txFreqSpinBox->setFrame(true);
    //txFreqSpinBox->setFrameShadow(QFrame::Raised);
    //txFreqSpinBox->setFrameStyle(QFrame::StyledPanel);
    //rxFreqSpinBox->setFrameShadow(QFrame::Raised);
    //rxFreqSpinBox->setFrameStyle(QFrame::StyledPanel);

    dxUpLeftInputFrame->setFrameShadow(QFrame::Raised);
    dxUpLeftInputFrame->setFrameStyle(QFrame::StyledPanel);

    qsoNLabelN->setFrameShadow(QFrame::Raised);
    qsoNLabelN->setFrameStyle(QFrame::StyledPanel);
    wazLabelN->setFrameShadow(QFrame::Raised);
    wazLabelN->setFrameStyle(QFrame::StyledPanel);
    localLabelN->setFrameShadow(QFrame::Raised);
    localLabelN->setFrameStyle(QFrame::StyledPanel);
    dxccLabelN->setFrameShadow(QFrame::Raised);
    dxccLabelN->setFrameStyle(QFrame::StyledPanel);
    workedLabelN->setFrameShadow(QFrame::Raised);
    workedLabelN->setFrameStyle(QFrame::StyledPanel);
    confirmedLabelN->setFrameShadow(QFrame::Raised);
    confirmedLabelN->setFrameStyle(QFrame::StyledPanel);
    awardLabelN->setFrameShadow(QFrame::Raised);
    awardLabelN->setFrameStyle(QFrame::StyledPanel);

    dxMarathonTopDXCCLabelN->setFrameShadow(QFrame::Raised);
    dxMarathonTopCQLabelN->setFrameShadow(QFrame::Raised);
    dxMarathonTopScoreLabelN->setFrameShadow(QFrame::Raised);
    dxMarathonLabelN->setFrameShadow(QFrame::Raised);
    dxMarathonTopDXCCLabelN->setFrameStyle(QFrame::StyledPanel);
    dxMarathonTopCQLabelN->setFrameStyle(QFrame::StyledPanel);
    dxMarathonTopScoreLabelN->setFrameStyle(QFrame::StyledPanel);
    dxMarathonLabelN->setFrameStyle(QFrame::StyledPanel);
/*
    continentLabel->setFrameShadow(QFrame::Raised);
    continentLabel->setFrameStyle(QFrame::StyledPanel);
    continentLabelN->setFrameShadow(QFrame::Raised);
    continentLabelN->setFrameStyle(QFrame::StyledPanel);
    prefixLabelN->setFrameShadow(QFrame::Raised);
    prefixLabelN->setFrameStyle(QFrame::StyledPanel);
    prefixLabel->setFrameShadow(QFrame::Raised);
    prefixLabel->setFrameStyle(QFrame::StyledPanel);
    cqzLabelN->setFrameShadow(QFrame::Raised);
    cqzLabelN->setFrameStyle(QFrame::StyledPanel);
    cqzLabel->setFrameShadow(QFrame::Raised);
    cqzLabel->setFrameStyle(QFrame::StyledPanel);
    ituzLabel->setFrameShadow(QFrame::Raised);
    ituzLabel->setFrameStyle(QFrame::StyledPanel);
    ituzLabelN->setFrameShadow(QFrame::Raised);
    ituzLabelN->setFrameStyle(QFrame::StyledPanel);    
    shortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    shortLabelN->setFrameShadow(QFrame::Raised);
    shortLabelN->setFrameStyle(QFrame::StyledPanel);

    longLabelN->setFrameShadow(QFrame::Raised);
    longLabelN->setFrameStyle(QFrame::StyledPanel);
    gradShortLabelN->setFrameShadow(QFrame::Raised);
    gradShortLabelN->setFrameStyle(QFrame::StyledPanel);
    gradShortLabel->setFrameShadow(QFrame::Raised);
    gradShortLabel->setFrameStyle(QFrame::StyledPanel);
    distShortLabelN->setFrameShadow(QFrame::Raised);
    distShortLabelN->setFrameStyle(QFrame::StyledPanel);
    distShortLabel->setFrameShadow(QFrame::Raised);
    distShortLabel->setFrameStyle(QFrame::StyledPanel);
    gradLongLabelN->setFrameShadow(QFrame::Raised);
    gradLongLabelN->setFrameStyle(QFrame::StyledPanel);
    gradLongLabel->setFrameShadow(QFrame::Raised);
    gradLongLabel->setFrameStyle(QFrame::StyledPanel);
    distLongLabelN->setFrameShadow(QFrame::Raised);
    distLongLabelN->setFrameStyle(QFrame::StyledPanel);
    distLongLabel->setFrameShadow(QFrame::Raised);
    distLongLabel->setFrameStyle(QFrame::StyledPanel);


    bandLabel1->setFrameShadow(QFrame::Raised);
    bandLabel1->setFrameStyle(QFrame::StyledPanel);
    bandLabel2->setFrameShadow(QFrame::Raised);
    bandLabel2->setFrameStyle(QFrame::StyledPanel);
    bandLabel3->setFrameShadow(QFrame::Raised);
    bandLabel3->setFrameStyle(QFrame::StyledPanel);
    bandLabel4->setFrameShadow(QFrame::Raised);
    bandLabel4->setFrameStyle(QFrame::StyledPanel);
    bandLabel5->setFrameShadow(QFrame::Raised);
    bandLabel5->setFrameStyle(QFrame::StyledPanel);
    bandLabel6->setFrameShadow(QFrame::Raised);
    bandLabel6->setFrameStyle(QFrame::StyledPanel);

    bandLabel7->setFrameShadow(QFrame::Raised);
    bandLabel7->setFrameStyle(QFrame::StyledPanel);
    bandLabel8->setFrameShadow(QFrame::Raised);
    bandLabel8->setFrameStyle(QFrame::StyledPanel);
    bandLabel9->setFrameShadow(QFrame::Raised);
    bandLabel9->setFrameStyle(QFrame::StyledPanel);
    bandLabel10->setFrameShadow(QFrame::Raised);
    bandLabel10->setFrameStyle(QFrame::StyledPanel);
    bandLabel11->setFrameShadow(QFrame::Raised);
    bandLabel11->setFrameStyle(QFrame::StyledPanel);
    bandLabel12->setFrameShadow(QFrame::Raised);
    bandLabel12->setFrameStyle(QFrame::StyledPanel);
*/
    infoLabel1->setFrameShadow(QFrame::Raised);
    infoLabel1->setFrameStyle(QFrame::StyledPanel);
    infoLabel2->setFrameShadow(QFrame::Raised);
    infoLabel2->setFrameStyle(QFrame::StyledPanel);
/*
    QSLViaLabelN->setFrameShadow(QFrame::Raised);
    QSLViaLabelN->setFrameStyle(QFrame::StyledPanel);
    QSLSentLabelN->setFrameShadow(QFrame::Raised);
    QSLSentLabelN->setFrameStyle(QFrame::StyledPanel);
    QSLRecLabelN->setFrameShadow(QFrame::Raised);
    QSLRecLabelN->setFrameStyle(QFrame::StyledPanel);
    eQSLSentLabelN->setFrameShadow(QFrame::Raised);
    eQSLSentLabelN->setFrameStyle(QFrame::StyledPanel);
    eQSLRecLabelN->setFrameShadow(QFrame::Raised);
    eQSLRecLabelN->setFrameStyle(QFrame::StyledPanel);
    lotWSentLabelN->setFrameShadow(QFrame::Raised);
    lotWSentLabelN->setFrameStyle(QFrame::StyledPanel);
    myPowerSpinBoxLabelN->setFrameShadow(QFrame::Raised);
    myPowerSpinBoxLabelN->setFrameStyle(QFrame::StyledPanel);
    rxPowerSpinBoxLabelN->setFrameShadow(QFrame::Raised);
    rxPowerSpinBoxLabelN->setFrameStyle(QFrame::StyledPanel);
    operatorLabelN->setFrameShadow(QFrame::Raised);
    operatorLabelN->setFrameStyle(QFrame::StyledPanel);
    stationCallSignLabelN->setFrameShadow(QFrame::Raised);
    stationCallSignLabelN->setFrameStyle(QFrame::StyledPanel);
    myLocatorLabelN->setFrameShadow(QFrame::Raised);
    myLocatorLabelN->setFrameStyle(QFrame::StyledPanel);
    lotWRecLabelN->setFrameShadow(QFrame::Raised);
    lotWRecLabelN->setFrameStyle(QFrame::StyledPanel);
*/
    //nameLabel->setFrameShape(QFrame::StyledPanel);

    //nameLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    //nameLineEdit->setFrameShadow(QFrame::Raised);
    //nameLineEdit->setFrameStyle(QFrame::StyledPanel);
    //qthLabel->setFrameShape(QFrame::Box);
    //qthLabel->setFrameShadow(QFrame::Raised);
    //qthLabel->setFrameStyle(QFrame::StyledPanel);

    //qthLineEdit->setFrameShadow(QFrame::Raised);
    //qthLineEdit->setFrameStyle(QFrame::StyledPanel);

    //locLabel->setFrameShadow(QFrame::Raised);
    //locLabel->setFrameStyle(QFrame::StyledPanel);

    //locatorLineEdit->setFrameShadow(QFrame::Raised);
    //locatorLineEdit->setFrameStyle(QFrame::StyledPanel);

    //rxPowerLayout->setFrameShadow(QFrame::Raised);
    //rxPowerLayout->setFrameStyle(QFrame::StyledPanel);

    //txfreqLabelN->setFrameShadow(QFrame::Raised);
    //txfreqLabelN->setFrameStyle(QFrame::StyledPanel);
    //rxfreqLabelN->setFrameShadow(QFrame::Raised);
    //rxfreqLabelN->setFrameStyle(QFrame::StyledPanel);



    //freqLabelN->setFrameShadow(QFrame::Raised);
    //freqLabelN->setFrameStyle(QFrame::StyledPanel);
/*
    nameLabel->setFrameShadow(QFrame::Raised);
    nameLabel->setFrameStyle(QFrame::StyledPanel);
    qthLabel->setFrameShadow(QFrame::Raised);
    qthLabel->setFrameStyle(QFrame::StyledPanel);
    locLabel->setFrameShadow(QFrame::Raised);
    locLabel->setFrameStyle(QFrame::StyledPanel);
    rstRxLabelN->setFrameShadow(QFrame::Raised);
    rstRxLabelN->setFrameStyle(QFrame::StyledPanel);
    rstTxLabelN->setFrameShadow(QFrame::Raised);
    rstTxLabelN->setFrameStyle(QFrame::StyledPanel);
*/


#else
    //qDebug() << "MainWindow::createUIDX - NO WINDOWS DETECTED!"  << endl;

    dxUpLeftInputFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    localLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    qsoNLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    wazLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxccLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    workedLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    confirmedLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    awardLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxMarathonTopDXCCLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxMarathonTopCQLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxMarathonTopScoreLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    dxMarathonLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    /*
    continentLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    continentLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    prefixLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    prefixLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    cqzLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    cqzLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    ituzLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    ituzLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    shortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    longLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradShortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradShortLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distShortLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distShortLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradLongLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distLongLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    distLongLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    gradLongLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


    bandLabel1->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel3->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel4->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel5->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel6->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel7->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel8->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel9->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel10->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel11->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    bandLabel12->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
*/
    infoLabel1->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    infoLabel2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


    //QSLViaLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //QSLSentLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //QSLRecLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //eQSLSentLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //eQSLRecLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //lotWSentLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //myPowerSpinBoxLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rxPowerSpinBoxLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //operatorLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //stationCallSignLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //myLocatorLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //lotWRecLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    //nameLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //nameLineEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //qthLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //qthLineEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //locLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //locatorLineEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rxPowerLayout->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


    //txfreqLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rxfreqLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rstRxLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //rstTxLabelN->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //nameLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //qthLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    //locLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);


#endif


 }

void MainWindow::slotADIFExport(){
    //qDebug() << "MainWindow::slotADIFExport " << endl;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               util->getHomeDir(),
                               tr("ADIF (*.adi *.adif)"));

    filemanager->adifLogExport(fileName, currentLog);

}

void MainWindow::slotADIFExportAll(){
    //qDebug() << "MainWindow::slotADIFExportAll " << endl;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               util->getHomeDir(),
                               tr("ADIF (*.adi *.adif)"));

    filemanager->adifLogExport(fileName, 0);

}

void MainWindow::slotRQSLExport()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ADIF File"),
                               util->getHomeDir(),
                               tr("ADIF (*.adi *.adif)"));


    filemanager->adifReqQSLExport(fileName);

}

void MainWindow::slotCabrilloExport()
{
    //qDebug() << "MainWindow::slotCabrilloExport " << endl;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Cabrillo File"),
                               util->getHomeDir(),
                               tr("Cabrillo (*.log)"));


    contest->saveFileToSend(fileName);

}

void MainWindow::slotADIFImport(){
   //qDebug() << "MainWindow::slotADIFImport " << endl;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     util->getHomeDir(),
                                                     tr("ADIF (*.adi *.adif)"));
    if (fileName.isNull())
    {
    }
    else
    {
       //qDebug() << "MainWindow::slotADIFImport -1" << endl;
        filemanager->adifReadLog(fileName, currentLog);
       //qDebug() << "MainWindow::slotADIFImport -2" << endl;

        logWindow->refresh();
   //qDebug() << "MainWindow::slotADIFImport -3" << endl;
        checkIfNewBandOrMode();
   //qDebug() << "MainWindow::slotADIFImport -4" << endl;

        switch (contestMode) {

            case CQ_WW_SSB:

            break;
            case CQ_WW_CW:

            break;
            default:

                operatingYearsComboBox->addItems(dataProxy->getOperatingYears(currentLog));                

//bandComboBox->setCurrentIndex(bandComboBox->findText(aux1));
   //qDebug() << "MainWindow::slotADIFImport -31" << endl;
                //awards->recalculateAwards();
               //qDebug() << "MainWindow::slotADIFImport -32" << endl;
                //showAwards();
               //qDebug() << "MainWindow::slotADIFImport -33" << endl;
                //dxccStatusWidget->setBands(bands);
               //qDebug() << "MainWindow::slotADIFImport -34" << endl;
                //dxccStatusWidget->refresh();
               //qDebug() << "MainWindow::slotADIFImport -35" << endl;
                slotShowAwards();
            break;

        }
       //qDebug() << "MainWindow::slotADIFImport-7" << endl;
    }
   //qDebug() << "MainWindow::slotADIFImport-END" << endl;
}

void  MainWindow::initialContestModeConfiguration()
{

   //qDebug() << "MainWindow::initialContestModeConfiguration: - 0" << endl;

     if (!configured){
         //qDebug() << "MainWindow::initialContestModeConfiguration: - 01" << endl;
        slotSetup();
        //qDebug() << "MainWindow::initialContestModeConfiguration: - 02" << endl;
        return;
    }
     //qDebug() << "MainWindow::initialContestModeConfiguration: - 03" << endl;
    QSqlQuery query;
    QStringList contestQS;

     /*

    QSqlQuery query("SELECT id FROM log");
    QSqlRecord rec = query.record();
    int nameCol;

    while (query.next()) {
        nameCol = rec.indexOf("call");
*/
    //qDebug() << "MainWindow::initialContestModeConfiguration: - 04" << endl;
    switch (contestMode) {

        case CQ_WW_SSB:
        //qDebug() << "MainWindow::initialContestModeConfiguration: - 05" << endl;
            defaultMode = dataProxy->getIdFromModeName("SSB");
            SRXLineEdit->setInputMask("09");
            STXLineEdit->setInputMask("09");

            contestQS << QString::number(world->getQRZARRLId(stationQRZ)) << QString::number(world->getQRZCqz(stationQRZ)) << world->getQRZContinentNumber(stationQRZ) <<  world->getQRZContinentNumber("K");
            contest = new ContestCQWWDXSSB(contestQS);

        break;
        case CQ_WW_CW:
        //qDebug() << "MainWindow::initialContestModeConfiguration: - 06" << endl;
            SRXLineEdit->setInputMask("09");
            STXLineEdit->setInputMask("09");


            contestQS << QString::number(world->getQRZARRLId(stationQRZ)) << QString::number(world->getQRZCqz(stationQRZ)) << world->getQRZContinentNumber(stationQRZ) <<  world->getQRZContinentNumber("K");
            //contest = new ContestCQWWDXSSB(contestQS);

        break;
        default:
            //qDebug() << "MainWindow::initialContestModeConfiguration: - 07" << endl;
        break;
    }


}

void MainWindow::qsoToEdit (const int _qso)
{
   //qDebug() << "MainWindow::qsoToEdit: " << QString::number(_qso) << endl;


    int nameCol;
    QString aux1;
    double testValueDouble; // Variable just to test if the value is in the appropriate range

    modify = true;
    modifyingQSO = _qso;
    OKButton->setText(tr("&Modify"));

    //TODO: Optimize the following query. Maybe the * is not needed.
    QString stringQuery = QString("SELECT * FROM log WHERE id ='%1' AND lognumber='%2'").arg(_qso).arg(currentLog);
    //qDebug() << "MainWindow::qsoToEdit: " << stringQuery << endl;


    QSqlQuery query(stringQuery);

    query.next();
    if (query.isValid())
    {
        if ((clublogActive) && (clublogRealTime))
        {
            clublogPrevQSO = dataProxy->getClubLogRealTimeFromId(_qso);
        }

        QSqlRecord rec = query.record();


    // ADD THE DATA THAT IS PRESENT IN ALL THE MODES

    nameCol = rec.indexOf("call");
    aux1 = (query.value(nameCol)).toString();
    qrzLineEdit->setText(aux1);
    currentQrz = aux1;
    currentEntity = world->getQRZARRLId(currentQrz);

   //qDebug() << "MainWindow::qsoToEdit - currentEntity " << QString::number(currentEntity) << endl;

    nameCol = rec.indexOf("qso_date");
    aux1 = (query.value(nameCol)).toString();
    dateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));

    nameCol = rec.indexOf("time_on");
    aux1 = (query.value(nameCol)).toString();
    timeEdit->setTime(QTime::fromString(aux1, "hh:mm:ss"));

    nameCol = rec.indexOf("bandid");
    aux1 = (query.value(nameCol)).toString();
    stringQuery = QString("SELECT name FROM band WHERE id ='%1'").arg(aux1);
    QSqlQuery queryAux(stringQuery);
    queryAux.next();
    if (queryAux.isValid())
    {
        aux1 = (queryAux.value(0)).toString();
        bandComboBox->setCurrentIndex(bandComboBox->findText(aux1));
    }
    else
    {
        bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand)));
        //bandComboBox->setCurrentIndex(defaultBand);
    }

    nameCol = rec.indexOf("modeid");
    aux1 = (query.value(nameCol)).toString();

    stringQuery = QString("SELECT submode FROM mode WHERE id ='%1'").arg(aux1);
    queryAux.exec(stringQuery);
    queryAux.next();
    if (queryAux.isValid())
    {
        aux1 = (queryAux.value(0)).toString();
        modeComboBox->setCurrentIndex(modeComboBox->findText(aux1));
    }
    else
    {
        modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(defaultMode)));
        //modeComboBox->setCurrentIndex(defaultMode);
    }

    nameCol = rec.indexOf("rst_sent");
    aux1 = (query.value(nameCol)).toString();
    rstTXLineEdit->setText(aux1);
    //qDebug() << "MainWindow::qsoToEdit: - RST_SENT: " << aux1  << endl;

    nameCol = rec.indexOf("rst_rcvd");
    aux1 = (query.value(nameCol)).toString();
    rstRXLineEdit->setText(aux1);

    //qDebug() << "MainWindow::qsoToEdit: - before switch"  << endl;

    switch (contestMode) {

        case CQ_WW_SSB:


        break;
        case CQ_WW_CW:

        break;
        default:

        //qDebug() << "MainWindow::qsoToEdit: - in default"  << endl;

        nameCol = rec.indexOf("qsl_via");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLVia(aux1);
        /*
        if (aux1.length()>0)
        {

            //qslViaLineEdit->setText(aux1);
        }
        else
        {
            qslViaLineEdit->clear();
        }
        */
        nameCol = rec.indexOf("qslmsg");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLMsg(aux1);

        //qslmsgTextEdit->setText(aux1);

        nameCol = rec.indexOf("comment");
        aux1 = (query.value(nameCol)).toString();
        if (aux1.length()>0)
        {
            commentTabWidget->setData(aux1);
            //commentLineEdit->setText(aux1);
        }
        else
        {
            commentTabWidget->clear();
            //commentLineEdit->clear();
        }


        //nameCol = rec.indexOf("notes");
        //aux1 = (query.value(nameCol)).toString();
        //notesTextEdit->setText(aux1);

        nameCol = rec.indexOf("name");
        aux1 = (query.value(nameCol)).toString();
        //qDebug() << "MainWindow::qsoToEdit: - NAME: " << aux1  << endl;
        if (aux1.length()>0)
        {
            nameLineEdit->setText(aux1);
        }
        else
        {
            nameLineEdit->clear();
        }

        nameCol = rec.indexOf("qth");
        aux1 = (query.value(nameCol)).toString();
        qthLineEdit->setText(aux1);

        nameCol = rec.indexOf("gridsquare");
        aux1 = (query.value(nameCol)).toString();
        locatorLineEdit->setText(aux1);

        nameCol = rec.indexOf("operator");
        aux1 = (query.value(nameCol)).toString();      
        //operatorLineEdit->setText(aux1.toUpper());
        myDataTabWidget->setOperator(aux1);

        nameCol = rec.indexOf("station_callsign");
        aux1 = (query.value(nameCol)).toString();        
        //stationCallSignLineEdit->setText(aux1.toUpper());
        myDataTabWidget->setStationQRZ(aux1);

        nameCol = rec.indexOf("my_gridsquare");
        aux1 = (query.value(nameCol)).toString();
        //myLocatorLineEdit->setText(aux1);
        myDataTabWidget->setMyLocator(aux1);

        nameCol = rec.indexOf("tx_pwr");        
        //aux1 = (query.value(nameCol)).toString();
        //testValueDouble = aux1.toDouble();
        myDataTabWidget->setMyPower((query.value(nameCol)).toDouble());

/*
        if (testValueDouble >=0)
        {
            myPowerSpinBox->setValue(testValueDouble);
        }
        else
        {
            myPowerSpinBox->setValue(0.0);
        }
*/
        nameCol = rec.indexOf("rx_pwr");
        aux1 = (query.value(nameCol)).toString();
        testValueDouble = aux1.toDouble();
        if (testValueDouble >=0)
        {
            rxPowerSpinBox->setValue(testValueDouble);
        }
        else
        {
            rxPowerSpinBox->setValue(0.0);
        }

        nameCol = rec.indexOf("freq");
        aux1 = (query.value(nameCol)).toString();
        //qDebug() << "MainWindow::qsoToEdit (freq STRING):"  << aux1 << endl;
        testValueDouble = aux1.toDouble();
        //qDebug() << "MainWindow::qsoToEdit (freq):"  << QString::number(testValueDouble) << endl;

        if ((testValueDouble >=0) && (testValueDouble <= txFreqSpinBox->maximum()) )
        {
            txFreqSpinBox->setValue(testValueDouble);
            //qDebug() << "MainWindow::qsoToEdit: Freq - OverFlow "  << endl;

            //freqQLCDNumber->display(testValueDouble);
        }
        else
        {
            //qDebug() << "MainWindow::qsoToEdit: Freq - OK "  << endl;
            txFreqSpinBox->setValue(0);
        }


        nameCol = rec.indexOf("freq_rx");
        aux1 = (query.value(nameCol)).toString();
        //qDebug() << "MainWindow::qsoToEdit (freq_rx STRING):"  << aux1 << endl;
        testValueDouble = aux1.toDouble();
        //qDebug() << "MainWindow::qsoToEdit (freq_rx):"  << QString::number(testValueDouble) << endl;

        if ((testValueDouble >=0) && (testValueDouble <= rxFreqSpinBox->maximum()) )
        {
            rxFreqSpinBox->setValue(testValueDouble);
            //qDebug() << "MainWindow::qsoToEdit: Freq_RX - OverFlow "  << endl;

            //freqQLCDNumber->display(testValueDouble);
        }
        else
        {
            //qDebug() << "MainWindow::qsoToEdit: Freq_RX - OK "  << endl;
            rxFreqSpinBox->setValue(0);
        }

       // myPowerSpinBox->setValue(aux1.toDouble());

        //QSL SENT

        nameCol = rec.indexOf("qsl_sent");
        aux1 = (query.value(nameCol)).toString();

        QSLTabWidget->setQSLSenStatus(aux1);
        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("qslsdate");
        aux1 = (query.value(nameCol)).toString();
        if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            QSLTabWidget->setQSLSenDate(QDate::fromString(aux1, "yyyy/MM/dd"));
        }


        nameCol = rec.indexOf("qsl_sent_via");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLSenVia(aux1);
/*
        if (  (aux1.toUpper()) == "B" )
        {
             qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("B-Bureau"))) ;

        }
        else if (  (aux1.toUpper()) == "D" ){
            qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("D-Direct"))) ;
        }
        else if (  (aux1.toUpper()) == "E" ){
            qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("E-Electronic"))) ;
        }
        else if (  (aux1.toUpper()) == "M" ){
            qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("M-Manager"))) ;
        }
        else
        {
            qslSentViaComboBox->setCurrentIndex( qslSentViaComboBox->findText(tr("B-Bureau"))) ;
        }
*/
    //QSL RECEPTION

        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
        // tr("B-Bureau") << tr("D-Direct") << tr("E-Electronic") << tr("M-Manager");
        //QSLRDATE: (only valid if QSL_RCVD is Y, I, or V)


        nameCol = rec.indexOf("qsl_rcvd");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLRecStatus(aux1);
        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("qslrdate");
        aux1 = (query.value(nameCol)).toString();
        if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            QSLTabWidget->setQSLRecDate(QDate::fromString(aux1, "yyyy/MM/dd"));
        }

 /*

        if (  (aux1.toUpper()) == "Y" )
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("Y-Yes")) );

            nameCol = rec.indexOf("qslrdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                qslRecQDateEdit->setEnabled(true);
                qslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }

        else if ( (aux1.toUpper()) == "N")
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("N-No") ) );
        }

        else if ((aux1.toUpper()) == "R")
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("R-Requested") ) );
        }

        else if ((aux1.toUpper()) =="I")
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("I-Ignore")) );

            nameCol = rec.indexOf("qslrdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                qslRecQDateEdit->setEnabled(true);
                qslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }

        }

        else if ((aux1.toUpper()) == "V")
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("V-Verified") ) );

            nameCol = rec.indexOf("qslrdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                qslRecQDateEdit->setEnabled(true);
                qslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }


        else
        {
            qslRecComboBox->setCurrentIndex( qslRecComboBox->findText(tr("N-No") ) );
        }
*/
        nameCol = rec.indexOf("qsl_rcvd_via");
        aux1 = (query.value(nameCol)).toString();
        QSLTabWidget->setQSLRecVia(aux1);

 /*

        if (  (aux1.toUpper()) == "B" )
        {
             qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("B-Bureau"))) ;

        }
        else if (  (aux1.toUpper()) == "D" ){
            qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("D-Direct"))) ;
        }
        else if (  (aux1.toUpper()) == "E" ){
            qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("E-Electronic"))) ;
        }
        else if (  (aux1.toUpper()) == "M" ){
            qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("M-Manager"))) ;
        }
        else
        {
            qslRecViaComboBox->setCurrentIndex( qslRecViaComboBox->findText(tr("B-Bureau"))) ;
        }

*/
    //TODO: BUG: When something is selected while modifying the QSL is deleted???

        //CLUBLOG
        nameCol = rec.indexOf("clublog_qso_upload_status");
        aux1 = (query.value(nameCol)).toString();
        eQSLTabWidget->setClubLogStatus(aux1.toUpper());

        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("clublog_qso_upload_date");
        aux1 = (query.value(nameCol)).toString();
        if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            eQSLTabWidget->setClubLogDate((QDate::fromString(aux1, "yyyy/MM/dd")));
        }



/*

        if (  (aux1.toUpper()) == "Y" )
        {
            clublogComboBox->setCurrentIndex( clublogComboBox->findText(tr("Y-Uploaded")) );
            nameCol = rec.indexOf("clublog_qso_upload_date");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                clublogQDateEdit->setEnabled(true);
                clublogQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }

        else if ( (aux1.toUpper()) == "N")
        {
            clublogComboBox->setCurrentIndex( clublogComboBox->findText(tr("N-Do not upload")) );
        }
        else if ( (aux1.toUpper()) == "M")
        {
            clublogComboBox->setCurrentIndex( clublogComboBox->findText(tr("M-Modified")) );

            nameCol = rec.indexOf("clublog_qso_upload_date");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                clublogQDateEdit->setEnabled(true);
                clublogQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }
        else
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("N-No") ) );
        }

*/
        //CLUBLOG


        //EQSL_QSL_SENT: {Y, N, R, Q, I}
        // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
       //EQSL_QSLSDATE (only valid if EQSL_SENT is Y, Q, or I)

        nameCol = rec.indexOf("eqsl_qsl_sent");
        aux1 = (query.value(nameCol)).toString();
        eQSLTabWidget->setEQSLSenStatus(aux1.toUpper());

        //TODO: Depending on the Value a date should or not exist.
        //      This code may be importing dates when they should not exist.
        nameCol = rec.indexOf("eqsl_qslsdate");
        aux1 = (query.value(nameCol)).toString();
        if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
        {
            eQSLTabWidget->setEQSLSenDate(QDate::fromString(aux1, "yyyy/MM/dd"));
        }
/*
        if (  (aux1.toUpper()) == "Y" )
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("Y-Yes")) );

            nameCol = rec.indexOf("eqsl_qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                eqslSentQDateEdit->setEnabled(true);
                eqslSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }

        else if ( (aux1.toUpper()) == "N")
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("N-No") ) );
        }

        else if ((aux1.toUpper()) == "R")
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("R-Requested") ) );
        }

        else if ((aux1.toUpper()) =="Q")
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("Q-Queued")) );

            nameCol = rec.indexOf("eqsl_qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                eqslSentQDateEdit->setEnabled(true);
                eqslSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }

        }

        else if ((aux1.toUpper()) == "I")
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("I-Ignore") ) );

            nameCol = rec.indexOf("eqsl_qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                eqslSentQDateEdit->setEnabled(true);
                eqslSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }
        }


        else
        {
            eqslSentComboBox->setCurrentIndex( eqslSentComboBox->findText(tr("N-No") ) );
        }
*/

        //E-QSL RECEPTION

            // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
            // EQSL_QSL_RCVD: {Y, N, R, I, V}
            // EQSL_QSLRDATE: (only valid if EQSL_RCVD is Y, I, or V)


            nameCol = rec.indexOf("eqsl_qsl_rcvd");
            aux1 = (query.value(nameCol)).toString();
            eQSLTabWidget->setEQSLRecStatus(aux1.toUpper());

            //TODO: Depending on the Value a date should or not exist.
            //      This code may be importing dates when they should not exist.
            nameCol = rec.indexOf("eqsl_qslrdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid() )
            {
                eQSLTabWidget->setEQSLRecDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }

/*
            if (  (aux1.toUpper()) == "Y" )
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("Y-Yes")) );

                nameCol = rec.indexOf("eqsl_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    eqslRecQDateEdit->setEnabled(true);
                    eqslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }
            }

            else if ( (aux1.toUpper()) == "N")
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("N-No") ) );
            }

            else if ((aux1.toUpper()) == "R")
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("R-Requested") ) );
            }

            else if ((aux1.toUpper()) =="I")
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("I-Ignore")) );

                nameCol = rec.indexOf("eqsl_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    eqslRecQDateEdit->setEnabled(true);
                    eqslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }

            }

            else if ((aux1.toUpper()) == "V")
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("V-Verified") ) );

                nameCol = rec.indexOf("eqsl_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    eqslRecQDateEdit->setEnabled(true);
                    eqslRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }
            }


            else
            {
                eqslRecComboBox->setCurrentIndex( eqslRecComboBox->findText(tr("N-No") ) );
            }
*/
            //LOTW_QSL_SENT: {Y, N, R, Q, I}
            // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("Q-Queued") << tr("I-Ignore");
           //LOTW_QSLSDATE (only valid if LOTW_SENT is Y, Q, or I)

            nameCol = rec.indexOf("lotw_qsl_sent");
            aux1 = (query.value(nameCol)).toString();
            eQSLTabWidget->setLOTWSenStatus(aux1.toUpper());

            //TODO: Depending on the Value a date should or not exist.
            //      This code may be importing dates when they should not exist.
            nameCol = rec.indexOf("lotw_qslsdate");
            aux1 = (query.value(nameCol)).toString();
            if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
            {
                eQSLTabWidget->setLOTWSenDate(QDate::fromString(aux1, "yyyy/MM/dd"));
            }

/*

            if (  (aux1.toUpper()) == "Y" )
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("Y-Yes")) );

                nameCol = rec.indexOf("lotw_qslsdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    lotwSentQDateEdit->setEnabled(true);
                    lotwSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }
            }

            else if ( (aux1.toUpper()) == "N")
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("N-No") ) );
            }

            else if ((aux1.toUpper()) == "R")
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("R-Requested") ) );
            }

            else if ((aux1.toUpper()) =="Q")
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("Q-Queued")) );

                nameCol = rec.indexOf("lotw_qslsdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    lotwSentQDateEdit->setEnabled(true);
                    lotwSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }

            }

            else if ((aux1.toUpper()) == "I")
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("I-Ignore") ) );

                nameCol = rec.indexOf("lotw_qslsdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    lotwSentQDateEdit->setEnabled(true);
                    lotwSentQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }
            }


            else
            {
                lotwSentComboBox->setCurrentIndex( lotwSentComboBox->findText(tr("N-No") ) );
            }

*/
            //E-QSL RECEPTION

                // tr("Y-Yes") << tr("N-No") << tr("R-Requested") << tr("I-Ignore") << tr("V-Verified");
                // lotw_QSL_RCVD: {Y, N, R, I, V}
                // lotw_QSLRDATE: (only valid if lotw_RCVD is Y, I, or V)


                nameCol = rec.indexOf("lotw_qsl_rcvd");
                aux1 = (query.value(nameCol)).toString();
                eQSLTabWidget->setLOTWRecStatus(aux1.toUpper());

                //TODO: Depending on the Value a date should or not exist.
                //      This code may be importing dates when they should not exist.
                nameCol = rec.indexOf("lotw_qslrdate");
                aux1 = (query.value(nameCol)).toString();
                if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                {
                    eQSLTabWidget->setLOTWRecDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                }

/*

                if (  (aux1.toUpper()) == "Y" )
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("Y-Yes")) );

                    nameCol = rec.indexOf("lotw_qslrdate");
                    aux1 = (query.value(nameCol)).toString();
                    if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                    {
                        lotwRecQDateEdit->setEnabled(true);
                        lotwRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                    }
                }

                else if ( (aux1.toUpper()) == "N")
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("N-No") ) );
                }

                else if ((aux1.toUpper()) == "R")
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("R-Requested") ) );
                }

                else if ((aux1.toUpper()) =="I")
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("I-Ignore")) );

                    nameCol = rec.indexOf("lotw_qslrdate");
                    aux1 = (query.value(nameCol)).toString();
                    if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                    {
                        lotwRecQDateEdit->setEnabled(true);
                        lotwRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                    }

                }

                else if ((aux1.toUpper()) == "V")
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("V-Verified") ) );

                    nameCol = rec.indexOf("lotw_qslrdate");
                    aux1 = (query.value(nameCol)).toString();
                    if (  (QDate::fromString(aux1, "yyyy/MM/dd")).isValid()  )
                    {
                        lotwRecQDateEdit->setEnabled(true);
                        lotwRecQDateEdit->setDate(QDate::fromString(aux1, "yyyy/MM/dd"));
                    }
                }


                else
                {
                    lotwRecComboBox->setCurrentIndex( lotwRecComboBox->findText(tr("N-No") ) );
                }
*/
                //qDebug() << "MainWindow::qsoToEdit: - just before IOTA"  << endl;

                nameCol = rec.indexOf("iota");
                aux1 = (query.value(nameCol)).toString();

                aux1 = awards->checkIfValidIOTA(aux1);
                othersTabWidget->setIOTA(aux1);
/*
                //qDebug() << "MainWindow::qsoToEdit: IOTA-CheckIfValidIOTA-1 " << aux1 << endl;
                if ((aux1.length())==6){
                    QStringList values = aux1.split("-", QString::SkipEmptyParts);
                    //qDebug() << "MainWindow::qsoToEdit: IOTA " << aux1 << endl;

                    iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(values.at(0) ) );
                    iotaNumberLineEdit->setText(values.at(1));

                }
*/
                nameCol = rec.indexOf("sat_name");
                aux1 = (query.value(nameCol)).toString();
                if (aux1.length()>0)
                {
                    satTabWidget->setSatName(aux1);
                }
                else
                {
                    satTabWidget->clear();
                }


                nameCol = rec.indexOf("sat_mode");
                aux1 = (query.value(nameCol)).toString();
                if (aux1.length()>1)
                {
                    satTabWidget->setSatMode(aux1);
                }
                else
                {
                    satTabWidget->setSatMode("-CLEAR-");
                }

               //qDebug() << "MainWindow::qsoToEdit: - in default - 100: " << QString::number(currentEntity)  << endl;

                nameCol = rec.indexOf("dxcc");
                aux1  = (query.value(nameCol)).toString();

               //qDebug() << "MainWindow::qsoToEdit: Checking DXCC: " << aux1 << " - " << world->getEntityName(aux1.toInt()) << endl;

                if (aux1.toInt()>=1)
                {

                    if (aux1.toInt() == util->getNormalizedDXCCValue(currentEntity))
                    {

                    }
                    else
                    {
                        currentEntity = aux1.toInt();
                    }

                   //qDebug() << "MainWindow::qsoToEdit: - in default - 101: " << QString::number(currentEntity)  << endl;
                }
                else
                {
                    currentEntity = world->getQRZARRLId(currentQrz);
                   //qDebug() << "MainWindow::qsoToEdit: - in default - 103: " << QString::number(currentEntity)  << endl;

                }
               //qDebug() << "MainWindow::qsoToEdit: - in default - 104: " << QString::number(currentEntity)  << endl;

                nameCol = rec.indexOf("prop_mode");
                aux1  = (query.value(nameCol)).toString();
                othersTabWidget->setPropMode(aux1);
/*
                if(( propModeComboBox->findText(aux1+" -", Qt::MatchContains))>0)
                {
                    propModeComboBox->setCurrentIndex( propModeComboBox->findText(aux1+" -", Qt::MatchContains));
                   //qDebug() << "MainWindow::qsoToEdit: Prop2: " << aux1 << endl;
                }
                else
                {
                    propModeComboBox->setCurrentIndex(0);
                }
*/
                infoLabel2->setText(world->getEntityName(currentEntity));
                infoWidget->showEntityInfo(currentEntity);
                //selectCorrectComboBoxEntity(currentEntity);
               //qDebug() << "MainWindow::qsoToEdit: " << QString::number(currentEntity) << endl;
                othersTabWidget->setEntity(currentEntity);
                //qDebug() << "MainWindow::qsoToEdit: - in default - 101"  << endl;

                QStringList _qs; //for the showStatusOfDXCC(const QStringList _qs)
                _qs.clear();
                //TODO: The band sometimes fails here. Check

                _qs << QString::number(currentEntity) << QString::number(dataProxy->getIdFromBandName(bandComboBox->currentText())) << QString::number(dataProxy->getIdFromBandName(modeComboBox->currentText()))  << QString::number(currentLog);


                //qDebug() << "MainWindow::qsoToEdit: - in default - 104"  << endl;
                showStatusOfDXCC(_qs);

//qDebug() << "MainWindow::qsoToEdit: - in default - 105"  << endl;
                //qDebug() << "MainWindow::qsoToEdit: - just after de IOTA"  << endl;
        break;
        } // Closesthe switch (contest or default)
    //qDebug() << "MainWindow::qsoToEdit: - in default - 106"  << endl;
    } //Closes the next.isValid
    //qDebug() << "MainWindow::qsoToEdit: - in default - END"  << endl;


}

/*
//TODO: TO BE REMOVED EQSL
void MainWindow::slotQSLRecvComboBoxChanged(){
    //qDebug() << "MainWindow::slotQSLRecvComboBoxChanged" << endl;

//QSLRDATE (only valid if QSL_RCVD is Y-0, I-3, or V-4)
//Y-Yes-0
//N-No-1
//R-Requested-2
//I-Ignore-3
//V-Verified-4

    int i = qslRecComboBox->currentIndex();

    switch (i)
    {
        case 0:
            qslRecQDateEdit->setEnabled(true);
            qslRecViaComboBox->setEnabled(true);
            qslRecQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            qslRecQDateEdit->setEnabled(true);
        break;
        case 4:
            qslRecQDateEdit->setEnabled(true);
        break;
        default: //NO
            qslRecQDateEdit->setEnabled(false);
            qslRecViaComboBox->setEnabled(false);


        break;


    }

}


void MainWindow::slotLotwRecvComboBoxChanged(){
    //qDebug() << "MainWindow::slotLotwRecvComboBoxChanged" << endl;

//QSLRDATE (only valid if QSL_RCVD is Y-0, I-3, or V-4)
//Y-Yes-0
//N-No-1
//R-Requested-2
//I-Ignore-3
//V-Verified-4

    int i = lotwRecComboBox->currentIndex();

    switch (i)
    {
        case 0:
            lotwRecQDateEdit->setEnabled(true);
            lotwRecQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            lotwRecQDateEdit->setEnabled(true);
        break;
        case 4:
            lotwRecQDateEdit->setEnabled(true);
        break;
        default: //NO
            lotwRecQDateEdit->setEnabled(false);
        break;


    }

}


void MainWindow::slotLotwSentComboBoxChanged(){
    //qDebug() << "MainWindow::slotLotwSentComboBoxChanged" << endl;

    int i = lotwSentComboBox->currentIndex();
//{Y, N, R, I, V}
    //(QSLSDATE is only valid if QSL_SENT is Y-0, Q-3, or I-4)
    // Y-Yes = 0
    // N-No = 1
    // R-Requested = 2
    // Q-Queued = 3
    // I-Ignore = 4
    switch (i)
    {
        case 0:
            lotwSentQDateEdit->setEnabled(true);
            lotwSentQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            lotwSentQDateEdit->setEnabled(true);
        break;
        case 4:
            lotwSentQDateEdit->setEnabled(true);
        break;

        default: //NO
            lotwSentQDateEdit->setEnabled(false);
        break;
    }
}


void MainWindow::slotQSLSentComboBoxChanged(){
    //qDebug() << "MainWindow::slotQSLSentComboBoxChanged" << endl;

    int i = qslSentComboBox->currentIndex();
//{Y, N, R, I, V}
    //(QSLSDATE is only valid if QSL_SENT is Y-0, Q-3, or I-4)
    // Y-Yes = 0
    // N-No = 1
    // R-Requested = 2
    // Q-Queued = 3
    // I-Ignore = 4
    switch (i)
    {
        case 0:
            qslSentQDateEdit->setEnabled(true);
            qslSentViaComboBox->setEnabled(true);
            qslSentQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            qslSentQDateEdit->setEnabled(true);
        break;
        case 4:
            qslSentQDateEdit->setEnabled(true);
        break;

        default: //NO
            qslSentQDateEdit->setEnabled(false);
            qslSentViaComboBox->setEnabled(false);
        break;
    }
}
*/
/*
void MainWindow::sloteQSLRecvComboBoxChanged(){
    //qDebug() << "MainWindow::sloteQSLRecvComboBoxChanged" << endl;

//QSLRDATE (only valid if QSL_RCVD is Y-0, I-3, or V-4)
//Y-Yes-0
//N-No-1
//R-Requested-2
//I-Ignore-3
//V-Verified-4

    int i = eqslRecComboBox->currentIndex();

    switch (i)
    {
        case 0:
            eqslRecQDateEdit->setEnabled(true);
            eqslRecQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            eqslRecQDateEdit->setEnabled(true);
        break;
        case 4:
            eqslRecQDateEdit->setEnabled(true);
        break;
        default: //NO
            eqslRecQDateEdit->setEnabled(false);
        break;


    }

}
*/

/*
void MainWindow::sloteQSLSentComboBoxChanged(){
    //qDebug() << "MainWindow::sloteQSLSentComboBoxChanged" << endl;

    int i = eqslSentComboBox->currentIndex();
//{Y, N, R, I, V}
    //(QSLSDATE is only valid if QSL_SENT is Y-0, Q-3, or I-4)
    // Y-Yes = 0
    // N-No = 1
    // R-Requested = 2
    // Q-Queued = 3
    // I-Ignore = 4
    switch (i)
    {
        case 0:
            eqslSentQDateEdit->setEnabled(true);
            eqslSentQDateEdit->setDate((dateTime->currentDateTime()).date());

        break;
        case 2:
        break;
        case 3:
            eqslSentQDateEdit->setEnabled(true);
        break;
        case 4:
            eqslSentQDateEdit->setEnabled(true);
        break;

        default: //NO
            eqslSentQDateEdit->setEnabled(false);
        break;
    }
}
*/

/*
void MainWindow::showInfoFromLocators(const QString _loc1, const QString _loc2)
{// Local / DX
    //qDebug() << "MainWindow::showInfoFromLocators: " << _loc1 << "/" << _loc2 << endl;
    QString lloc = _loc1.toUpper();
    QString ldx = _loc2.toUpper();

    if ( locator->isValidLocator(lloc)  )
    {
        if ( locator->isValidLocator(ldx)  )
        {

            int beam = locator->getBeamBetweenLocators(lloc, ldx);


            gradShortLabel->setText( QString::number(beam) );

             if (beam >= 180)
             {
                   gradLongLabel->setText( QString::number(beam -180 ) );
             }
             else
             {
                 gradLongLabel->setText( QString::number(beam + 180 ) );
             }

             distShortLabel->setText( QString::number( locator->getDistanceBetweenLocators(lloc, ldx, imperialSystem) ) );
             distLongLabel->setText( QString::number( 40000 - locator->getDistanceBetweenLocators(lloc, ldx, imperialSystem) ) );
        }
        else
        {
            infoWidget->clear();
            //infoWidget->clearInfoFromLocators();
            return;
        }
    }
    else
    {
        infoWidget->clear();
        //infoWidget->clearInfoFromLocators();
        return ;
    }
}

*/
void MainWindow::slotLocatorTextChanged()
{//TO BE REMOVED ONCE InfoWidget is FINISHED - At least modified
    //qDebug() << "MainWindow::slotLocatorTextChanged: " << locatorLineEdit->text() << endl;
    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        dxLocator = (locatorLineEdit->text()).toUpper();
        infoWidget->showDistanceAndBearing(myLocator, dxLocator);
        //showInfoFromLocators(myLocator, dxLocator);
    }
    else
    {
        return;
    }
}

void MainWindow::slotMyLocatorTextChanged()
{
    //qDebug() << "MainWindow::slotMyLocatorTextChanged: " << myLocatorLineEdit->text() << endl;

    if ( locator->isValidLocator((myLocatorLineEdit->text()).toUpper()) )
    {
        myLocator = (myLocatorLineEdit->text()).toUpper();
        //qDebug() << "MainWindow::slotMyLocatorTextChanged: My LOCATOR CHANGED TO: " << myLocator << endl;
        slotLocatorTextChanged();
    }
    else
    {
        return;
    }
}

/*
void MainWindow::clearInfoFromLocators()
{
    //qDebug() << "MainWindow::clearInfoFromLocators" << endl;
    gradShortLabel->setText( "0" );
    gradLongLabel->setText( "0" );
    distShortLabel->setText( "0" );
    distLongLabel->setText( "0" );
}

*/

/*
void MainWindow::showEntityInfo(const int _enti, int _cq, int _itu)
{
  //qDebug() << "MainWindow::showEntityInfo" << QString::number(_enti) << endl;

    if (_enti<=0)
    {
        return;
    }

    infoLabel2->setText(world->getEntityName(_enti));
    continentLabel->setText( world->getContinentShortName(_enti) );
    prefixLabel->setText( world->getEntityMainPrefix(_enti));

    if ( locator->isValidLocator((locatorLineEdit->text()).toUpper()) )
    {
        dxLocator = (locatorLineEdit->text()).toUpper();
    }
    else
    {
        dxLocator = world->getLocator(_enti);
    }

    infoWidget->showDistanceAndBearing (myLocator, dxLocator);

    int i = -1;

    if ((_cq>0) && (_cq<41))
    {
        cqzLabel->setText( QString::number(_cq) );
    }
    else
    {
        i = world->getEntityCqz(_enti);
        if( i > 0 )
        {
            cqzLabel->setText( QString::number(i) );
        }
        else
        {
            cqzLabel->setText("0");
        }
    }

    if (_itu>0)
    {
        ituzLabel->setText( QString::number(_itu) );
    }
    else
    {
        i = world->getEntityItuz(_enti);
        if ( i > 0  )
        {
            ituzLabel->setText( QString::number(i) );
        }
        else
        {
            ituzLabel->setText("0");
        }
    }

}
*/

void MainWindow::showStatusOfDXCC(const QStringList _qs)
{
    //qDebug() << "MainWindow::showStatusOfDXC: QRZ: " << _qs.at(0) << "/ Bandid :" << _qs.at(1) << "/Modeid: " << _qs.at(2) << endl;
    // Receives:  QStringList _qs;
    //_qs << QRZ << BandId << ModeId << lognumber;

    // Check if new one, needed, worked, confirmed
    // Print the infoLabel1
    // Print/Color the band boxes/labels

    /*
    0 - New One
    1 - Needed
    2 - Worked
    3 - Confirmed
    */

    if ((_qs.length() != 4) || (_qs.at(1) == "-1")) // is the qs valid?
    {
        infoWidget->clear();
        infoLabel1->setText(tr("--"));
        return;
    }
    // Set the status bar with the appropriate message
    int status = awards->getDXStatus (_qs);
    QString message = QString();

    //qDebug() << "MainWindow::showStatusOfDXC: " << QString::number(status) << endl;

    message = awards->getDXStatusString(status);
    infoLabel1->setText(message);
    infoWidget->showInfo((_qs.at(0)).toInt(), (_qs.at(1)).toInt(), (_qs.at(2)).toInt(), (_qs.at(3)).toInt() );
}


void MainWindow::showDXMarathonNeeded(const int _dxcc, const int _cqz, const int _year, const int _log)
{
   //qDebug()() << "MainWindow::showDXMarathonNeeded" << endl;
    if ((_dxcc<=0) || (_cqz<=0))
    {
        return;
    }
    if ( awards->isDXMarathonNeed(_dxcc, _cqz, _year, _log))
    {        
        infoLabel1->setText(infoLabel1->text()+ tr(" - Needed for DXMarathon"));
    }
}
void MainWindow::slotShowAwards()
{ //To be called from the logWindow & searchWidget

    logWindow->refresh();
    awards->recalculateAwards();
    showAwards();
    dxccStatusWidget->refresh();

}

void MainWindow::showAwards()
{ // Updates and show all the award status tab.
//qDebug() << "MainWindow::showAwards" << endl;
/*
  WAZ
  Local
*/
    int _num = 0;
    QSqlQuery query;
    QString aux;

/*
    aux = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(currentLog);
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _num = (query.value(0)).toInt();
    }
    */
    _num = dataProxy->getHowManyQSOInLog(currentLog);
    qsoWorkedQLCDNumber->display(_num);

    _num = dataProxy->getHowManyConfirmedQSLInLog(currentLog);
    //_num = 0;
    //aux = QString("SELECT count(id) FROM log WHERE qsl_rcvd='Y' AND lognumber='%1'").arg(currentLog);
    //query.exec(aux);
    //query.next();
    //if (query.isValid())
    //{
    //    _num = (query.value(0)).toInt();
    //}
    qsoConfirmedQLCDNumber->display(_num);
    _num = 0;

    dxccWorkedQLCDNumber->display(awards->getDXCCWorked(currentLog));
    _num = 0;

    dxccConfirmedQLCDNumber->display(awards->getDXCCConfirmed(currentLog));
    _num = 0;

    wazWorkedQLCDNumber->display(awards->getWAZWorked(currentLog));
    _num = 0;

    wazConfirmedQLCDNumber->display(awards->getWAZConfirmed(currentLog));

    showDXMarathon(selectedYear);


}

void MainWindow::showDXMarathon(const int _year)
{
    //qDebug() << "MainWindow::MainWindow::showDXMarathon: Year: " << QString::number(_year) << endl;
    int i = 0;

    i = awards->getDXMarathonDXCC(_year, currentLog);
    //qDebug() << "MainWindow::MainWindow::showDXMarathon: DXCC: " << QString::number(i) << endl;
    dxMarathonDXCCQLCDNumber->display(i);

    i = awards->getDXMarathonCQ(_year, currentLog);
    dxMarathonCQQLCDNumber->display(i);
    //qDebug() << "MainWindow::MainWindow::showDXMarathon: CQ: " << QString::number(i) << endl;

    i = awards->getDXMarathonScore(_year, currentLog);
    dxMarathonPointsQLCDNumber->display(i);
    //qDebug() << "MainWindow::MainWindow::showDXMarathon: Score: " << QString::number(i) << endl;
}
void MainWindow::fillQSOData()
{ // Updates all QSO with the dxcc, CQZ, ... if empty.
   //qDebug() << "MainWindow::fillQSOData" << endl;

    QString stringQuery = QString("SELECT call, bandid, modeid, qso_date, time_on, lognumber, confirmed, id, cqz, ituz, dxcc FROM log WHERE lognumber='%1'").arg(currentLog);

    QSqlQuery query;
    query.exec(stringQuery);
    QSqlQuery query1;
    QSqlRecord rec = query.record();
    int nameCol;
    QString aux, queryString;
    QString _call, _bandid, _modeid, _tdate, _ttime, _lognumber, _id, aux1, updateString, _confirmed;
    bool toModify = false;
    bool noMoreQso = false;
    bool sqlOK;

    int numberOfQsos = 0;
    int i = 0;
/*
    aux = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(currentLog);
    //aux = "SELECT count(id) FROM log";
    query1.exec(aux);
    query1.next();
    if (query1.isValid())
    {
        numberOfQsos = (query1.value(0)).toInt();
    }
*/
    numberOfQsos = dataProxy->getHowManyQSOInLog(currentLog);

    //int progressBarPosition = 0;

    QProgressDialog progress(tr("Filling QSOs..."), tr("Abort filling"), 0, numberOfQsos, this);
    progress.setWindowModality(Qt::WindowModal);


    while ( (query.next()) && (!noMoreQso) ) {
        if (query.isValid())
        {
            i++;
            toModify = false;
            nameCol = rec.indexOf("call");
            if ( (query.value(nameCol)).isValid() )
            {
                _call = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("bandid");
            if ( (query.value(nameCol)).isValid() )
            {
                _bandid = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("modeid");
            if ( (query.value(nameCol)).isValid() )
            {
                _modeid = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("qso_date");
            if ( (query.value(nameCol)).isValid() )
            {
                _tdate = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("time_on");
            if ( (query.value(nameCol)).isValid() )
            {
                _ttime = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("lognumber");
            if ( (query.value(nameCol)).isValid() )
            {
                _lognumber = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("confirmed");
            if ( (query.value(nameCol)).isValid() )
            {
                _confirmed = (query.value(nameCol)).toString();
            }
            nameCol = rec.indexOf("id");
            if ( (query.value(nameCol)).isValid() )
            {
                _id = (query.value(nameCol)).toString();
            }
            //qDebug() << "MainWindow::fillQSOData: ID: " << _id << endl;
            //TODO: Prepare this query
            updateString = "UPDATE log SET call = '" + _call + "', bandid = '" + _bandid + "', modeid = '" + _modeid + "', qso_date = '" + _tdate + "', time_on = '" + _ttime + "', lognumber = '" + _lognumber  + "', confirmed = '" + _confirmed + "'";

            nameCol = rec.indexOf("cqz");
            if (( (query.value(nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number(world->getQRZCqz(_call));
                updateString = updateString + ", cqz='" + aux1 + "'";
            toModify = true;
            }
            else
            {
            }

            nameCol = rec.indexOf("ituz");
            if (( (query.value(nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number( world->getQRZItuz(_call) );
                updateString = updateString + ", ituz='" + aux1 + "'";
                toModify = true;
            }
            else
            {}
//qDebug() << "MainWindow::fillQSOData: DXCC" << endl;
            nameCol = rec.indexOf("dxcc");
            if (( (query.value(nameCol)).toString()).length() < 1 )
            {
                aux1 = QString::number(world->getQRZARRLId(_call) );
                //qDebug() << "MainWindow::fillQSOData: DXCC proposed: " << aux1 << endl;
                if (aux1.toInt()>0)
                {
                    updateString = updateString + ", dxcc='" + aux1 + "'";
                    toModify = true;
                    //qDebug() << "MainWindow::fillQSOData: DXCC: " << aux1 << endl;
                }
                else
                {
                    //qDebug() << "MainWindow::fillQSOData: no DXCC identified"  << endl;
                }

            }
            else
            {
                //qDebug() << "MainWindow::fillQSOData: DXCC already existed"  << endl;
            }

            //qDebug() << "MainWindow::fillQSOData1: " << updateString << endl;
            if (toModify)
            {
                updateString = updateString + " WHERE id = " + "'" + _id + "'";
                //qDebug() << "MainWindow::fillQSOData2: " << updateString << endl;
                sqlOK = query1.exec(updateString);
                if (sqlOK)
                {
                    //qDebug() << "MainWindow::fillQSOData: sqlOK=True" << endl;
                }
                else
                {
                    //qDebug() << "MainWindow::fillQSOData: sqlOK=False" << endl;
                }

            }
            else
            {
                updateString.clear();
            }

            aux = tr("Filling QSOs...\n QSO: ")  + QString::number(i) + "/" + QString::number(numberOfQsos);
            progress.setLabelText(aux);
            progress.setValue(i);

            if ( progress.wasCanceled() )
            {
                //qDebug() << "MainWindow::fillQSOData3: " << endl;
                noMoreQso = true;
            }
        } // Closes the next.isValid
    } // Closes the While
}


void MainWindow::slotUpdateCTYDAT()
{
   //qDebug() << "MainWindow::slotUpdateCTYDAT" << endl;
    downloadcty->download();
}

void MainWindow::slotWorldReload()
{
   //qDebug() << "MainWindow::slotWorldReload" << endl;
    //TODO: world.recreate returns a boolean, so it is possible to manage the errors
    world->recreate(ctyDatFile);
}

void MainWindow::slotFilePrint()
{
    //qDebug() << "MainWindow::slotFilePrint" << endl;
    QPrinter printer;
    QString aux;
    QSqlQuery query, query1;
    int row = 0;
    int _numberOfQsos = 0;
    bool cancelPrinting = false;
/*
    aux = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(currentLog);
    //aux = "SELECT count(id) FROM log";
    query.exec(aux);
    query.next();
    if (query.isValid())
    {
        _numberOfQsos = (query.value(0)).toInt();
    }
*/
    _numberOfQsos = dataProxy->getHowManyQSOInLog(currentLog);


    QTextCursor cursor(doc);
    QTextTable *textTable = cursor.insertTable(1, 9);

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount(1);
    tableFormat.setCellPadding(5);
    tableFormat.setAlignment(Qt::AlignCenter);

    textTable->setFormat(tableFormat); //The first row is the header

    cursor = textTable->cellAt(row, 0).firstCursorPosition();
    cursor.setBlockFormat(centerAlignment);
    cursor.insertText(tr("Number"));
    cursor = textTable->cellAt(row, 1).firstCursorPosition();
    cursor.insertText(tr("Date"));
    cursor = textTable->cellAt(row, 2).firstCursorPosition();
    cursor.insertText(tr("Time"));
    cursor = textTable->cellAt(row, 3).firstCursorPosition();
    cursor.insertText(tr("QRZ"));
    cursor = textTable->cellAt(row, 4).firstCursorPosition();
    cursor.insertText(tr("RSTtx"));
    cursor = textTable->cellAt(row, 5).firstCursorPosition();
    cursor.insertText(tr("RSTrx"));
    cursor = textTable->cellAt(row, 6).firstCursorPosition();
    cursor.insertText(tr("Band"));
    cursor = textTable->cellAt(row, 7).firstCursorPosition();
    cursor.insertText(tr("Mode"));
    cursor = textTable->cellAt(row, 8).firstCursorPosition();
    cursor.insertText(tr("Comment"));

    int nameCol = 0;

    //int qsoPerPage = 25;
    //int maxPages = (int)(Klog::number / qsoPerPage)+1; // To print just 10 QSO per page
    //int printedSoFar = 0;
    //int page = 1;

    printer.setOrientation(QPrinter::Landscape); // For testing, the log will be printed landscape.
    printer.setDocName(stationQRZ+"-log");

    QPrintDialog printDialog(&printer, this);
    printDialog.setWindowTitle(tr("Print Log"));

    if (printDialog.exec() == QDialog::Accepted)
    {
        int _qsos = 0;
        QProgressDialog progress(tr("Printing the log..."), tr("Abort printing"), 0, _numberOfQsos, this);
        progress.setMaximum(_numberOfQsos);
        progress.setWindowModality(Qt::WindowModal);


        QString stringQuery = QString("SELECT id, qso_date, time_on, call, rst_sent, rst_rcvd, bandid, modeid, comment FROM log WHERE lognumber='%1'").arg(currentLog);
        query.exec(stringQuery);
        QSqlRecord rec = query.record();

        while ((query.next()) && (!cancelPrinting))
        {
            if (query.isValid())
            {

                textTable->appendRows(1);
                row++;
                _qsos++;
                aux = tr("Printing the log...\n QSO: ")  + QString::number(_qsos) + "/" + QString::number(_numberOfQsos);
                progress.setLabelText(aux);
                progress.setValue(_qsos);

                nameCol = rec.indexOf("id");
                cursor = textTable->cellAt(row, 0).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("qso_date");
                cursor = textTable->cellAt(row, 1).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("time_on");
                cursor = textTable->cellAt(row, 2).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("call");
                cursor = textTable->cellAt(row, 3).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("rst_sent");
                cursor = textTable->cellAt(row, 4).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("rst_rcvd");
                cursor = textTable->cellAt(row, 5).firstCursorPosition();
                cursor.insertText((query.value(nameCol)).toString());

                nameCol = rec.indexOf("bandid");
                aux = (query.value(nameCol)).toString();
                stringQuery = QString("SELECT name FROM band WHERE id='%1'").arg(aux);
                query1.exec(stringQuery);
                query1.next();
                if (query1.isValid())
                {
                    cursor = textTable->cellAt(row, 6).firstCursorPosition();
                    cursor.insertText((query1.value(0)).toString());
                }

                nameCol = rec.indexOf("modeid");
                aux = (query.value(nameCol)).toString();
                aux = dataProxy->getNameFromSubModeId(aux.toInt());
                if (aux.length()>1)
                {
                    cursor = textTable->cellAt(row, 7).firstCursorPosition();
                    cursor.insertText((query1.value(0)).toString());
                }

                //stringQuery = QString("SELECT name FROM mode WHERE id='%1'").arg(aux);
                //query1.exec(stringQuery);
                //query1.next();
                //if (query1.isValid())
                //{
                //    cursor = textTable->cellAt(row, 7).firstCursorPosition();
                //    cursor.insertText((query1.value(0)).toString());
                //}

                nameCol = rec.indexOf("comment");
                aux = (query.value(nameCol)).toString();
                if ((aux.length())>0)
                {
                    cursor = textTable->cellAt(row, 8).firstCursorPosition();
                    cursor.insertText(aux);
                }
            }
            if ( progress.wasCanceled() )
            {
                    cancelPrinting = true;
            }
        }
        progress.setValue(_numberOfQsos);
        if (!cancelPrinting)
        {
            doc->print(&printer);
        }
    }

}
//UPDATE CTY.CSV


//UPDATE CTY.CSV
/*
 void MainWindow::slotToolSearchRequestedQSLToSend()
 {

    //qDebug() << "MainWindow::slotToolSearchRequestedQSLToSend" << endl;
     slotToolSearchQSL(1);
     //return;
 }

void MainWindow::searchToolNeededQSLToSend()
{
   //qDebug() << "MainWindow::slotToolSearchQSLToSend - TO PREPARE THE QUERY and optimize the function" << endl;
    slotToolSearchQSL(0);
}

void MainWindow::slotToolSearchNeededQSLPendingToReceive()
{
    //qDebug() << "MainWindow::slotToolSearchNeededQSLPendingToReceive "  << endl;
    // QSL RCVD requested or
    // QSL Sent Y and qsl_rcvd!=Y AND qsl_rcvd!=I
    //select call from log where (qsl_sent='Y' and qsl_rcvd!='Y' and qsl_rcvd!='I') OR
    //QString stringQuery = QString("SELECT call FROM log where (qsl_sent='Y' AND qsl_rcvd!='Y' AND qsl_rcvd!='I') OR qsl_rcvd='R'");
    slotToolSearchQSL(2);
}

void MainWindow::slotToolSearchNeededQSLRequested()
{ // Requested DX QSL
    slotToolSearchQSL(3);
}


void MainWindow::slotToolSearchQSL(const int actionQSL)
{
    //qDebug() << "MainWindow::slotToolSearchQSL: " << QString::number(actionQSL) << " - LogNumber: " << QString::number(currentLog) << endl;
    // 2 means QSL_RCVD = 'R'
    QString stringQuery = QString();
    QString message = QString();
    QString aux = QString();
    int i = -1;


    switch (actionQSL)
    {
        case 0://void searchToolNeededQSLToSend();
         //aux = QString("SELECT count(id) FROM log WHERE lognumber='%1'").arg(currentLog);
         //qDebug() << "MainWindow::slotToolSearchQSL: CASE 0" << endl;
         stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, log.id FROM log JOIN awarddxcc ON awarddxcc.qsoid=log.id WHERE awarddxcc.confirmed='0' AND log.qsl_sent!='Y' AND log.qsl_sent!='Q' AND log.qsl_sent!='R' AND log.lognumber='%1'").arg(currentLog);
            message = tr("Needed QSO to send the QSL");
            qslingNeeded = true;
            dxUpRightTab->setCurrentIndex(2);
        break;
        case 1://void slotToolSearchRequestedQSLToSend();
        //qDebug() << "MainWindow::slotToolSearchQSL: CASE 1" << endl;
            stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, id FROM log  WHERE qsl_sent=='R' AND lognumber='%1'").arg(currentLog);
            message = tr("My QSL requested to be sent");
        break;
        case 2://void slotToolSearchNeededQSLPendingToReceive();
        //qDebug() << "MainWindow::slotToolSearchQSL: CASE 2" << endl;
        stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, log.id FROM log WHERE lognumber='%1' AND ( (qsl_sent='Y' AND qsl_rcvd!='Y' AND qsl_rcvd!='I') OR qsl_rcvd='R')").arg(currentLog);
            message = tr("DX QSL pending to be received");
        break;
    case 3://void slotToolSearchNeededQSLRequested()
        //qDebug() << "MainWindow::slotToolSearchQSL: CASE 3" << endl;
        stringQuery = QString("SELECT call, qso_date, time_on, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, log.id FROM log WHERE lognumber='%1' AND  qsl_rcvd='R'").arg(currentLog);
        message = tr("DX QSL pending to be received");
    break;
        default:
        //qDebug() << "MainWindow::slotToolSearchQSL: CASE DEFAULT" << endl;
        // should never be reached
            return;
        break;
    }

    int nameCol = -1;
    QString _call, _dateTime, _band, _mode, _freq, _qsltx, _qslrx, _id, _stationcallsign;
    QFont font;
    font.setBold(true);
    QColor color;
    QStringList q;

    searchResultsTreeWidget->clear();
    QSqlQuery query(stringQuery);
    QSqlRecord rec = query.record();

    if (!query.exec())
    {
       //qDebug() << "MainWindow::slotToolSearchQSL: Query ERROR" << endl;
    //TODO: Control the error!!
    }
    else
    {
        slotUpdateStatusBar(message); // updating the status bar
        while(query.next())
        {
        if (query.isValid())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(searchResultsTreeWidget);
            //aux = world->getQRZEntityName(_call);
            i = world->getQRZARRLId(_call);
            aux = world->getEntityName(i) + " - CQ: " + QString::number(world->getEntityCqz(i));
            item->setToolTip(0, aux);
            item->setToolTip(1, aux);
            item->setToolTip(2, aux);
            item->setToolTip(3, aux);
            item->setToolTip(4, aux);
            item->setToolTip(5, aux);
            item->setToolTip(6, aux);

            nameCol = rec.indexOf("call");
            _call= (query.value(nameCol)).toString();
            nameCol = rec.indexOf("qso_date");
            _dateTime = (query.value(nameCol)).toString();
            nameCol = rec.indexOf("time_on");
            _dateTime = _dateTime + "-" +(query.value(nameCol)).toString();

            nameCol = rec.indexOf("bandid");
            _freq = (query.value(nameCol)).toString();
            _band = db->getBandNameFromNumber( _freq.toInt() );

            nameCol = rec.indexOf("modeid");
            _mode = dataProxy->getSubModeFromId((query.value(nameCol)).toInt());

            //qDebug() << "MainWindow::slotToolSearchQSL: Mode: " << _mode << endl;
            //qDebug() << "MainWindow::slotToolSearchQSL: mode " << QString::number((query.value(nameCol)).toInt()) << endl;

            nameCol = rec.indexOf("qsl_sent");
            _qsltx = (query.value(nameCol)).toString();
            if (_qsltx.length()<1)
            {
                _qsltx = "N";
            }

            nameCol = rec.indexOf("qsl_rcvd");
            _qslrx = (query.value(nameCol)).toString();
            if (_qslrx.length()<1)
            {
                _qslrx = "N";
            }

            if (stationCallSignShownInSearch)
            {
                //qDebug() << "MainWindow::slotToolSearchQSL: stationCallSign "<< endl;

                nameCol = rec.indexOf("station_callsign");
                if (((query.value(nameCol)).toString()).length()>=3)
                {
                    _stationcallsign = (query.value(nameCol)).toString();
                }
                else
                {
                    _stationcallsign.clear();
                }

            }

            nameCol = rec.indexOf("id");
            _id= (query.value(nameCol)).toString();

            q.clear();
            q << _call << _freq << _mode << QString::number(currentLog);

            color = awards->getQRZDXStatusColor(q);

            item->setText(0, _call);
            item->setText(1, _dateTime);
            item->setText(2, _band);
            item->setText(3, _mode);
            item->setText(4, _qsltx);
            item->setText(5, _qslrx);
            if (stationCallSignShownInSearch)
            {
                item->setText(6, _stationcallsign);
                item->setText(7, _id);
                item->setToolTip(7, aux);
            }
            else
            {
                item->setText(6, _id);
            }

            item->setForeground(0, QBrush(color));
        }
        else
        {
        //TODO: Check what is happening here!
        }
    }
        //qslingNeeded = true;
        dxUpRightTab->setCurrentIndex(2);
    }
}

*/
//DX-CLUSTER - DXCLUSTER

void MainWindow::slotAnalyzeDxClusterSignal(QStringList ql)
{
   //qDebug() << "MainWindow::slotAnalyzeDxClusterSignal: 1: " << ql.at(0) <<"/1: " << ql.at(1) << "/2: " << ql.at(2) << endl;
    QStringList qls;
    int _entity = world->getQRZARRLId(ql.at(0));
    qls.clear();
    QString _mode = "-1";
    if (!manageMode)
    {
        _mode = "-1";
    }

    if (ql.length()==3)
    {
        if ((ql.at(2)) == "double")
        {
            clusterSpotToLog(ql.at(0), ql.at(1));
        }
        else if ((ql.at(2)) == "selected")
        {
            infoLabel2->setText(world->getEntityName(_entity));            

            infoWidget->showEntityInfo( _entity );

            // Becareful, he Frecuency arrives in KHz instead of bandid!!
            // db.getBandFromFreq expects a MHz!
            //(ql.at(1)).toDouble()

            qls << QString::number(_entity) << QString::number(dataProxy->getBandIdFromFreq((ql.at(1).toDouble()/1000))) << _mode <<  QString::number(currentLog);
            // We use a mode = -1 because we don't know the mode info from the DXCluster spot

            // TODO: Check if we can know the mode and replace the "-1" in previous sentence

            //qls << QRZ << BandId << ModeId << lognumber;
            showStatusOfDXCC(qls);
        }
        else
        {
        }
    }
    else
    { // Signal was not properly emited
    }


}

double MainWindow::checkFreqRanges(double _f)
{
    if ( (_f > 0) && (_f <= txFreqSpinBox->maximum()))
    {
        return _f;
    }
        else
    {
        return 0;
    }
    return 0;
}

//void MainWindow::clusterSpotToLog(const QStringList _qs)
void MainWindow::clusterSpotToLog(const QString _call, const QString _freq)
{
    //qDebug() << "MainWindow::clusterSpotToLog: " << _call <<"/" << _freq << endl;

    QString _aux;    
    double _freqN = (_freq.toDouble()) / 1000;
    qrzLineEdit->setText(_call);
    //qrzLineEdit->setText(_qs.at(0));

    txFreqSpinBox->setValue(checkFreqRanges(_freqN));
    //freqQLCDNumber->display(_freqN);

    _aux = QString::number(_freqN);

    //qDebug() << "MainWindow::clusterSpotToLog - Freq: " << _aux << endl;

    int _bandi = dataProxy->getBandIdFromFreq(_aux.toDouble());
    //qDebug() << "MainWindow::clusterSpotToLog - Bandi: " << QString::number(_bandi) << endl;
    _aux = QString::number(_bandi);
    _aux = QString("SELECT name FROM band WHERE id ='%1'").arg(_aux);

    //qDebug() << "MainWindow::clusterSpotToLog - Band: " << _aux << endl;

    QSqlQuery query(_aux);
    query.next();
    if (query.isValid())
    {
        _aux = (query.value(0)).toString();
        bandComboBox->setCurrentIndex(bandComboBox->findText(_aux));
    }
    else
    {
        bandComboBox->setCurrentIndex(bandComboBox->findText(dataProxy->getNameFromBandId(defaultBand)));
        //bandComboBox->setCurrentIndex(defaultBand);
    }

}
//DX-CLUSTER - DXCLUSTER

void MainWindow::updateQSLRecAndSent()
{
    //qDebug() << "MainWindow::updateQSLRecAndSent "  << endl;
    // Checks the log to fill all the qsl_rcvd and qsl_sent
    QSqlQuery query, query1;
    QString queryString, aux, idT;
    int nameCol=0;

    queryString = QString("SELECT id, qsl_rcvd, qsl_sent FROM log WHERE lognumber='%1'").arg(currentLog);

    query.exec(queryString);

    QSqlRecord rec = query.record();

    while (query.next())
    {
        if (query.isValid())
        {
            nameCol = rec.indexOf("id");
            idT = (query.value(nameCol)).toString();
           //qDebug() << "MainWindow::updateQSLRecAndSent: " << idT  << endl;

            // {Y, N, R, I, V}
            nameCol = rec.indexOf("qsl_rcvd");
            aux = (query.value(nameCol)).toString();
            if ( (aux != "Y") && (aux != "N") && (aux != "R") && (aux != "I") && (aux != "V") )
            {//QSL_RCVD
                nameCol = rec.indexOf("qsl_sent");
                aux = (query.value(nameCol)).toString();

                //{Y, N, R, Q, I}

                if ( (aux != "Y") && (aux != "N") && (aux != "R") && (aux != "Q") && (aux != "I") )
                {//QSL_SENT
                    queryString = QString("UPDATE log SET qsl_rcvd='N' qsl_sent='N' WHERE id='%1'").arg(idT);
                    query1.exec(queryString);
                }
                else
                {
                    queryString = QString("UPDATE log SET qsl_rcvd='N' WHERE id='%1'").arg(idT);
                    query1.exec(queryString);

                }

            }
            else
            {//QSL_SENT

                nameCol = rec.indexOf("qsl_sent");
                aux = (query.value(nameCol)).toString();

                //{Y, N, R, Q, I}

                if ( (aux != "Y") && (aux != "N") && (aux != "R") && (aux != "Q") && (aux != "I") )
                {//QSL_SENT
                    queryString = QString("UPDATE log SET qsl_sent='N' WHERE id='%1'").arg(idT);
                    query1.exec(queryString);
                }
                else
                {

                }
            }
        }
    }
}
/*
void MainWindow::selectCorrectComboBoxEntity(const int _ent)
{// Select the appropriate entity in the ComboBox
    //qDebug() << "MainWindow::selectCorrectEntity: " << QString::number(_ent) << "/" << world->getEntityMainPrefix(_ent)  << endl;
    if (_ent<=0)
    {
        entityNameComboBox->setCurrentIndex(0);
        return;
    }
    QString pref = QString();
    //pref = world->getEntityMainPrefix(_ent);
    pref = world->getEntityName(_ent);

    //int indexC = entityNameComboBox->findText(pref, Qt::MatchContains);
    int indexC = entityNameComboBox->findText("("+QString::number(_ent)+")", Qt::MatchContains);
    //qDebug() << "MainWindow::selectCorrectEntity: " << pref << "/" << QString::number(indexC) << endl;
    entityNameComboBox->setCurrentIndex(indexC);

}

int MainWindow::getDXCCFromComboBox()
{
    //qDebug() << "MainWindow::getDXCCFromComboBox" << endl;
    //QString pref = (entityNameComboBox->currentText()).split('-').at(0);
    //qDebug() << "MainWindow::getDXCCFromComboBox: " << pref << "/" << QString::number(world->getQRZARRLId(pref))<< endl;
    return world->getQRZARRLId(othersTabWidget->getEntityPrefix());
}
*/
void MainWindow::slotOperatingYearComboBoxChanged()
{
    //qDebug() << "MainWindow::slotOperatingYearComboBoxChanged: " << operatingYearsComboBox->currentText() << endl;
    selectedYear = (operatingYearsComboBox->currentText()).toInt();
    showDXMarathon(selectedYear);
}

void MainWindow::defineStationCallsign()
{

   //qDebug() << "MainWindow::defineStationCallsign (currentLog): " << QString::number(currentLog) << endl;
    QString logQRZ;
    logQRZ = dataProxy->getStationCallSignFromLog(currentLog);
   //qDebug() << "MainWindow::defineStationCallsign (logQrz): " << logQRZ << endl;

    if (world->checkQRZValidFormat(logQRZ))
    {
        stationQRZ = logQRZ;
    }
    else
    {
        stationQRZ = mainQRZ;
    }
    myDataTabWidget->setData(myPower, stationQRZ, operatorQRZ, myLocator);

    //qDebug() << "MainWindow::defineStationCallsign: " << stationQRZ << endl;

}
/*
bool MainWindow::trueOrFalse(const QString _s)
{// reads a String and return true if s.upper()== TRUE :-)
    //qDebug() << "MainWindow::trueOrFalse: " << _s << endl;

    if ( (_s.toUpper()) == "TRUE")
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
*/
void MainWindow::slotSetPropMode(const QString _p)
{
    //qDebug() << "MainWindow::slotSetPropMode: " << _p << endl;
    //if(modify)
    //{
    //    return;
    //}
    othersTabWidget->setPropMode(_p);
    //int indexC = propModeComboBox->findText(" - " + _p + " - ", Qt::MatchContains);
    //propModeComboBox->setCurrentIndex(indexC);
}

/*
QString MainWindow::getPropModeFromComboBox()
{
    QString _pm = QString();
   //qDebug() << "MainWindow::getPropModeFromComboBox:" << propModeComboBox->currentText() << endl;
    _pm = (((propModeComboBox->currentText()).split('-')).at(1)).simplified();
   //qDebug() << "MainWindow::getPropModeFromComboBox: " << _pm << endl;
    if (_pm == "Not")
    {
        return QString();
    }
    return _pm;
}
*/

void MainWindow::completeWithPreviousQSO(const QString _call)
{
    //qDebug() << "MainWindow::completeWithPreviousQSO" << endl;
    //This function completes: Name, QTH, Locator, Entity, Iota
    if ((!completeWithPrevious) || (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    //if ( (_call.length()<=0) || (dataProxy->isWorkedB4(_call, -1)<=0))
    {
        if (completedWithPreviousName)
        {
            nameLineEdit->clear();
            completedWithPreviousName = false;
            nameLineEdit->setPalette(palBlack);
        }
        if (completedWithPreviousQTH)
        {
            qthLineEdit->clear();
            completedWithPreviousQTH = false;
            qthLineEdit->setPalette(palBlack);
         }
        if (completedWithPreviousLocator)
        {
            locatorLineEdit->clear();
            completedWithPreviousLocator = false;
            locatorLineEdit->setPalette(palBlack);
        }
        if (completedWithPreviousIOTA)
        {
            othersTabWidget->clearIOTA();
            //iotaContinentComboBox->setCurrentIndex(0);
            //iotaNumberLineEdit->setText("000");
            completedWithPreviousIOTA = false;
            //iotaNumberLineEdit->setPalette(palBlack);
        }
        if (completedWithPreviousQSLVia)
        {
            QSLTabWidget->setQSLVia("");

            //qslViaLineEdit->clear();
            completedWithPreviousQSLVia = false;
            //qslViaLineEdit->setPalette(palBlack);
        }
        return;
    }

    QString aux = QString();

    aux = dataProxy->getNameFromQRZ(_call);
    if ((aux.length()>=0) && ((nameLineEdit->text()).length()<=0) )
    {
        nameLineEdit->setPalette(palRed);
        completedWithPreviousName = true;
        nameLineEdit->setText(aux);
    }
    else if (completedWithPreviousName)
    {
        nameLineEdit->clear();
        completedWithPreviousName = false;
        nameLineEdit->setPalette(palBlack);
    }
    else
    {
    }

    aux = dataProxy->getQTHFromQRZ(_call);
    if ((aux.length()>=0) && ((qthLineEdit->text()).length()<=0) )
    {
        qthLineEdit->setPalette(palRed);
        completedWithPreviousQTH = true;
        qthLineEdit->setText(aux);
    }
    else if (completedWithPreviousQTH)
    {
        qthLineEdit->clear();
        completedWithPreviousQTH = false;
        qthLineEdit->setPalette(palBlack);

    }

    aux = dataProxy->getLocatorFromQRZ(_call);
    if ((aux.length()>=0) && ((locatorLineEdit->text()).length()<=0) )
    {
        locatorLineEdit->setPalette(palRed);
        locatorLineEdit->setText(aux);
        completedWithPreviousLocator=true;
    }
    else if (completedWithPreviousLocator)
    {
        locatorLineEdit->clear();
        completedWithPreviousLocator = false;
        locatorLineEdit->setPalette(palBlack);
    }

    aux = dataProxy->getIOTAFromQRZ(_call);
    //othersTabWidget->setIOTA(aux);

    if ((aux.length()>=0) && (othersTabWidget->isIOTAModified()) )
    {

        aux = awards->checkIfValidIOTA(aux);

        if ((aux.length())==6)
        {
            othersTabWidget->setIOTA(aux, false);
            //TODO: Decide if it is better this way or like in : void MainWindowInputQSL::setQSLVia(const QString _qs, QColor qColor)
            //QStringList values = aux.split("-", QString::SkipEmptyParts);
            //iotaContinentComboBox->setCurrentIndex( iotaContinentComboBox->findText(values.at(0) ) );
            //iotaNumberLineEdit->setPalette(palRed);
            //iotaNumberLineEdit->setText(values.at(1));
            completedWithPreviousIOTA=true;
        }
        else if (completedWithPreviousIOTA)
        {
            othersTabWidget->clearIOTA();
            //iotaContinentComboBox->setCurrentIndex(0);
            //iotaNumberLineEdit->setPalette(palBlack);
            //iotaNumberLineEdit->setText("000");
            completedWithPreviousName = false;
        }
    }
    else if (completedWithPreviousIOTA)
    {
        othersTabWidget->clearIOTA();
        //iotaContinentComboBox->setCurrentIndex(0);
        //iotaNumberLineEdit->setPalette(palBlack);
        //iotaNumberLineEdit->setText("000");
        completedWithPreviousIOTA = false;
    }

    aux = dataProxy->getQSLViaFromQRZ(_call);
    if ((aux.length()>=0) && ((QSLTabWidget->getQSLVia()).length()<=0) )
    {

        QSLTabWidget->setQSLVia(aux, Qt::red);

        //qslViaLineEdit->setPalette(palRed);
        //qslViaLineEdit->setText(aux);
        completedWithPreviousQSLVia=true;
    }
    else if (completedWithPreviousQSLVia)
    {
        QSLTabWidget->setQSLVia("");
        //qslViaLineEdit->clear();
        //completedWithPreviousQSLVia = false;
        //qslViaLineEdit->setPalette(palBlack);
    }
}

void MainWindow::slotSatBandTXComboBoxChanged(const QString _q)
{
    //qDebug() << "MainWindow::slotSatBandTXComboBoxChanged" << _q << endl;
    bandComboBox->setCurrentIndex(bandComboBox->findText(_q));
}

void MainWindow::slotFreqTXChanged()
{
    QString _q;
    int v = dataProxy->getBandIdFromFreq(txFreqSpinBox->value());
    if (v<0)
    {
        return;
    }

    _q = dataProxy->getNameFromBandId (v);
    bandComboBox->setCurrentIndex(bandComboBox->findText(_q));
}

void MainWindow::slotFreqRXChanged()
{
    QString _q;
    int v = dataProxy->getBandIdFromFreq(rxFreqSpinBox->value());
    int txv = dataProxy->getBandIdFromFreq(txFreqSpinBox->value());
    if ((v<0) || (txv>=0) )
    {
        //estoyaqui viendo como le doy prioridad a la TX freq
        return;
    }

    _q = dataProxy->getNameFromBandId (v);
    bandComboBox->setCurrentIndex(bandComboBox->findText(_q));
}


