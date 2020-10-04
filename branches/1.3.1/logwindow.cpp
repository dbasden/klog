/***************************************************************************
                          logwindow.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "logwindow.h"


LogWindow::LogWindow(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
      //qDebug() << "LogWindow::LogWindow: "  << endl;
    dataProxy = dp;
    //sortingThroughProxyModel = false;
    logModel = new LogModel(dataProxy, this);
    util = new Utilities;
    connect(logModel, SIGNAL(queryError(QString, QString, int, QString)), this, SLOT(slotQueryErrorManagement(QString, QString, int, QString)) );
    logView = new QTableView;
    //dxccStatusWidget = new DXCCStatusWidget(dataProxy);
    elogClublog = new eLogClubLog();    
    currentLog = -1;
    //proxyModel = new LogViewSortFilterProxyModel(this);


    awards = new Awards(dataProxy, Q_FUNC_INFO);

    createUI();
    createActions();
    setDefaultData();
       //qDebug() << "LogWindow::LogWindow: - END"  << endl;

}

LogWindow::~LogWindow()
{
//    emit clearError();
}
/*
void LogWindow::setProxyModel (const bool _p)
{
    sortingThroughProxyModel = _p;
    if (sortingThroughProxyModel)
    {
        logView->setModel(proxyModel);
        logView->setCurrentIndex(proxyModel->index(0, 0));
    }
    else
    {
        logView->setModel(logModel);
        logView->setCurrentIndex(logModel->index(0, 0));
    }
}
*/

void LogWindow::sortColumn(const int _c)
{
    //proxyModel->sort(_c);
    logModel->sort(_c, Qt::AscendingOrder);
}

void LogWindow::clear()
{
      //qDebug() << "LogWindow::clear "  << endl;
}

void LogWindow::createUI()
{
      //qDebug() << "LogWindow::createUI"  << endl;

    logView->setContextMenuPolicy(Qt::CustomContextMenu);
    logView->setSortingEnabled(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(logView);
    setLayout(layout);
}

void LogWindow::setDefaultData()
{

       //qDebug() << "LogWindow::setDefaultData"  << endl;    
}


void LogWindow::createlogPanel(const int _currentLog)
{
      //qDebug() << "LogWindow::createlogPanel: " << QString::number(_currentLog) << endl;
    currentLog = _currentLog;
    logModel->createlogModel(currentLog);
    //proxyModel->setSourceModel(logModel);
    logView->setModel(logModel);

    logView->setCurrentIndex(logModel->index(0, 0));

    //setProxyModel(false);

    //QString contestMode = dataProxy->getLogTypeOfUserLog(currentLog);
    setColumnsToDX();
    sortColumn(1);  //Initial sort by column 1 (date & time)

/*
    if (contestMode == "DX")
    {
           //qDebug() << "LogWindow::createlogPanel: DX"  << endl;
        setColumnsToDX();
    }
    else if (contestMode == "CQ-WW-SSB")
    {
           //qDebug() << "LogWindow::createlogPanel: CQ-WW-SSB"  << endl;
    }

    else
    {
        // THIS POINT SHOULD NOT BE REACHED. It means that there is a kind of contest not supported.
        // Maybe the way should be to move ALL the actions from DX here.
           //qDebug() << "LogWindow::createlogPanel: No log type found!"  << endl;
    }
*/

    //qDebug() << "LogWindow::createlogPanel " << logModel->record(0).field(1).value().toString() << endl;
    //logView->setItemDelegateForColumn(1, new ItemDelegate);
    //logView->setItemDelegate(new QSqlRelationalDelegate(this));

    logView->setSelectionMode(QAbstractItemView::SingleSelection);
    logView->setSelectionBehavior(QAbstractItemView::SelectRows);
    logView->resizeColumnsToContents();
    logView->horizontalHeader()->setStretchLastSection(true);
    logView->sortByColumn(1);
    /*
    if (sortingThroughProxyModel)
    {
        proxyModel->sort(1);
    }
    else
    {
        logView->sortByColumn(1);
    }
    */

}

void LogWindow::setColumnsToDX()
{
      //qDebug() << "LogWindow::setColumnsToDX"  << endl;
    QString stringQuery;
    stringQuery = QString("SELECT * FROM log LIMIT 1");
    QSqlQuery query;
    bool sqlOK = query.exec(stringQuery);
    if (!sqlOK)
    {
        emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number(), query.lastQuery());
    }
    QSqlRecord rec;
    rec = query.record(); // Number of columns
    int columns = rec.count();


    for (int i=0; i < columns; i++)
    {
        logView->setColumnHidden(i, true);
    }

    columns = rec.indexOf("qso_date");
    logView->setColumnHidden(columns, false);
    columns = rec.indexOf("call");
    logView->setColumnHidden(columns, false);
    columns = rec.indexOf("rst_sent");
    logView->setColumnHidden(columns, false);
    columns = rec.indexOf("rst_rcvd");
    logView->setColumnHidden(columns, false);
    columns = rec.indexOf("bandid");
    logView->setColumnHidden(columns, false);
    columns = rec.indexOf("modeid");
    logView->setColumnHidden(columns, false);
    columns = rec.indexOf("comment");
    logView->setColumnHidden(columns, false);
}

