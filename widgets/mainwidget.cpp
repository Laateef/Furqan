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

#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "resourcemanager.h"
#include "classwidget.h"
#include "coursewidget.h"
#include "studentwidget.h"
#include "teacherwidget.h"
#include "teachingwidget.h"
#include "resultwidget.h"
#include "quranwidget.h"
#include "competitionwidget.h"
#include "competitorwidget.h"
#include "jobwidget.h"
#include "employeewidget.h"
#include "settingwidget.h"

#include <QSplashScreen>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // no need to initialize here beacause a ChangeEvent will be posted during the startup,
    // however, to for the main widget to show up complete at the very beginning we initialize it here.
    initialize();

    connect(ui->listWidget, SIGNAL(currentRowChanged(int)),
            ui->stackedWidget, SLOT(setCurrentIndex(int)));

    connect(ui->listWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(setControlFrameVisible(int)));

    setWindowTitle(tr("Furqan"));
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowState(Qt::WindowFullScreen);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        //the layout direction doesn't change automatically (maybe due to a bug),
        //so we manually change it.
        setLayoutDirection(RscMgr::dir());

        ui->retranslateUi(this);

        initialize();

        break;
    default:
        break;
    }
}

void MainWidget::setControlFrameVisible(int index)
{
    if (index == ui->listWidget->count() - 1) {
        ui->controlFrame->hide();
    } else {
        ui->controlFrame->show();
        ui->addButton->setVisible(ui->listWidget->currentRow() != NS_TABLE::RESULT);
        ui->modifyButton->setVisible(ui->listWidget->currentRow() != NS_TABLE::RESULT);
        ui->removeButton->setVisible(ui->listWidget->currentRow() != NS_TABLE::RESULT);
    }
}

void MainWidget::initialize()
{
    allocateWidgets();
    populateListWidget();
    populateStackWidget();

    ui->listWidget->setCurrentRow(0);
    ui->stackedWidget->setCurrentIndex(0);

    ui->listWidget->setMinimumWidth(ui->listWidget->sizeHintForColumn(0) + 7);
}

void MainWidget::allocateWidgets()
{
    m_classWidget = new ClassWidget;
    m_courseWidget = new CourseWidget;
    m_studentWidget = new StudentWidget;
    m_teacherWidget = new TeacherWidget;
    m_teachingWidget = new TeachingWidget;
    m_resultWidget = new ResultWidget;
    m_quranWidget = new QuranWidget;
    m_competitionWidget = new CompetitionWidget;
    m_competitorWidget = new CompetitorWidget;
    m_jobWidget = new JobWidget;
    m_employeeWidget = new EmployeeWidget;
    m_settingWidget = new SettingWidget;
}

void MainWidget::populateListWidget()
{
    //the listWidget might contain items,
    //actually this function would be invoked once the user requests to change language.
    ui->listWidget->clear();

    ui->listWidget->insertItem(NS_TABLE::CLASS, tr("Classes"));
    ui->listWidget->insertItem(NS_TABLE::COURSE, tr("Courses"));
    ui->listWidget->insertItem(NS_TABLE::STUDENT, tr("Students"));
    ui->listWidget->insertItem(NS_TABLE::TEACHER, tr("Teachers"));
    ui->listWidget->insertItem(NS_TABLE::TEACHING, tr("Teaching"));
    ui->listWidget->insertItem(NS_TABLE::RESULT, tr("Results of Courses"));
    ui->listWidget->insertItem(NS_TABLE::QURAN, tr("Studies of Quran"));
    ui->listWidget->insertItem(NS_TABLE::COMPETITION, tr("Competitions"));
    ui->listWidget->insertItem(NS_TABLE::COMPETITOR, tr("Competitors"));
    ui->listWidget->insertItem(NS_TABLE::JOB, tr("Jobs"));
    ui->listWidget->insertItem(NS_TABLE::EMPLOYEE, tr("Employees"));
    ui->listWidget->addItem(tr("Setting"));
}

void MainWidget::populateStackWidget()
{
    while (ui->stackedWidget->count() > 0) {
        QWidget *page = ui->stackedWidget->widget(ui->stackedWidget->count() - 1);
        ui->stackedWidget->removeWidget(page);
        page->close();
        page->deleteLater();
    }

    ui->stackedWidget->insertWidget(NS_TABLE::CLASS, m_classWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::COURSE, m_courseWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::STUDENT, m_studentWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::TEACHER, m_teacherWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::TEACHING, m_teachingWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::RESULT, m_resultWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::QURAN, m_quranWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::COMPETITION, m_competitionWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::COMPETITOR, m_competitorWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::JOB, m_jobWidget);
    ui->stackedWidget->insertWidget(NS_TABLE::EMPLOYEE, m_employeeWidget);
    ui->stackedWidget->addWidget(m_settingWidget);
}

