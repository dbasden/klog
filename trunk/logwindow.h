#ifndef LOGWINDOW_H
#define LOGWINDOW_H

/***************************************************************************
                          logwindow.h  -  description
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
#include <QWidget>
//#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QAction>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include "dataproxy.h"
#include "logmodel.h"
#include "awards.h"
//#include "dxccstatuswidget.h"
#include "elogclublog.h"


class LogWindow : public  QWidget
{
    Q_OBJECT

public:
    explicit LogWindow(DataProxy *dp, QWidget *parent = 0);
    ~LogWindow();
    void createlogPanel(const int _currentLog);
    void clear();
    void refresh();
    void setCurrentLog(const int _currentLog);


    void qslSentViaBureau(const int _qsoId);    //Maybe this could be defined as private and call it with an action, if needed.
    void qslRecViaBureau(const int _qsoId);     //Maybe this could be defined as private and call it with an action, if needed.
    void qslRecViaDirect(const int _qsoId);

    bool isQSLReceived(const int _qsoId);
    bool isQSLSent(const int _qsoId);

signals:
    void actionQSODoubleClicked(const int _qsoid);
    void updateAwards();
    void updateSearchText();   
    //void qsoFound(const QStringList _qs); // Each: QString with format: Fieldname:value
    void queryError(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery); // To alert about any failed query execution
    //void clearError();

private slots:

    void slotDoubleClickLog(const QModelIndex & index);

    void slotRighButtonFromLog(const QPoint& pos);


    void slotQSLSentViaBureauFromLog();
    void slotQSLSentViaDirectFromLog();
    void slotQSLRecViaDirectFromLog();
    void slotQSLRecViaBureauFromLog();
    void slotQsoDeleteFromLog();
    void slotQSOToEditFromLog();
    void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, int errorCodeN, QString failedQuery);


private:    
    void createUI();
    void createActionsCommon();
    void createActions();

    void deleteQSO(const int _qsoID);
    void righButtonFromLogMenu(const int trow);
    void showMenuRightButtonFromLogCreateActions();


    void setDefaultData();
    void setColumnsToDX();


    DataProxy *dataProxy;
    LogModel *logModel;
    Awards *awards;
    //DXCCStatusWidget *dxccStatusWidget;
    eLogClubLog *elogClublog;

    QTableView *logView;
    QLabel *logLabel;

    QAction *delQSOFromLogAct;
    QAction *qsoToEditFromLogAct;
    QAction *qslSentViaBureauFromLogAct;
    QAction *qslSentViaDirectFromLogAct;
    QAction *qslRecViaBureauFromLogAct;
    QAction *qslRecViaDirectFromLogAct;

    int currentLog;
};



#endif // LOGWINDOW_H
