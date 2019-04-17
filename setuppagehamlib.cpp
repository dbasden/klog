#include "setuppagehamlib.h"

SetupPageHamLib::SetupPageHamLib(DataProxy *dp, QWidget *parent) : QWidget(parent)
{
    //qDebug() << "SetupPageHamLib::SetupPageHamLib" << endl;
    hamlib = new HamLibClass();
    activateHamlibCheckBox = new QCheckBox();
    rigTypeComboBox = new QComboBox();
    serialBaudsComboBox = new QComboBox();
    serialPortComboBox = new QComboBox();

    dataBitsComboBox = new QComboBox();
    flowControlComboBox = new QComboBox();
    parityComboBox = new QComboBox();
    stopBitsComboBox = new QComboBox();

    RTSCheckBox = new QCheckBox();
    DTRCheckBox = new QCheckBox();

    scanSerialPortButton = new QPushButton();

    //serialBaudsSpinBox = new QSpinBox;

    serialPort = new QLineEdit;

   strings.clear();
   //strings << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";


    createUI();
    setDefaults();

    //qDebug() << "SetupPageHamLib::SetupPageHamLib END" << endl;
}

void SetupPageHamLib::fillSerialPortsComboBox()
{
    //qDebug() << "SetupPageHamLib::fillSerialPortsComboBox" << endl;
    serialPortComboBox->clear();
    serialPortComboBox->addItems(getAvailableSerialPorts());
    //serialPortComboBox->setCurrentIndex(0);
}

void SetupPageHamLib::createUI()
{

    connect(scanSerialPortButton, SIGNAL(clicked(bool)), this, SLOT(slotScanPorts()) );

    activateHamlibCheckBox->setText(tr("Activate HamLib"));
    activateHamlibCheckBox->setToolTip(tr("Activates the hamlib support that will enable the connection to a radio."));


    RTSCheckBox->setText(tr("RTS on"));
    RTSCheckBox->setToolTip(tr("Setting RTS may be needed for some serial ports."));
    DTRCheckBox->setText(tr("DTR on"));
    DTRCheckBox->setToolTip(tr("Setting DTR may be needed for some serial ports."));


    rigTypeComboBox->clear();
    strings.clear();
    setRig();
    //serialPortComboBox->addItems(getAvailableSerialPorts());
    fillSerialPortsComboBox();
    rigTypeComboBox->setCurrentIndex(0);

    QLabel *rigTypeLabel = new QLabel(tr("Radio"));
    rigTypeLabel->setBuddy(rigTypeComboBox);
    rigTypeLabel->setToolTip(tr("Select your rig."));
    rigTypeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    rigTypeLabel->setEnabled(true);

    serialPortComboBox->addItems(serialPorts);
    QLabel *serialPortLabel = new QLabel(tr("Port"));
    serialPortLabel->setBuddy(serialPortComboBox);
    serialPortLabel->setToolTip(tr("Select the serial port. Only the serial ports that are detected are shown."));
    serialPortLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    serialPortLabel->setEnabled(true);

    scanSerialPortButton->setText(tr("Scan"));;
    scanSerialPortButton->setToolTip(tr("Click to identify the serial ports available in your computer."));

    strings.clear();
    strings << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    serialBaudsComboBox->addItems(strings);
    QLabel *serialBaudsLabel = new QLabel(tr("Bauds"));
    serialBaudsLabel->setBuddy(serialBaudsComboBox);
    serialBaudsLabel->setToolTip(tr("Select the serial port speed."));
    serialBaudsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    serialBaudsLabel->setEnabled(true);


    dataBitsComboBox = new QComboBox();
    strings.clear();
    strings << tr("5 bits") << tr("6 bits") << tr("7 bits") << tr("8 bits");
    dataBitsComboBox->addItems(strings);
    QLabel *dataBitsLabel = new QLabel(tr("Data bits"));
    dataBitsLabel->setBuddy(dataBitsComboBox);
    dataBitsLabel->setToolTip(tr("Select the serial data bits."));
    dataBitsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    dataBitsLabel->setEnabled(true);

    flowControlComboBox = new QComboBox();
    strings.clear();
    strings << tr("None") << tr("Hardware") << tr("Software XON/XOFF");
    flowControlComboBox->addItems(strings);
    QLabel *flowControlLabel = new QLabel(tr("Flow control"));
    flowControlLabel->setBuddy(flowControlComboBox);
    flowControlLabel->setToolTip(tr("Select the serial flow control"));
    flowControlLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    flowControlLabel->setEnabled(true);

    parityComboBox = new QComboBox();
    strings.clear();
    strings << tr("No parity") << tr("Even") << tr("Odd") << tr("Space") << tr("Mark") ;
    parityComboBox->addItems(strings);
    QLabel *parityLabel = new QLabel(tr("Parity"));
    parityLabel->setBuddy(parityComboBox);
    parityLabel->setToolTip(tr("Select the serial parity."));
    parityLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    parityLabel->setEnabled(true);


    stopBitsComboBox = new QComboBox();
    strings.clear();
    strings << tr("1 bit") << tr("1.5 bits") << tr("2 bits");
    stopBitsComboBox->addItems(strings);
    QLabel *stopBitsLabel = new QLabel(tr("Stop bits"));
    stopBitsLabel->setBuddy(stopBitsComboBox);
    stopBitsLabel->setToolTip(tr("Select the serial stop bits."));
    stopBitsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);
    stopBitsLabel->setEnabled(true);

    QHBoxLayout *checkLayout = new QHBoxLayout;
    checkLayout->addWidget(RTSCheckBox);
    checkLayout->addWidget(DTRCheckBox);

    QGridLayout *topData = new QGridLayout;
    topData->addWidget(rigTypeLabel, 0, 0);
    topData->addWidget(rigTypeComboBox, 0, 1);
    topData->addWidget(serialPortLabel, 1, 0);
    topData->addWidget(serialPortComboBox, 1, 1);
    topData->addWidget(scanSerialPortButton, 1, 2);
    topData->addWidget(serialBaudsLabel, 2, 0);
    topData->addWidget(serialBaudsComboBox, 2, 1);
    topData->addWidget(dataBitsLabel, 3, 0);
    topData->addWidget(dataBitsComboBox, 3, 1);
    topData->addWidget(stopBitsLabel, 4, 0);
    topData->addWidget(stopBitsComboBox, 4, 1);
    topData->addWidget(flowControlLabel, 5, 0);
    topData->addWidget(flowControlComboBox, 5, 1);
    topData->addWidget(parityLabel, 6, 0);
    topData->addWidget(parityComboBox, 6, 1);
    topData->addLayout(checkLayout, 7, 1);


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(topData, 0, 0);
    //mainLayout->addWidget(dataBitsGroupBox, 1, 0);
    //mainLayout->addWidget(stopBitsGroupBox, 1, 1);
    //mainLayout->addWidget(handShakeGroupBox, 2, 0);
    //mainLayout->addWidget(flowControlLineGroupBox, 2, 1);

    QVBoxLayout *mLayout = new QVBoxLayout;
    mLayout->addWidget(activateHamlibCheckBox);
    mLayout->addLayout(mainLayout);
    //mLayout->setAlignment(activateHamlibCheckBox, Qt::AlignHCenter | Qt::AlignTop);


    setLayout(mLayout);
}

