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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class ClassWidget;
class CourseWidget;
class StudentWidget;
class TeacherWidget;
class TeachingWidget;
class ResultWidget;
class QuranWidget;
class CompetitionWidget;
class CompetitorWidget;
class JobWidget;
class EmployeeWidget;
class SettingWidget;

class QSqlTableModel;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:
    void changeEvent(QEvent *);

private slots:
    void initialize();

    void allocateWidgets();

    void populateListWidget();

    void populateStackWidget();

    void setControlFrameVisible(int);

    void on_addButton_clicked();

    void on_modifyButton_clicked();

    void on_removeButton_clicked();

    void on_printButton_clicked();

    void on_furqanButton_clicked();

private:
    Ui::MainWidget *ui;

    ClassWidget *m_classWidget;
    CourseWidget *m_courseWidget;
    StudentWidget *m_studentWidget;
    TeacherWidget *m_teacherWidget;
    TeachingWidget *m_teachingWidget;
    ResultWidget *m_resultWidget;
    QuranWidget *m_quranWidget;
    CompetitionWidget *m_competitionWidget;
    CompetitorWidget *m_competitorWidget;
    JobWidget *m_jobWidget;
    EmployeeWidget *m_employeeWidget;
    SettingWidget *m_settingWidget;
};

#endif // MAINWIDGET_H
