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

#include "printoption.h"

PrintOption::PrintOption(QObject *parent) :
    QObject(parent),
    m_majorColumn(1),
    m_currentAggregativeColumn(-1)
{}

QString PrintOption::preamble() const
{
    return m_preamble;
}

void PrintOption::setPreamble(const QString &preamble)
{
    m_preamble = preamble;
}

QString PrintOption::commentary() const
{
    return m_commentary;
}

void PrintOption::setCommentary(const QString &commentary)
{
    m_commentary = commentary;
}

void PrintOption::setColumnHidden(int column, bool isHidden)
{
    m_hiddenColumnMap.insert(column, isHidden);
}

bool PrintOption::isColumnHidden(int column) const
{
    return m_hiddenColumnMap.value(column, false);
}

int PrintOption::hiddenColumnCount()
{
    return m_hiddenColumnMap.keys(true).size();
}

void PrintOption::setColumnAggregative(int column, bool isAggregative)
{
    m_aggregativeColumnMap.insert(column, isAggregative);
}

bool PrintOption::isColumnAggregative(int column) const
{
    return m_aggregativeColumnMap.value(column, false);
}

void PrintOption::setRowHidden(int row, bool isHidden)
{
    m_hiddenRowMap.insert(row, isHidden);
}

bool PrintOption::isRowHidden(int row) const
{
    return m_hiddenRowMap.value(row, false);
}

void PrintOption::setAggregationEntryHidden(const QString &entry, bool isHidden)
{
    m_hiddenAggregationEntryMap.insert(entry, isHidden);
}

bool PrintOption::isAggregationEntryHidden(const QString &entry) const
{
    return m_hiddenAggregationEntryMap.value(entry, false);
}

void PrintOption::setColumnRemoved(int column, bool isRemoved)
{
    m_removedColumnMap.insert(column, isRemoved);
}

bool PrintOption::isColumnRemoved(int column) const
{
    return m_removedColumnMap.value(column, false);
}

void PrintOption::setCurrentAggregativeColumn(int column)
{
    m_currentAggregativeColumn = column;

    m_hiddenAggregationEntryMap.clear();
}

int PrintOption::currentAggregativeColumn() const
{
    return m_currentAggregativeColumn;
}

void PrintOption::setMajorColumn(int column)
{
    m_majorColumn = column;
}

int PrintOption::majorColumn() const
{
    return m_majorColumn;
}
