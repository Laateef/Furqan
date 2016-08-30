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

#ifndef MAPHANDLER_H
#define MAPHANDLER_H

#include <QObject>
#include <QMap>

#include "maprelation.h"
#include "mapenumeration.h"

class MapHandler : public QObject
{
    Q_OBJECT

public:
    static MapHandler *singleton();

    const QMap<qlonglong, QString> *map(NS_MAP::MAP) const;

private slots:
    void mapMaps();

    void initialize();

private:
    explicit MapHandler(QObject *parent = 0);

    QMap<qlonglong, QString> m_genderMap;
    QMap<qlonglong, QString> m_levelMap;
    QMap<qlonglong, QString> m_semesterMap;
    QMap<qlonglong, QString> m_competitionMap;
    QMap<qlonglong, QString> m_recitalMap;

    QMap<NS_MAP::MAP, QMap<qlonglong, QString> *> m_mapMap;
};

#endif // MAPHANDLER_H
