#include "mainqsoentrywidget.h"

MainQSOEntryWidget::MainQSOEntryWidget(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
      //qDebug()<< "MainQSOEntryWidget::MainQSOEntryWidget " << endl;
    upAndRunning = false;
    dataProxy = dp;
    qrzLineEdit = new QLineEdit;
    bandComboBox = new QComboBox;
    modeComboBox = new QComboBox;
    dateEdit = new QDateEdit;
    timeEdit = new QTimeEdit;
    realtimeCheckBox = new QCheckBox;
    enabledCR = realtimeCheckBox->backgroundRole();
    OKButton = new QPushButton(tr("&Add"), this);
    clearButton = new QPushButton(tr("&Clear"), this);
    timer = new QTimer(this);
    util = new Utilities;
    realTime = true;
    duplicatedQSOSlotInSecs = 0;
    logSeverity = 7;  //7 Debug /0=emergency or no debug

    createUI();
    setInitialData();    
      //qDebug()<< "MainQSOEntryWidget::MainQSOEntryWidget: - END" << endl;
}

void MainQSOEntryWidget::createUI()
{
      //qDebug()<< "MainQSOEntryWidget::createUI" << endl;
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    qrzLineEdit->setToolTip(tr("Call of the QSO."));
    bandComboBox->setToolTip(tr("Band of the QSO."));
    modeComboBox->setToolTip(tr("Mode of the QSO."));
    dateEdit->setToolTip(tr("Date of the QSO."));
    timeEdit->setToolTip(tr("Time of the QSO."));
    OKButton->setToolTip(tr("Add the QSO to the log."));
    clearButton->setToolTip(tr("Clears the QSO entry."));
    realtimeCheckBox->setToolTip(tr("KLog will show real time if enabled."));

    QHBoxLayout *TimeLayout = new QHBoxLayout;
    TimeLayout->addWidget(dateEdit);
    TimeLayout->addWidget(timeEdit);
    TimeLayout->addWidget(realtimeCheckBox);

    QHBoxLayout *BandModeLayout = new QHBoxLayout;
    BandModeLayout->addWidget(bandComboBox);
    BandModeLayout->addWidget(modeComboBox);

    QHBoxLayout *QrzBandModeLayout = new QHBoxLayout;
    QrzBandModeLayout->addWidget(qrzLineEdit);
    QrzBandModeLayout->addLayout(BandModeLayout);

    qrzgroupBox = new QGroupBox(tr("Call"));
    qrzgroupBox->setFlat(true);
    QVBoxLayout *qrzvbox = new QVBoxLayout;
    qrzvbox->addLayout(QrzBandModeLayout);
    qrzgroupBox->setLayout(qrzvbox);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(OKButton);
    buttonsLayout->addWidget(clearButton);

    dateEdit->setDisplayFormat("yyyy-MM-dd");
    timeEdit->setDisplayFormat("HH:mm:ss");

    QGridLayout *widgetLayout = new QGridLayout;
    widgetLayout->addWidget(qrzgroupBox, 0, 0, 1, 0);
    widgetLayout->addLayout(TimeLayout, 1, 0);
    widgetLayout->addLayout(buttonsLayout,1, 1);

    setLayout(widgetLayout);

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);

    connect(qrzLineEdit, SIGNAL(returnPressed()), this, SLOT(slotOKButtonClicked() ) );
    connect(qrzLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQRZTextChanged() ) );
    connect(bandComboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(slotBandComboBoxChanged() ) ) ;
    connect(modeComboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(slotModeComboBoxChanged() ) ) ;    

    connect(OKButton, SIGNAL(clicked()), this, SLOT(slotOKButtonClicked() ) );
    connect(clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked() ) );
    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateTime()) );
    connect(realtimeCheckBox, SIGNAL(clicked()), this, SLOT(slotCheckBoxClicked()) );
      //qDebug()<< "MainQSOEntryWidget::createUI-END" << endl;

}

void MainQSOEntryWidget::slotCheckBoxClicked()
{

    if (realtimeCheckBox->isChecked())
    {
        realTime = true;
        timeEdit->setBackgroundRole(enabledCR);
    }
    else
    {
        realTime = false;
        timeEdit->setBackgroundRole(QPalette::BrightText);
    }
}

void MainQSOEntryWidget::setCleaning (const bool _c)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    cleaning = _c;
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