// the result case is excluded because the database's trigger who inserts the corresponding
// rows when a new course or student is added to the database.
void MainWidget::on_addButton_clicked()
{
    switch (ui->listWidget->currentRow()) {
    case NS_TABLE::CLASS:       m_classWidget->add();        break;
    case NS_TABLE::COURSE:      m_courseWidget->add();       break;
    case NS_TABLE::STUDENT:     m_studentWidget->add();      break;
    case NS_TABLE::TEACHER:     m_teacherWidget->add();      break;
    case NS_TABLE::TEACHING:    m_teachingWidget->add();     break;
        //    case NS_TABLE::RESULT:      mResultWidget->add();       break;
    case NS_TABLE::QURAN:       m_quranWidget->add();        break;
    case NS_TABLE::COMPETITION: m_competitionWidget->add();  break;
    case NS_TABLE::COMPETITOR:  m_competitorWidget->add();   break;
    case NS_TABLE::JOB:         m_jobWidget->add();          break;
    case NS_TABLE::EMPLOYEE:    m_employeeWidget->add();     break;

    default:
        break;
    }
}

void MainWidget::on_modifyButton_clicked()
{
    switch (ui->listWidget->currentRow()) {
    case NS_TABLE::CLASS:       m_classWidget->modify();        break;
    case NS_TABLE::COURSE:      m_courseWidget->modify();       break;
    case NS_TABLE::STUDENT:     m_studentWidget->modify();      break;
    case NS_TABLE::TEACHER:     m_teacherWidget->modify();      break;
    case NS_TABLE::TEACHING:    m_teachingWidget->modify();     break;
        //    case NS_TABLE::RESULT:      mResultWidget->modify();       break;
    case NS_TABLE::QURAN:       m_quranWidget->modify();        break;
    case NS_TABLE::COMPETITION: m_competitionWidget->modify();  break;
    case NS_TABLE::COMPETITOR:  m_competitorWidget->modify();   break;
    case NS_TABLE::JOB:         m_jobWidget->modify();          break;
    case NS_TABLE::EMPLOYEE:    m_employeeWidget->modify();     break;

    default:
        break;
    }
}

void MainWidget::on_removeButton_clicked()
{
    switch (ui->listWidget->currentRow()) {
    case NS_TABLE::CLASS:       m_classWidget->remove();        break;
    case NS_TABLE::COURSE:      m_courseWidget->remove();       break;
    case NS_TABLE::STUDENT:     m_studentWidget->remove();      break;
    case NS_TABLE::TEACHER:     m_teacherWidget->remove();      break;
    case NS_TABLE::TEACHING:    m_teachingWidget->remove();     break;
        //    case NS_TABLE::RESULT:      mResultWidget->remove();       break;
    case NS_TABLE::QURAN:       m_quranWidget->remove();        break;
    case NS_TABLE::COMPETITION: m_competitionWidget->remove();  break;
    case NS_TABLE::COMPETITOR:  m_competitorWidget->remove();   break;
    case NS_TABLE::JOB:         m_jobWidget->remove();          break;
    case NS_TABLE::EMPLOYEE:    m_employeeWidget->remove();     break;

    default:
        break;
    }
}

void MainWidget::on_printButton_clicked()
{
    switch (ui->listWidget->currentRow()) {
    case NS_TABLE::CLASS:       m_classWidget->print();        break;
    case NS_TABLE::COURSE:      m_courseWidget->print();       break;
    case NS_TABLE::STUDENT:     m_studentWidget->print();      break;
    case NS_TABLE::TEACHER:     m_teacherWidget->print();      break;
    case NS_TABLE::TEACHING:    m_teachingWidget->print();     break;
    case NS_TABLE::RESULT:      m_resultWidget->print();       break;
    case NS_TABLE::QURAN:       m_quranWidget->print();        break;
    case NS_TABLE::COMPETITION: m_competitionWidget->print();  break;
    case NS_TABLE::COMPETITOR:  m_competitorWidget->print();   break;
    case NS_TABLE::JOB:         m_jobWidget->print();          break;
    case NS_TABLE::EMPLOYEE:    m_employeeWidget->print();     break;

    default:
        break;
    }
}

void MainWidget::on_furqanButton_clicked()
{
    QSplashScreen *splashScreen = RscMgr::createSplashScreen();
    splashScreen->show();
}