void LogWindow::refresh()
{
       //qDebug() << "LogWindow::refresh"  << endl;

    logModel->select();
}

void LogWindow::createActions()
{
    createActionsCommon();
}

void LogWindow::createActionsCommon()
{
    //LOG VIEW

        connect(logView, SIGNAL(customContextMenuRequested( const QPoint& ) ), this, SLOT(slotRighButtonFromLog( const QPoint& ) ) );
        connect(logView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickLog( const QModelIndex& ) ) );
}

void LogWindow::slotRighButtonFromLog(const QPoint& pos)
{
       //qDebug() << "LogWindow::slotshowRighButtonFromLog"  << endl;
    int row = (logView->indexAt(pos)).row();
    showMenuRightButtonFromLogCreateActions();
    rightButtonFromLogMenu(row);
    //TODO: To be added to the logWindow and create an action that emist the QSO id
}

void LogWindow::rightButtonFromLogMenu(const int trow)
{
       //qDebug() << "LogWindow::slotshowRighButtonFromLogMenu:  " << QString::number(trow) << endl;

    int _qsoID = ((logModel->index(trow, 0)).data(0)).toInt();
    //qDebug() << "LogWindow::slotshowRighButtonFromLogMenu:  QSOid: " << QString::number(_qsoID) << endl;
    bool qslReceived = isQSLReceived(_qsoID);
    bool qslSent = isQSLSent(_qsoID);
    QMenu menu(this);

    menu.addAction(delQSOFromLogAct);
    delQSOFromLogAct->setData(trow);
    menu.addAction(qsoToEditFromLogAct);
    qsoToEditFromLogAct->setData(trow);
    menu.addAction(checkQRZCOMFromLogAct);
    checkQRZCOMFromLogAct->setData(trow);
    menu.addAction(checkDXHeatFromLogAct);
    checkDXHeatFromLogAct->setData(trow);


        menu.addSeparator();
        if (qslSent)
        {
        }
        else
        {
            QMenu *menuSentQsl = menu.addMenu(tr("QSL Send"));
            menuSentQsl->addAction(qslSentViaBureauFromLogAct);
            menuSentQsl->addAction(qslSentViaDirectFromLogAct);
            qslSentViaBureauFromLogAct->setData(trow);
            qslSentViaDirectFromLogAct->setData(trow);
        }

        if (qslReceived)
        {
        }
        else
        {
            QMenu *menuRecQsl = menu.addMenu(tr("QSL Rcvd"));
            menuRecQsl->addAction(qslRecViaBureauFromLogAct);
            menuRecQsl->addAction(qslRecViaDirectFromLogAct);
            qslRecViaBureauFromLogAct->setData(trow);
            qslRecViaDirectFromLogAct->setData(trow);
        }
    menu.exec(QCursor::pos());
}

void LogWindow::slotDoubleClickLog(const QModelIndex & index)
{
       //qDebug() << "LogWindow::slotDoubleClickLog: Row: " << QString::number(index.row()) << "Column: " << QString::number(index.column()) << endl;

    int row = index.row();
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());
    int qsoID = ((logModel->index(row, Qt::DisplayRole)).data(0)).toInt();

       //qDebug() << "LogWindow::slotDoubleClickLog: n: " << QString::number (logModel->data(index, Qt::DisplayRole).toInt()) << endl;
       //qDebug() << "LogWindow::slotDoubleClickLog: emitted: " << QString::number (((logModel->index(row, Qt::DisplayRole)).data(0)).toInt()) << endl;

    emit actionQSODoubleClicked(qsoID);
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());

    //TODO: To be added to the logWindow and create an action that emist the QSO id to be edited

    logModel->select();
}

bool LogWindow::isQSLReceived(const int _qsoId)
{
       //qDebug() << "LogWindow::isQSLReceived: " << QString::number(_qsoId) << endl;
    return dataProxy->isQSLReceived(_qsoId);
}

bool LogWindow::isQSLSent(const int _qsoId)
{
       //qDebug() << "LogWindow::isQSLSent: " << QString::number(_qsoId) << endl;

    return dataProxy->isQSLSent(_qsoId);
}

