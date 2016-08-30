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

#include "teacherwidget.h"
#include "ui_teacherwidget.h"

#include "resourcemanager.h"
#include "uniquedelegate.h"
#include "mapdelegate.h"
#include "datedelegate.h"
#include "printdialog.h"
#include "printOption.h"

TeacherWidget::TeacherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherWidget)
{
    ui->setupUi(this);

    ui->teacherView->setModel(RscMgr::modelHandler()->model(NS_TABLE::TEACHER));
    ui->teacherView->hideColumn(NS_TEACHER::ID);
    ui->teacherView->setItemDelegateForColumn(NS_TEACHER::FULL_NAME, new UniqueDelegate(ui->teacherView));
    ui->teacherView->setItemDelegateForColumn(NS_TEACHER::FATHER_NAME, new UniqueDelegate(ui->teacherView));
    ui->teacherView->setItemDelegateForColumn(NS_TEACHER::GENDER, new MapDelegate(ui->teacherView));
    ui->teacherView->setItemDelegateForColumn(NS_TEACHER::BIRTH_DATE, new DateDelegate(ui->teacherView));
    ui->teacherView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->teacherView->horizontalHeader()->setSectionResizeMode(NS_TEACHER::FULL_NAME, QHeaderView::Stretch);
    ui->teacherView->setCurrentIndex(ui->teacherView->model()->index(0, NS_TEACHER::FULL_NAME));
}

TeacherWidget::~TeacherWidget()
{
    delete ui;
}

void TeacherWidget::add()
{
    RscMgr::modelHandler()->insertRow(NS_TABLE::TEACHER);

    const QModelIndex &idx = ui->teacherView->model()->index(0, NS_TEACHER::FULL_NAME);

    ui->teacherView->setCurrentIndex(idx);
    ui->teacherView->edit(idx);
}

void TeacherWidget::modify()
{
    if (!ui->teacherView->currentIndex().isValid())
        return;

    ui->teacherView->edit(ui->teacherView->currentIndex());
}

void TeacherWidget::remove()
{
    if (!ui->teacherView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::TEACHER, ui->teacherView->currentIndex().row());

    ui->teacherView->setFocus();
    ui->teacherView->setCurrentIndex(ui->teacherView->model()->index(0, NS_TEACHER::FATHER_NAME));
}

void TeacherWidget::print()
{
    PrintOption opt;
    opt.setColumnRemoved(NS_TEACHER::ID, true);
    opt.setColumnHidden(NS_TEACHER::ID, true);
    opt.setMajorColumn(NS_TEACHER::FULL_NAME);
    opt.setColumnAggregative(NS_TEACHER::GENDER, true);
    opt.setColumnAggregative(NS_TEACHER::NATIONALITY, true);
    opt.setColumnAggregative(NS_TEACHER::RESIDENCY, true);

    PrintDialog dlg(ui->teacherView->model(), &opt);
    dlg.exec();
}
