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

#include "maprelation.h"

#include <QMap>

MapRelation::MapRelation(const QMap<qlonglong, QString> *map, QObject *parent) :
    AbstractRelation(parent),
    m_map(map)
{}

QString MapRelation::forwardLookup(qlonglong key) const
{
    return m_map->value(key);
}

qlonglong MapRelation::backwardLookup(const QString &value) const
{
    return m_map->key(value);
}

AbstractRelation::RelationType MapRelation::type() const
{
    return Map;
}
const QMap<qlonglong, QString> *MapRelation::map() const
{
    return m_map;
}

