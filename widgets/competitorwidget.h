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

#ifndef COMPETITORWIDGET_H
#define COMPETITORWIDGET_H

#include <QWidget>

class QSortFilterProxyModel;

namespace Ui {
class CompetitorWidget;
}

class CompetitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitorWidget(QWidget *parent = 0);

    ~CompetitorWidget();

    void add();

    void modify();

    void remove();

    void print();

private slots:
    void adjustToCompetition();

    void refresh();

private:
    Ui::CompetitorWidget *ui;

    QSortFilterProxyModel *m_competitorProxyModel;
};

#endif // COMPETITORWIDGET_H