void MainQSOEntryWidget::slotQRZTextChanged()
{
    //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: " << qrzLineEdit->text() << " / Length: " << QString::number((qrzLineEdit->text()).size()) << "###### START ######" << endl;
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if ((qrzLineEdit->text()).length()<1)
    {
        slotClearButtonClicked();
        emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
        //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: QRZ <1 - END" << endl;
    return;
    }
    int cursorP = qrzLineEdit->cursorPosition();
    //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position: " << QString::number(cursorP) << endl;
    qrzLineEdit->setText((qrzLineEdit->text()).toUpper());
    if (cleaning)
    {
        //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: Cleaning - END" << endl;
        emit debugLog(Q_FUNC_INFO, "END-2", logSeverity);
        return;
    }

    if (qrzAutoChanging)
    {
        //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: qrzAutoChanging - END" << endl;
        qrzAutoChanging = false;
        emit debugLog(Q_FUNC_INFO, "END-3", logSeverity);
        return;
    }

    qrzAutoChanging = true;

   //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position.1: " << QString::number(cursorP) << endl;

    if ( (qrzLineEdit->text()).endsWith(' ') )
    {/*Remove the space and moves the focus to SRX to write the RX exchange*/
        previousQRZ = (qrzLineEdit->text()).simplified();
        qrzLineEdit->setText(previousQRZ);
        //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: Space detected" << endl;
    }

    //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: Simplifiying & Capitalizing" << endl;
    qrzLineEdit->setText(((qrzLineEdit->text())).simplified());
    qrzLineEdit->setText((qrzLineEdit->text()).remove(" "));


    //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: checking for invalid chars" << endl;
    //TODO: This validCharactersInCall may be removed?
    InValidCharsInPrevCall = validCharactersInCall(qrzLineEdit->text());

    if (!util->isValidCall(qrzLineEdit->text()))
    {
        qrzLineEdit->setPalette(palRed);
        //emit showInfoLabel(tr("QRZ not valid"));
        //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: QRZ not valid - END" << endl;
        emit debugLog(Q_FUNC_INFO, "END-4", logSeverity);
        //return;
    }
    else
    {
        qrzLineEdit->setPalette(palBlack);
        //emit showInfoLabel(tr(""));
    }
    /*
      //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: checking for length" << endl;
    if (((qrzLineEdit->text()).length() < 1))
    { // If QRZ box is blank, Information labels should be cleared.
        slotClearButtonClicked();
        emit debugLog(Q_FUNC_INFO, "END-5", logSeverity);
        return;
    }
    */

    //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: checking for modify or length<1" << endl;
    if (qrzSmallModDontCalculate)
    //if ((modify) || ((qrzLineEdit->text()).length() < 1) || (qrzSmallModDontCalculate))
    {
        //qDebug() << "MainQSOEntryWidget::slotQRZTextChanged: qrzSmallModDontCalculate < 1" << endl;
        qrzSmallModDontCalculate=false;
        emit debugLog(Q_FUNC_INFO, "END-6", logSeverity);
        return;
    }

    //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: running..." << endl;
    qrzSmallModDontCalculate = true; // A kind of flag to prevent multiple calls to this method.
    currentQrz = qrzLineEdit->text();

    if ((currentQrz).count('\\'))
    { // Replaces \ by / to ease operation.
        currentQrz.replace(QChar('\\'), QChar('/'));
        qrzLineEdit->setText(currentQrz);
    }

    currentQrz = qrzLineEdit->text();
    //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position.3: " << QString::number(cursorP) << endl;
    if (cursorP>currentQrz.length())
    {// A Space that has been removed without updating the cursor
         //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursorP > currentQRZ.length" << endl;
    }
    else
    {
        if (cursorP==0)
        {

        }
        else if ((currentQrz.at(cursorP-1)).isSpace())
        {
            //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position.5: " << QString::number(cursorP) << endl;
            previousQRZ = currentQrz.remove(cursorP-1, 1);
            //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position.6: " << QString::number(cursorP) << endl;
            cursorP--;
            //qDebug()<< "MainQSOEntryWidget::slotQRZTextChanged: cursor position.7: " << QString::number(cursorP) << endl;
            qrzLineEdit->setText(previousQRZ);
        }
    }

    currentQrz = qrzLineEdit->text();
    //qDebug() << "MainQSOEntryWidget::slotQRZTextChanged: Emitting: " << currentQrz << endl;
    emit currentQRZSignal(currentQrz);

    qrzSmallModDontCalculate = false; // If the text has not been modified in this method
    //qDebug() << "MainQSOEntryWidget::slotQRZTextChanged: cursorP at the end : " << QString::number(cursorP) << endl;
    qrzLineEdit->setCursorPosition(cursorP);
    checkIfDupe();
    qrzAutoChanging = false;
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
     //qDebug() << "MainQSOEntryWidget::slotQRZTextChanged: END" << endl;
}

