#ifndef SETUPPAGEMISC_H
#define SETUPPAGEMISC_H
/***************************************************************************
                          setuppagemisc.h  -  description
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

//#include <QtWidgets>
#include <QtGui>
class SetupPageMisc : public QWidget {
    Q_OBJECT

public:
    SetupPageMisc(QWidget *parent=0);
    ~SetupPageMisc();


    QString getRealTime();
    void setRealTime(const QString t);
    QString getUTCTime();
    void setUTCTime(const QString t);
    QString getAlwaysADIF();
    void setAlwaysADIF(const QString t);
    QString getDefaultFileName();
    void setUseDefaultName(const QString t);
    QString getUseDefaultName();
    void setDefaultFileName(const QString t);
    //QString getInMemory();
    //void setInMemory(const QString t);
    QString getImperial();
    void setImperial(const QString t);
    QString getSendQSLWhenRec();
    void setSendQSLWhenRec(const QString t);
    QString getShowStationCallSignInSearch();
    void setShowStationCallSignInSearch(const QString t);
    QString getKeepMyData();
    void setKeepMyData(const QString t);
    QString getCompleteWithPrevious();
    void setCompleteWithPrevious(const QString t);

private slots:
    void slotOpenFileButtonClicked();
    void slotUseDefaultButtonStateChanged(int state);



private:
    void createActions();

    QCheckBox *realTimeCheckbox, *UTCCheckbox, *alwaysADIFCheckBox, *useDefaultName, *completeWithPreviousCheckBox;
    QCheckBox *imperialCheckBox, *sendQSLWhenRecCheckBox, *showStationCallWhenSearchCheckBox, *keepMyDataCheckBox;
    QString defaultFileName;
    QLineEdit *defaultFileNameLineEdit;
    QPushButton *fileNameButton;

    QString kontestDir; //TODO: To be removed when the defaultDir is saved in the config file

    //QWidget *bandsWidget;

};


#endif // SETUPPAGEMISC_H
