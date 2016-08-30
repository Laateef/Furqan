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

#ifndef SQLRELATION_H
#define SQLRELATION_H

#include "abstractrelation.h"

class QSqlTableModel;

class SqlRelation : public AbstractRelation
{
    Q_OBJECT

public:
    explicit SqlRelation(const QSqlTableModel *remoteModel,
                         int keyColumn, int displayColumn, QObject *parent);

    QString forwardLookup(qlonglong key) const;

    qlonglong backwardLookup(const QString &) const;

    RelationType type() const;

    const QSqlTableModel *remoteModel() const;

    int keyColumn() const;

    int displayColumn() const;

private:
    const QSqlTableModel *m_remoteModel;

    const int m_keyColumn;
    const int m_displayColumn;
};

#endif // SQLRELATION_H
