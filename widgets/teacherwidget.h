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

#ifndef TEACHERWIDGET_H
#define TEACHERWIDGET_H

#include <QWidget>

namespace Ui {
class TeacherWidget;
}

class TeacherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherWidget(QWidget *parent = 0);

    ~TeacherWidget();

    void add();

    void modify();

    void remove();

    void print();

private:
    Ui::TeacherWidget *ui;
};

#endif // TEACHERWIDGET_H
