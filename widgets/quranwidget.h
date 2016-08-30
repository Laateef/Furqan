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

#ifndef QURANWIDGET_H
#define QURANWIDGET_H

#include <QWidget>

class QSortFilterProxyModel;

namespace Ui {
class QuranWidget;
}

class QuranWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QuranWidget(QWidget *parent = 0);

    ~QuranWidget();

    void add();

    void modify();

    void remove();

    void print();

private slots:
    void adjustToStudent();

    void refresh();

private:
    Ui::QuranWidget *ui;

    QSortFilterProxyModel *m_quranProxyModel;
};

#endif // QURANWIDGET_H
