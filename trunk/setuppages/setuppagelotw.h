#ifndef SETUPPAGELOTW_H
#define SETUPPAGELOTW_H
/***************************************************************************
                          setupplotw.h  -  description
                             -------------------
    begin                : feb 2020
    copyright            : (C) 2020 by Jaime Robles
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


#include <QtWidgets>
#include "utilities.h"

class SetupPageLoTW : public QWidget {
    Q_OBJECT

public:
    SetupPageLoTW(QWidget *parent=nullptr);
    ~SetupPageLoTW();


    void sePath(const QString &c);
    QString getPath();


private slots:
    void slotSelectTQSLClicked();
    //void slotClubLogActive(bool _s);
    //void slotUseStationCall(bool _s);

private:

    //QString defaultFileName;
    QPushButton *searchTQSLPushButton;
    QCheckBox *lotwActiveCheckBox;
    QLineEdit *tqslPathLineEdit;
    QLabel *pathLabel;

    Utilities *util;
};

#endif // SETUPPAGELOTW_H