void MainQSOEntryWidget::setCurrentQRZ(const QString &_qrz)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    currentQrz = _qrz;
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

void MainQSOEntryWidget::slotBandComboBoxChanged(){
      //qDebug() << "MainQSOEntryWidgetslotBandComboBoxChanged: " << endl;
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);

    emit bandChanged(bandComboBox->currentText());
    checkIfDupe();
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
      //qDebug() << "MainQSOEntryWidgetslotBandComboBoxChanged: END" << endl;
}

void MainQSOEntryWidget::slotModeComboBoxChanged()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);

    emit modeChanged(modeComboBox->currentText());
    checkIfDupe();
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

void MainQSOEntryWidget::slotOKButtonClicked()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if ((qrzLineEdit->text()).length()<1)
    {
        return;
    }
    emit OKClicked();
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

void MainQSOEntryWidget::slotClearButtonClicked()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    clear();
    emit clearForNextQSOSignal();
    checkIfDupe();
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

void MainQSOEntryWidget::clear()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    cleaning = true;

    OKButton->setText(tr("&Add"));
    qrzLineEdit->clear();
    qrzLineEdit->setFocus(Qt::OtherFocusReason);

    cleaning = false;
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

void MainQSOEntryWidget::setInitialData()
{
      //qDebug()<< "MainQSOEntryWidget::setInitialData" << endl;
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    //Default band/modes
    modify = false;
    qrzAutoChanging = false;
    InValidCharsInPrevCall = false;
    qrzSmallModDontCalculate=false;
    previousQRZ = QString();

    bands.clear();
    modes.clear();
    bands << "10M" << "15M" << "20M" << "40M" << "80M" << "160M";
    modes << "SSB" << "CW" << "RTTY";


    setBands(bands);
    setModes(modes);

    selectDefaultBand(true);
    selectDefaultMode(true);

    dateEdit->setDate(QDate::currentDate());
    timeEdit->setTime(QTime::currentTime());

    UTCTime = true;
    //realTime = true;

    timer->start(1000);
   emit debugLog(Q_FUNC_INFO, "END", logSeverity);
     //qDebug()<< "MainQSOEntryWidget::setInitialData-END" << endl;

}

void MainQSOEntryWidget::setBands(const QStringList _bands)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
      //qDebug()<< "MainQSOEntryWidget::setBands" << endl;
    bands.clear();
    bands = _bands;
    bands.removeDuplicates();
    bands = dataProxy->sortBandNamesBottonUp(bands);
    bandComboBox->clear();
    bandComboBox->addItems(bands);
    selectDefaultBand(true);
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
      //qDebug()<< "MainQSOEntryWidget::setBands-END" << endl;
}

void MainQSOEntryWidget::setModes(const QStringList _modes)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    modes.clear();
    modes = _modes;
    modes.removeDuplicates();
    modes.sort();
    modeComboBox->clear();
    modeComboBox->addItems(modes);
    selectDefaultMode(true);
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);

}

QStringList MainQSOEntryWidget::getModes()
{
    return modes;
}

bool MainQSOEntryWidget::setBand(const QString &_band)
{
      //qDebug() << "MainQSOEntryWidget::setBand: " << _band << endl;
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
     if (bandComboBox->findText(_band, Qt::MatchCaseSensitive) < 0)
     {
           //qDebug() << "MainQSOEntryWidget::setBand-1: Band not found " << _band << endl;
         emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
        return false;
     }
     else
     {
        bandComboBox->setCurrentIndex(bandComboBox->findText(_band, Qt::MatchCaseSensitive));
        emit debugLog(Q_FUNC_INFO, "END-2", logSeverity);

          //qDebug() << "MainQSOEntryWidget::setBand-2: " << _band << endl;
        return true;
     }

}

bool MainQSOEntryWidget::setMode(const QString &_mode)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << "MainQSOEntryWidget::setMode: " << _mode << endl;
    if (modeComboBox->findText(_mode, Qt::MatchCaseSensitive) < 0)
    {
        emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
        return false;
    }
    else
    {
        modeComboBox->setCurrentIndex(modeComboBox->findText(_mode, Qt::MatchCaseSensitive));
        emit debugLog(Q_FUNC_INFO, "END", logSeverity);
       return true;
    }
}

