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

#ifndef ABSTRACTRELATION_H
#define ABSTRACTRELATION_H

#include <QObject>

class AbstractRelation : public QObject
{
    Q_OBJECT

public:
    enum RelationType { Map, Sql };

    explicit AbstractRelation(QObject *parent = 0);

    virtual ~AbstractRelation();

    virtual RelationType type() const = 0;

    virtual QString forwardLookup(qlonglong key) const = 0;

    virtual qlonglong backwardLookup(const QString &) const = 0;
};

#endif // ABSTRACTRELATION_H
