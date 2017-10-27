#ifndef SHOWERRORDIALOG_H
#define SHOWERRORDIALOG_H

/***************************************************************************
                          showerrordialog.h  -  description
                             -------------------
    begin                : oct 2017
    copyright            : (C) 2017 by Jaime Robles
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
#include <QDialog>
#include <QTextBrowser>

class ShowErrorDialog: public QDialog
{
    Q_OBJECT
public:
    ShowErrorDialog();
    ~ShowErrorDialog();
    void setText(const QString txt);

private slots:
    void slotAcceptButtonClicked();

private:
    void keyPressEvent(QKeyEvent *event);
     QTextBrowser *textBrowser;
     QString text;
     QLabel *txtLabel;

};


#endif // SHOWERRORDIALOG_H