void SetupPageHamLib::setRig()
{
    //qDebug() << "SetupPageHamLib::SetRig" << endl;
  // Rutine to fill the rig combo boxes
  // Do not display debug codes when load the rig's

    rigTypeComboBox->insertItems(0, hamlib->getRigList());
/*
    rig_set_debug (RIG_DEBUG_NONE);

  // and continue...
  rig_load_all_backends ();
  rig_list_foreach (addRigToList, this);
  strings.sort ();
  rigTypeComboBox->insertItems (0, strings);
  strings.clear ();
  */
}
/*
int SetupPageHamLib::addRigToList (const struct rig_caps *caps, void *data)
{
    //qDebug() << "SetupPageHamLib::addRigToList" << endl;
  QString name;
  SetupPageHamLib *r = (SetupPageHamLib *) data;
  name = caps->model_name;
  //qDebug() << "SetupPageHamLib::addRigToList: " << name << endl;
  //if (name == "Dummy")
  //  name = "None";
  //r->rigname2rigid[name] = caps->rig_model;
  //r->rigid2rigname[caps->rig_model] = name;
  r->strings << name;
  return -1;                    // not 0 --> we want all rigs
}
*/

QStringList SetupPageHamLib::getAvailableSerialPorts()
{
    QStringList st;
    st.clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        //st << info.portName();
        st << info.systemLocation();
    }
    return st;

}

