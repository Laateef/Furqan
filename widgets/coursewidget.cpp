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

#include "coursewidget.h"
#include "ui_coursewidget.h"

#include "resourcemanager.h"
#include "mapdelegate.h"
#include "uniquedelegate.h"
#include "printdialog.h"
#include "printoption.h"

CourseWidget::CourseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseWidget)
{
    ui->setupUi(this);

    ui->courseView->setModel(RscMgr::modelHandler()->model(NS_TABLE::COURSE));
    ui->courseView->hideColumn(NS_COURSE::ID);
    ui->courseView->setItemDelegateForColumn(NS_COURSE::NAME, new UniqueDelegate(ui->courseView));
    ui->courseView->setItemDelegateForColumn(NS_COURSE::SEMESTER, new MapDelegate(ui->courseView));
    ui->courseView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CourseWidget::~CourseWidget()
{
    delete ui;
}

void CourseWidget::add()
{
    QMap<int, QVariant> map;
    map.insert(NS_COURSE::SEMESTER, NS_SEMESTER::NONE);

    RscMgr::modelHandler()->insertRow(NS_TABLE::COURSE, map);

    const QModelIndex &idx = ui->courseView->model()->index(0, NS_COURSE::NAME);

    ui->courseView->setCurrentIndex(idx);
    ui->courseView->edit(idx);
}

void CourseWidget::modify()
{
    if (!ui->courseView->currentIndex().isValid())
        return;

    ui->courseView->edit(ui->courseView->currentIndex());
}

void CourseWidget::remove()
{
    if (!ui->courseView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::COURSE, ui->courseView->currentIndex().row());

    ui->courseView->setFocus();
    ui->courseView->setCurrentIndex(ui->courseView->model()->index(0, NS_COURSE::NAME));
}

void CourseWidget::print()
{
    PrintOption opt;

    opt.setColumnRemoved(NS_COURSE::ID, true);
    opt.setColumnHidden(NS_COURSE::ID, true);
    opt.setMajorColumn(NS_COURSE::NAME);
    opt.setColumnAggregative(NS_COURSE::SEMESTER, true);
    opt.setCurrentAggregativeColumn(-1);

    PrintDialog dlg(ui->courseView->model(), &opt);
    dlg.exec();
}
