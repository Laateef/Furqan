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

#include "sqlrelation.h"

#include <QSqlTableModel>

SqlRelation::SqlRelation(const QSqlTableModel *remoteModel, int keyColumn, int displayColumn, QObject *parent) :
    AbstractRelation(parent),
    m_remoteModel(remoteModel),
    m_keyColumn(keyColumn),
    m_displayColumn(displayColumn)
{}

QString SqlRelation::forwardLookup(qlonglong key) const
{
    const QModelIndexList &list = m_remoteModel->match(m_remoteModel->index(0, m_keyColumn),
                                                       Qt::DisplayRole,
                                                       key,
                                                       1,
                                                       Qt::MatchExactly);

    if (list.isEmpty())
        return QString();

    return m_remoteModel->index(list.first().row(), m_displayColumn).data().toString();
}

qlonglong SqlRelation::backwardLookup(const QString &value) const
{
    const QModelIndexList &list = m_remoteModel->match(m_remoteModel->index(0, m_displayColumn),
                                                       Qt::DisplayRole,
                                                       value,
                                                       1,
                                                       Qt::MatchExactly);

    if (list.isEmpty())
        return -1;

    return m_remoteModel->index(list.first().row(), m_keyColumn).data().toLongLong();
}

AbstractRelation::RelationType SqlRelation::type() const
{
    return Sql;
}

const QSqlTableModel *SqlRelation::remoteModel() const
{
    return m_remoteModel;
}

int SqlRelation::keyColumn() const
{
    return m_keyColumn;
}

int SqlRelation::displayColumn() const
{
    return m_displayColumn;
}
