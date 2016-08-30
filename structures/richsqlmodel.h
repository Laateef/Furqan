/*
 * Copyright (C) 30-8-2016 Abdullateef Qallaa
 *
 * This file is part of Furqan project,
 * hosted on GitHub: https://github.com/laateef/furqan,
 * licensed under GPLv3.
 *
 * GNU General Public License Usage
 * This file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.md included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 */

#ifndef RICHSQLMODEL_H
#define RICHSQLMODEL_H

#include <QSqlTableModel>

class AbstractRelation;

class RichSqlModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit RichSqlModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    void setRelation(int column, AbstractRelation *);

    AbstractRelation *relation(int column) const;

    QVariant data(const QModelIndex &idx, int role) const;

private:
    QMap<int, AbstractRelation *> m_relationMap;
    QList<int> m_hiddenColumnList;
    int m_columnCollapsed;
};

#endif // RICHSQLMODEL_H