void LogWindow::showMenuRightButtonFromLogCreateActions()
{
   //qDebug() << "LogWindow::showMenuRightButtonFromLogCreateActions" << endl;

    delQSOFromLogAct = new QAction(tr("&Delete"), this);
    delQSOFromLogAct->setShortcut(Qt::CTRL + Qt::Key_D);
    delQSOFromLogAct->setStatusTip(tr("Delete a QSO"));
    connect(delQSOFromLogAct, SIGNAL(triggered()), this, SLOT(slotQsoDeleteFromLog()));

    qsoToEditFromLogAct = new QAction(tr("&Edit QSO"), this);
    qsoToEditFromLogAct->setShortcut(Qt::CTRL + Qt::Key_E);
    qsoToEditFromLogAct->setStatusTip(tr("Edit this QSO"));
    connect(qsoToEditFromLogAct, SIGNAL(triggered()), this, SLOT(slotQSOToEditFromLog()));

    qslSentViaBureauFromLogAct = new QAction(tr("Via &bureau"), this);
    qslSentViaBureauFromLogAct->setShortcut(Qt::CTRL + Qt::Key_B);
    qslSentViaBureauFromLogAct->setStatusTip(tr("Send this QSL via bureau"));
    connect(qslSentViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaBureauFromLog() ));

    qslSentViaDirectFromLogAct = new QAction(tr("D&irect"), this);
    qslSentViaDirectFromLogAct->setShortcut(Qt::CTRL + Qt::Key_I);
    qslSentViaDirectFromLogAct->setStatusTip(tr("Send this QSL via direct"));
    connect(qslSentViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLSentViaDirectFromLog()   ));

    qslRecViaBureauFromLogAct = new QAction(tr("Via bureau"), this);
    qslRecViaBureauFromLogAct->setShortcut(Qt::CTRL + Qt::Key_R);
    qslRecViaBureauFromLogAct->setStatusTip(tr("QSL &received via bureau"));
    connect(qslRecViaBureauFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaBureauFromLog() ));

    qslRecViaDirectFromLogAct = new QAction(tr("Direct"), this);
    qslRecViaDirectFromLogAct->setShortcut(Qt::CTRL + Qt::Key_T);
    qslRecViaDirectFromLogAct->setStatusTip(tr("QSL received via direc&t"));
    connect(qslRecViaDirectFromLogAct, SIGNAL(triggered()), this, SLOT( slotQSLRecViaDirectFromLog() ));

    checkQRZCOMFromLogAct = new QAction(tr("Check in QRZ.com"), this);
    checkQRZCOMFromLogAct->setShortcut(Qt::CTRL + Qt::Key_Q);
    checkQRZCOMFromLogAct->setStatusTip(tr("Check this callsign in QRZ.com"));
    connect(checkQRZCOMFromLogAct, SIGNAL(triggered()), this, SLOT( slotCheckQRZCom() ));

    checkDXHeatFromLogAct = new QAction(tr("Check in DXHeat.com"), this);
    checkDXHeatFromLogAct->setShortcut(Qt::CTRL + Qt::Key_Q);
    checkDXHeatFromLogAct->setStatusTip(tr("Check this callsign in DXHeat.com"));
    connect(checkDXHeatFromLogAct, SIGNAL(triggered()), this, SLOT( slotCheckDXHeatCom() ));


}



void LogWindow::slotQSLSentViaBureauFromLog()
{
      //qDebug() << "LogWindow::slotQSLSentViaBureauFromLog: " << (qslSentViaBureauFromLogAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((logModel->index( ( (qslSentViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslSentViaBureau(_qsoId);
}

