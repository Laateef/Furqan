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

#ifndef PRINTOPTION_H
#define PRINTOPTION_H

#include <QObject>
#include <QMap>

class PrintOption : public QObject
{
    Q_OBJECT

public:
    explicit PrintOption(QObject *parent = 0);

public slots:
    QString preamble() const;
    void setPreamble(const QString &preamble);

    QString commentary() const;
    void setCommentary(const QString &commentary);

    void setColumnHidden(int column, bool isHidden);
    bool isColumnHidden(int column) const;

    int hiddenColumnCount();

    void setColumnAggregative(int column, bool isAggregative);
    bool isColumnAggregative(int column) const;

    void setRowHidden(int row, bool isHidden);
    bool isRowHidden(int row) const;

    void setAggregationEntryHidden(const QString &entry, bool isHidden);
    bool isAggregationEntryHidden(const QString &) const;

    void setColumnRemoved(int column, bool isRemoved);
    bool isColumnRemoved(int column) const;

    void setCurrentAggregativeColumn(int column);
    int currentAggregativeColumn() const;

    void setMajorColumn(int column);
    int majorColumn() const;

private:
    QString m_preamble;
    QString m_commentary;

    QMap<int, bool> m_hiddenRowMap;
    QMap<int, bool> m_hiddenColumnMap;
    QMap<int, bool> m_aggregativeColumnMap;
    QMap<int, bool> m_removedColumnMap;

    QMap<QString, bool> m_hiddenAggregationEntryMap;

    int m_majorColumn;
    int m_currentAggregativeColumn;
};

#endif // PRINTOPTION_H