bool MainQSOEntryWidget::setQRZ(const QString &_qrz)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    //qDebug() << "MainQSOEntryWidget::setQRZ: " << _qrz << endl;
    //TODO: Add validations to prevent that non valid qrz are sent from the outside of this function or at least manage this appropriately.
    qrzLineEdit->setText(_qrz.toUpper());
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
    return false;
}

bool MainQSOEntryWidget::setDate(const QDateTime _date)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if (_date.isValid())
    {
        dateEdit->setDate(_date.date());
        timeEdit->setTime(_date.time());

        emit debugLog(Q_FUNC_INFO, "END", logSeverity);
        return true;
    }
    else
    {
        //qDebug() << "MainQSOEntryWidget::setDate - NO VALID DATE" << endl;
        emit debugLog(Q_FUNC_INFO, "END", logSeverity);
        return false;
    }
}

bool MainQSOEntryWidget::setTime(const QTime _time)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if (_time.isValid())
    {
        timeEdit->setTime(_time);
        emit debugLog(Q_FUNC_INFO, "END", logSeverity);
        return true;
    }
    else
    {
        emit debugLog(Q_FUNC_INFO, "END", logSeverity);
        return false;
    }
}

QString MainQSOEntryWidget::getQrz()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);    
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
    return (qrzLineEdit->text()).toUpper();
}

QString MainQSOEntryWidget::getBand(const int _b)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if (_b<0)
    {
        emit debugLog(Q_FUNC_INFO, "END", logSeverity);
        //qDebug() << "MainQSOEntryWidget::getBand(1): " << bandComboBox->currentText()<< endl;
        return bandComboBox->currentText();
    }
    else
    {
        if (bandComboBox->count()>=_b)
        {
            emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
            //qDebug() << "MainQSOEntryWidget::getBand(2): " << bandComboBox->currentText()<< endl;
            return bandComboBox->itemText(_b);
        }
        else
        {
            emit debugLog(Q_FUNC_INFO, "END-2", logSeverity);
            //qDebug() << "MainQSOEntryWidget::getBand(3): " << bandComboBox->currentText()<< endl;
            return bandComboBox->currentText();
        }
    }
}

QString MainQSOEntryWidget::getMode(const int _m)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if (_m<0)
    {
        emit debugLog(Q_FUNC_INFO, "END", logSeverity);
        return modeComboBox->currentText();
    }
    else
    {
        if (modeComboBox->count()>=_m)
        {
            emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
            return modeComboBox->itemText(_m);
        }
        else
        {
            emit debugLog(Q_FUNC_INFO, "END-2", logSeverity);
            return modeComboBox->currentText();
        }
    }
}

QDate MainQSOEntryWidget::getDate()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
    return dateEdit->date();
}


QDateTime MainQSOEntryWidget::getDateTime()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    //emit debugLog(Q_FUNC_INFO, "END", logSeverity);
    QDateTime dateTime;
    dateTime.setDate(dateEdit->date());
    dateTime.setTime(timeEdit->time());
    return dateTime;

}

QTime MainQSOEntryWidget::getTime()
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
    return timeEdit->time();
}

void MainQSOEntryWidget::setRealTime(const bool _realTime)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    realtimeCheckBox->setChecked(_realTime);
    //realTime = _realTime;
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}


void MainQSOEntryWidget::setUTC(const bool _utc)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    UTCTime = _utc;
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

void MainQSOEntryWidget::setModify(const bool _modify)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    modify = _modify;
    if (modify)
    {
        OKButton->setText(tr("&Modify"));
    }
    else
    {
        OKButton->setText(tr("&Add"));
    }
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

void MainQSOEntryWidget::slotUpdateTime()
{
    //qDebug()<< "MainQSOEntryWidget::slotUpdateTime" << endl;
    //emit debugLog(Q_FUNC_INFO, "Start", logSeverity);

    if ( (!modify) && (realtimeCheckBox->isChecked())  )
    {
        //qDebug()<< "MainQSOEntryWidget::slotUpdateTime - Real Time & update" << endl;

        if (UTCTime)
        {

            dateEdit->setDate(QDateTime::currentDateTime().toUTC().date());
            timeEdit->setTime(QDateTime::currentDateTime().toUTC().time());
        }
        else
        {

            dateEdit->setDate(QDateTime::currentDateTime().date());
            timeEdit->setTime(QDateTime::currentDateTime().time());
        }
    }

    //emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

bool MainQSOEntryWidget::validCharactersInCall(const QString &_qrz)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    for (int i = 0; i<_qrz.size();i++)
    {
        if (!( ((_qrz.at(i)).isLetterOrNumber()) || (_qrz.at(i)=='\\')  || (_qrz.at(i)=='/')  ))
        {
            emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
            return false;
        }

    }
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
    return true;
}