void LogWindow::slotQSLSentViaDirectFromLog()
{
       //qDebug() << "LogWindow::slotQSLSentViaDirectFromLog: " << (qslSentViaDirectFromLogAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
     int _qsoId = ((logModel->index( ( (qslSentViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    //dataProxy->qslSentViaDirect(_qsoId, (QDateTime::currentDateTime()).toString("yyyy-MM-dd"));
    dataProxy->qslSentViaDirect(_qsoId, QDate::currentDate());

}

void LogWindow::slotQSLRecViaBureauFromLog()
{
      //qDebug() << "LogWindow::slotQSLRecViaBureauFromLog: " << endl;

    int _qsoId = ((logModel->index( ( (qslRecViaBureauFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslRecViaBureau(_qsoId);
    //TODO: To be added to the logWindow and create an action that emist the QSO id
}

void LogWindow::slotQSLRecViaDirectFromLog()
{
       //qDebug() << "LogWindow::slotQSLRecViaDirectFromLog: " << (qslRecViaDirectFromLogAct->data()).toString() << " - Id = " << QString::number( ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt()) ) << endl;
    int _qsoId = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    qslRecViaDirect(_qsoId);
    // Mark Sent, Bureau, date, update log.
    //TODO: To be added to the logWindow and create an action that emist the QSO id
}

void LogWindow::slotQSOToEditFromLog()
{
       //qDebug() << "slotQSOToEditFromLog: " << (qsoToEditFromLogAct->data()).toString() << endl;

    //qsoToEdit((logModel->index((qsoToEditFromLogAct->data()).toInt(), 0)).data(0).toInt());
    int QSOid = ((logModel->index((qsoToEditFromLogAct->data()).toInt(), 0)).data(0)).toInt();
    /*
    int row = index.row();
    int qsoID = ((logModel->index(row, Qt::DisplayRole)).data(0)).toInt();
    emit actionQSODoubleClicked(qsoID);
    */

    //int row = index.row();
    //qsoToEdit((logModel->index(row, 0)).data(0).toInt());
    emit actionQSODoubleClicked(QSOid);



    //TODO: To be added to the logWindow and create an action that emit the QSO id
}



void LogWindow::deleteQSO(const int _qsoID)
{
       //qDebug() << "LogWindow::deleteQSO: " << QString::number(_qsoID) << endl;
    elogClublog->deleteQSO(dataProxy->getClubLogRealTimeFromId(_qsoID));
    dataProxy->deleteQSO(_qsoID);

    //logModel->removeRow((delQSOFromLogAct->data()).toInt()); //TODO: This has been replaced by the previous line
    //awards->recalculateAwards();
    refresh();       
    //dxccStatusWidget->refresh();
    emit updateAwards();
    emit updateSearchText();

}

void LogWindow::slotQsoDeleteFromLog()
{
      //qDebug() << "LogWindow::slotQsoDeleteFromLog: " << (delQSOFromLogAct->data()).toString() << endl;
    //TODO: To be added to the logWindow and create an action that emist the QSO id

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("You have requested to delete this QSO."));
    msgBox.setInformativeText(tr("Are you sure?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    int QSOid = ((logModel->index((delQSOFromLogAct->data()).toInt(), 0)).data(0)).toInt();

      //qDebug() << "LogWindow::slotQsoDeleteFromLog (id): " << QString::number(QSOid) << endl;

    switch (ret) {
      case QMessageBox::Yes:
          //qDebug() << "LogWindow::slotQsoDeleteFromLog (id): -1" << endl;
        deleteQSO(QSOid);

          break;
      case QMessageBox::No:
          // No was clicked
          break;
      default:
          // should never be reached
          break;
    }
}

void LogWindow::qslSentViaBureau(const int _qsoId)
{
       //qDebug() << "LogWindow::qslSentViaBureau: " << QString::number(_qsoId)yyyy-MM-dd
    dataProxy->qslSentViaBureau(_qsoId, QDate::currentDate());
}

void LogWindow::qslRecViaBureau(const int _qsoId)
{
   //    //qDebug() << "LogWyyyy-MM-ddRecViaBureau: " << QString::number(_qsoIyyyy-MM-dd<< (dateTime->currentDateTime()).toString("yyyy/MM/dd") << endl;
    dataProxy->qslRecViaBureau(_qsoId, QDate::currentDate(), false);
    awards->setAwards(_qsoId);   //Update the Award status

    refresh();
    emit updateAwards();
}


void LogWindow::qslRecViaDirect(const int _qsoId)
{
       //qDebug() << "LogWindow::qslRecViaDirect: " << QString::number(_qsoId)yyyy-MM-dd
    dataProxy->qslRecViaDirect(_qsoId, QDate::currentDate(), false);
    awards->setAwards(_qsoId);

    refresh();
    emit updateAwards();
}


void LogWindow::slotQueryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery)
{
    emit queryError(functionFailed, errorCodeS, errorCodeN, failedQuery);
}

void LogWindow::slotCheckQRZCom()
{
    QString _qrz = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 2)).data(Qt::DisplayRole).toString());
    //int _qsoId = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    //QString _qrz = dataProxy->getCallFromId(_qsoId);
      //qDebug() << "LogWindow::sloTCheckQRZCom: " << _qrz << endl;
    QString url = "https://www.qrz.com/db/" + _qrz;

    QDesktopServices::openUrl(QUrl(url));

}

void LogWindow::slotCheckDXHeatCom()
{
    QString _qrz = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 2)).data(Qt::DisplayRole).toString());
    //int _qsoId = ((logModel->index( ( (qslRecViaDirectFromLogAct->data()).toInt()  ) , 0)).data(0).toInt());
    //QString _qrz = dataProxy->getCallFromId(_qsoId);
      //qDebug() << "LogWindow::slotCheckDXHeatCom(): " << _qrz << endl;
    QString url = "https://www.dxheat.com/db/" + _qrz;
    QDesktopServices::openUrl(QUrl(url));
}