void SetupPageHamLib::setDefaults()
{
    rigTypeComboBox->setCurrentIndex(0);
    serialPortComboBox->setCurrentIndex(0);
    serialBaudsComboBox->setCurrentIndex(0);
    dataBitsComboBox->setCurrentIndex(3);
    flowControlComboBox->setCurrentIndex(0);
    parityComboBox->setCurrentIndex(0);
    stopBitsComboBox->setCurrentIndex(0);

    RTSCheckBox->setChecked(false);
    DTRCheckBox->setChecked(false);

}

QString SetupPageHamLib::getData()
{
    //qDebug() << "SetupPageHamLib::getData" << endl;
    QString _output;
    _output.clear();
    QString _rigType, _serialPort, _baudsSpeed;//, dataBits, stopBits, handshake, flowControlLine;


    _rigType = rigTypeComboBox->currentText();
    _serialPort = serialPortComboBox->currentText();
    _baudsSpeed = serialBaudsComboBox->currentText();

     //qDebug() << "SetupPageHamLib::getData: " << _rigType << endl;


    _output.clear();
    if (activateHamlibCheckBox->isChecked())
    {        
        _output = _output + "Hamlib=True;\n";
    }
    else
    {

    }
    if (RTSCheckBox->isChecked())
    {
        _output = _output + "HamLibSerialRTS=True;\n";
    }
    if (DTRCheckBox->isChecked())
    {
        _output = _output + "HamLibSerialDTR=True;\n";
    }
    _output = _output + getDataBits() + ";\n";
    //qDebug() << "SetupPageHamLib::getData: dataBits: " << getDataBits() << endl;
    _output = _output + getStopBits() + ";\n";
    _output = _output + getFlowControl() + ";\n";
    _output = _output + getParity() + ";\n";


    //qDebug() << "SetupPageHamLib::getData: " << QString::number(hamlib->getModelIdFromName(_rigType)) << endl;
    _output = _output + "HamLibRigType=" + QString::number(hamlib->getModelIdFromName(_rigType)) + ";\n";
    //_output = _output + "HamLibRigType=" + _rigType + "\n";
    _output = _output + "HamlibSerialPort=" + _serialPort + ";\n";
    _output = _output + "HamlibSerialBauds=" + _baudsSpeed + ";\n";


    //qDebug() << "SetupPageHamLib::getData: " << _output << endl;
    return _output;
}

bool SetupPageHamLib::setRigType(const QString _radio)
{
    //qDebug() << "SetupPageHamLib::setRig: " << _radio << endl;

    int _index = rigTypeComboBox->findText(hamlib->getNameFromModelId(_radio.toInt()), Qt::MatchFlag::MatchExactly);
    if (_index >= 0)
    {
        rigTypeComboBox->setCurrentIndex(_index);
        return true;
    }
    else
    {
        rigTypeComboBox->setCurrentIndex(0);
    }
    return false;
}

bool SetupPageHamLib::setSerialPort(const QString _port)
{
    int _index = serialPortComboBox->findText(_port, Qt::MatchFlag::MatchExactly);
    if (_index >= 0)
    {
        serialPortComboBox->setCurrentIndex(_index);
        return true;
    }
    else
    {
        serialPortComboBox->setCurrentIndex(0);
    }
    return false;
}

bool SetupPageHamLib::setSerialSpeed(const QString _speed )
{
    int _index = serialBaudsComboBox->findText(_speed, Qt::MatchFlag::MatchExactly);
    if (_index >= 0)
    {
        serialBaudsComboBox->setCurrentIndex(_index);
        return true;
    }
    else
    {
        serialPortComboBox->setCurrentIndex(0);
    }
    return false;
}

bool SetupPageHamLib::setActive(const QString _active)
{
    if (_active.toUpper() == "TRUE")
    {
        activateHamlibCheckBox->setChecked(true);
    }
    else {
       activateHamlibCheckBox->setChecked(false);
    }
}

void SetupPageHamLib::slotScanPorts()
{
    //qDebug() << "SetupPageHamLib::slotScanPorts"  << endl;
    fillSerialPortsComboBox();
}

//DataBits { Data5, Data6, Data7, Data8, UnknownDataBits }
QString SetupPageHamLib::getDataBits()
{
    //qDebug() << "SetupPageHamLib::getDataBits"  << endl;
    int ret = dataBitsComboBox->currentIndex();
    QString output;
    output = "HamLibSerialDataBits=";
    switch (ret)
    {
        case 0:
            output = output + "5";
            //qDebug() << "SetupPageHamLib::getDataBits-0-5"  << endl;
        break;
        case 1:
            output = output + "6";
            //qDebug() << "SetupPageHamLib::getDataBits-1-6"  << endl;
        break;
        case 2:
            output = output + "7";
            //qDebug() << "SetupPageHamLib::getDataBits-2-7"  << endl;
        case 3:
            output = output + "8";
            //qDebug() << "SetupPageHamLib::getDataBits-3-8"  << endl;
        break;
        default:
        // should never be reached
            //qDebug() << "SetupPageHamLib::getDataBits-d-8"  << endl;
            output = output + "8";
        break;
    }
    return output;
}