bool MainQSOEntryWidget::isModeExisting(const QString &_m)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if (modeComboBox->findText(_m, Qt::MatchCaseSensitive) >= 0)
    {
        emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
        return true;
    }
    else
    {
        emit debugLog(Q_FUNC_INFO, "END-2", logSeverity);
        return false;
    }

}

bool MainQSOEntryWidget::isBandExisting(const QString &_b)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if (bandComboBox->findText(_b, Qt::MatchCaseSensitive) >= 0)
    {
        emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
        return true;
    }
    else
    {
        emit debugLog(Q_FUNC_INFO, "END-2", logSeverity);
        return false;
    }
}

void MainQSOEntryWidget::setUpAndRunning(const bool _u)
{
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    upAndRunning = _u;
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
}

void MainQSOEntryWidget::selectDefaultBand(const bool _init)
{
      //qDebug() << "MainQSOEntryWidget::selectDefaultBand" << endl;
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if ((upAndRunning) || (!_init))
    {
        emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
          //qDebug() << "MainQSOEntryWidgetselectDefaultBand-END-1" << endl;
        return;
    }
    QString aux;
    aux = QString();
    int defaultBand = dataProxy->getMostUsedBand(-1); //TODO: The log could be defined here
      //qDebug() << "MainQSOEntryWidget::selectDefaultBand: " << QString::number(defaultBand) << dataProxy->getNameFromBandId (defaultBand)<< endl;
    if (defaultBand<1)
    {

        defaultBand = dataProxy->getIdFromBandName(getBand(0));

    }

    aux = dataProxy->getNameFromBandId(defaultBand);

    setBand(aux);
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);

      //qDebug() << "MainQSOEntryWidgetselectDefaultBand_END" << endl;
}

void MainQSOEntryWidget::selectDefaultMode(const bool _init)
{
      //qDebug() << "MainQSOEntryWidgetselectDefaultMode" << endl;
    emit debugLog(Q_FUNC_INFO, "Start", logSeverity);
    if ((upAndRunning) || (!_init))
    {
        emit debugLog(Q_FUNC_INFO, "END-1", logSeverity);
        return;
    }
    int defaultMode = dataProxy->getMostUsedMode(-1); //TODO: The log could be defined here
      //qDebug() << "MainQSOEntryWidgetselectDefaultMode: " << QString::number(defaultMode) << endl;

    if (defaultMode < 1)
    {
        defaultMode = dataProxy->getSubModeIdFromSubMode(getBand(0));
    }
    setMode(dataProxy->getNameFromSubModeId(defaultMode));
    //modeComboBox->setCurrentIndex(modeComboBox->findText(dataProxy->getNameFromSubModeId(defaultMode)));

     //qDebug() << "MainQSOEntryWidgetselectDefaultMode3: " << QString::number(defaultMode) << endl;
    emit debugLog(Q_FUNC_INFO, "END", logSeverity);
     //qDebug() << "MainQSOEntryWidgetselectDefaultMode-END" << endl;
}

void MainQSOEntryWidget::setDuplicatedQSOSlot (const int _secs)
{
    if (_secs >= 0)
    {
        duplicatedQSOSlotInSecs = _secs;
    }
}

void MainQSOEntryWidget::checkIfDupe()
{
    //qDebug() << Q_FUNC_INFO << endl;
    QDateTime _dateTime;
    _dateTime.setDate(dateEdit->date());
    _dateTime.setTime(timeEdit->time());

    if ((dataProxy->isThisQSODuplicated(Q_FUNC_INFO, qrzLineEdit->text(), _dateTime, dataProxy->getIdFromBandName(bandComboBox->currentText()), dataProxy->getIdFromModeName(modeComboBox->currentText()), duplicatedQSOSlotInSecs).length()<2) && !modify)
    {
        qrzgroupBox->setTitle(tr("Call"));
    }
    else
    {
        qrzgroupBox->setTitle(tr("DUPE", "Translator: DUPE is a common world for hams. Do not translate of not sure"));

    }
}
