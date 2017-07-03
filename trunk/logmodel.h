#ifndef LOGMODEL_H
#define LOGMODEL_H
/***************************************************************************
                          logmodel.h  -  description
                             -------------------
    begin                : june 2017
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
#include <QDebug>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include "dataproxy.h"
#include "dataproxy_sqlite.h"


class LogModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    LogModel(QObject *parent);
    void createlogModel(const int _i);

private:
    void setColumnsToDX();
    //QSqlRelationalTableModel *logModel;
    DataProxy *dataProxy;
};

#endif // LOGMODEL_H


/*
class MyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MyModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};
*/
