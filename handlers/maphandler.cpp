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

#include "maphandler.h"

#include "resourcemanager.h"

#include <QApplication>

MapHandler::MapHandler(QObject *parent) :
    QObject(parent)
{
    initialize();

    mapMaps();

    connect(RscMgr::translatorHandler(), SIGNAL(languageChanged()), SLOT(initialize()));
}

void MapHandler::initialize()
{
    m_genderMap.clear();
    m_genderMap.insert(NS_GENDER::NONE, tr("None"));
    m_genderMap.insert(NS_GENDER::MALE, tr("Male"));
    m_genderMap.insert(NS_GENDER::FEMALE, tr("Female"));

    m_levelMap.clear();
    m_levelMap.insert(NS_LEVEL::NONE, tr("None"));
    m_levelMap.insert(NS_LEVEL::BASIC, tr("Basic"));
    m_levelMap.insert(NS_LEVEL::INTERMEDIATE, tr("Intermediate"));
    m_levelMap.insert(NS_LEVEL::ADVANCED, tr("Advanced"));

    m_semesterMap.clear();
    m_semesterMap.insert(NS_SEMESTER::NONE, tr("None"));
    m_semesterMap.insert(NS_SEMESTER::FIRST, tr("First"));
    m_semesterMap.insert(NS_SEMESTER::SECOND, tr("Second"));

    m_competitionMap.clear();
    m_competitionMap.insert(NS_COMPETITION_TYPE::NONE, tr("None"));
    m_competitionMap.insert(NS_COMPETITION_TYPE::DAILY, tr("Daily"));
    m_competitionMap.insert(NS_COMPETITION_TYPE::WEEKLY, tr("Weekly"));
    m_competitionMap.insert(NS_COMPETITION_TYPE::QUARTERLY, tr("Quarterly"));

    m_recitalMap.clear();
    m_recitalMap.insert(NS_RECITAL::NONE, tr("None"));
    m_recitalMap.insert(NS_RECITAL::READING, tr("Reading"));
    m_recitalMap.insert(NS_RECITAL::MEMORIZING, tr("Memorizing"));
    m_recitalMap.insert(NS_RECITAL::REVIEWING, tr("Reviewing"));
}

void MapHandler::mapMaps()
{
    m_mapMap.clear();
    m_mapMap.insert(NS_MAP::GENDER, &m_genderMap);
    m_mapMap.insert(NS_MAP::LEVEL, &m_levelMap);
    m_mapMap.insert(NS_MAP::SEMESTER, &m_semesterMap);
    m_mapMap.insert(NS_MAP::COMPETITION, &m_competitionMap);
    m_mapMap.insert(NS_MAP::RECITAL, &m_recitalMap);
}

MapHandler *MapHandler::singleton()
{
    static MapHandler *mapHandler = NULL;

    if (!mapHandler)
        mapHandler = new MapHandler(qApp);

    return mapHandler;
}

const QMap<qlonglong, QString> *MapHandler::map(NS_MAP::MAP map) const
{
    return m_mapMap.value(map);
}
