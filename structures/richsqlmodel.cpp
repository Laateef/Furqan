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

#include "richsqlmodel.h"

#include "abstractrelation.h"

RichSqlModel::RichSqlModel(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent, db)
{}

void RichSqlModel::setRelation(int column, AbstractRelation *relation)
{
    m_relationMap.insert(column, relation);
}

AbstractRelation *RichSqlModel::relation(int column) const
{
    return m_relationMap.value(column, NULL);
}

QVariant RichSqlModel::data(const QModelIndex &idx, int role) const
{
    if (role == Qt::DisplayRole) {
        if (m_relationMap.find(idx.column()) == m_relationMap.end()) {
            return QSqlTableModel::data(idx, role);
        } else {
            return m_relationMap.value(idx.column())->forwardLookup(QSqlTableModel::data(idx).toLongLong());
        }
    } else {
        return QSqlTableModel::data(idx, role);
    }
    return QVariant();
}
