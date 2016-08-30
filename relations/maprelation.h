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

#ifndef MAPRELATION_H
#define MAPRELATION_H

#include "abstractrelation.h"

#include <QMap>

class MapRelation : public AbstractRelation
{
    Q_OBJECT

public:
    explicit MapRelation(const QMap<qlonglong, QString> *map, QObject *parent = 0);

    QString forwardLookup(qlonglong key) const;

    qlonglong backwardLookup(const QString &) const;

    RelationType type() const;

    const QMap<qlonglong, QString> *map() const;

private:
    const QMap<qlonglong, QString> *m_map;
};

#endif // MAPRELATION_H