void SetupPageHamLib::setDataBits(const QString _st)
{
    if (_st == "5")
    {
        dataBitsComboBox->setCurrentIndex(0);
    }
    else if (_st == "6")
    {
        dataBitsComboBox->setCurrentIndex(1);
    }
    else if (_st == "7")
    {
        dataBitsComboBox->setCurrentIndex(2);
    }
    else
    {
        dataBitsComboBox->setCurrentIndex(3);
    }
}

//FlowControl { NoFlowControl, HardwareControl, SoftwareControl, UnknownFlowControl }
QString SetupPageHamLib::getFlowControl()
{
    int ret = flowControlComboBox->currentIndex();
    QString output;
    output = "HamLibSerialFlowControl=";
    switch (ret)
    {
        case 0:
            output = output + "None";
        break;
        case 1:
            output = output + "Hardware";
            break;
        case 2:
            output = output + "Software";
            break;
        default:
        // should never be reached
            output = output + "None";
        break;
    }
    return output;
}
void SetupPageHamLib::setFlowControl(const QString _st)
{
    QString _s = _st.toUpper();
    if (_s == "HARDWARE")
    {
        flowControlComboBox->setCurrentIndex(1);
    }
    else if (_s == "SOFTWARE")
    {
        flowControlComboBox->setCurrentIndex(2);
    }
    else
    {
        flowControlComboBox->setCurrentIndex(0);
    }
}

//Parity { NoParity, EvenParity, OddParity, SpaceParity, MarkParity, UnknownParity }
QString SetupPageHamLib::getParity()
{
    int ret = parityComboBox->currentIndex();
    QString output;
    output = "HamLibSerialParity=";
    switch (ret)
    {
        case 0:
            output = output + "None";
        break;
        case 1:
            output = output + "Even";
            break;
        case 2:
            output = output + "Odd";
            break;
        case 3:
            output = output + "Space";
            break;
        case 4:
            output = output + "Mark";
            break;
        default:
        // should never be reached
            output = output + "None";
        break;
    }
    return output;
}
void SetupPageHamLib::setParity(const QString _st)
{
    QString _s = _st.toUpper();
    if (_s == "EVEN")
    {
        parityComboBox->setCurrentIndex(1);
    }
    else if (_s == "ODD")
    {
        parityComboBox->setCurrentIndex(2);
    }
    else if (_s == "SPACE")
    {
        parityComboBox->setCurrentIndex(3);
    }
    else if (_s == "MARK")
    {
        parityComboBox->setCurrentIndex(4);
    }
    else
    {
        parityComboBox->setCurrentIndex(0);
    }
}

//StopBits { OneStop, OneAndHalfStop, TwoStop, UnknownStopBits }

QString SetupPageHamLib::getStopBits()
{
    int ret = stopBitsComboBox->currentIndex();
    QString output;
    output = "HamLibSerialStopBits=";
    switch (ret)
    {
        case 0:
            output = output + "1";
        break;
        case 1:
            output = output + "1.5";
            break;
        case 2:
            output = output + "2";
            break;

        default:
        // should never be reached
            output = output + "1";
        break;
    }
    return output;
}
void SetupPageHamLib::setStopBits(const QString _st)
{
    if (_st == "1.5")
    {

        stopBitsComboBox->setCurrentIndex(1);
    }
    else if (_st == "2")
    {
        stopBitsComboBox->setCurrentIndex(2);
    }
    else
    {
        stopBitsComboBox->setCurrentIndex(0);
    }
}

void SetupPageHamLib::setRTS(const QString _state)
{
    if (_state.toUpper() == "TRUE")
    {
        RTSCheckBox->setChecked(true);
    }
    else
    {
        RTSCheckBox->setChecked(false);
    }
}

void SetupPageHamLib::setDTR(const QString _state)
{
    if (_state.toUpper() == "TRUE")
    {
        DTRCheckBox->setChecked(true);
    }
    else
    {
        DTRCheckBox->setChecked(false);
    }
}
