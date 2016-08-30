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

#include "jobwidget.h"
#include "ui_jobwidget.h"

#include "resourcemanager.h"
#include "uniquedelegate.h"
#include "printdialog.h"
#include "printoption.h"

JobWidget::JobWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JobWidget)
{
    ui->setupUi(this);

    ui->jobView->setModel(RscMgr::modelHandler()->model(NS_TABLE::JOB));
    ui->jobView->hideColumn(NS_JOB::ID);
    ui->jobView->setItemDelegateForColumn(NS_JOB::NAME, new UniqueDelegate(ui->jobView));
    ui->jobView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

JobWidget::~JobWidget()
{
    delete ui;
}

void JobWidget::add()
{
    RscMgr::modelHandler()->insertRow(NS_TABLE::JOB);

    const QModelIndex &idx = ui->jobView->model()->index(0, NS_JOB::NAME);

    ui->jobView->setCurrentIndex(idx);
    ui->jobView->edit(idx);
}

void JobWidget::modify()
{
    if (!ui->jobView->currentIndex().isValid())
        return;

    ui->jobView->edit(ui->jobView->currentIndex());
}

void JobWidget::remove()
{
    if (!ui->jobView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::JOB, ui->jobView->currentIndex().row());

    ui->jobView->setFocus();
    ui->jobView->setCurrentIndex(ui->jobView->model()->index(0, NS_JOB::NAME));
}

void JobWidget::print()
{
    PrintOption opt;

    opt.setColumnRemoved(NS_JOB::ID, true);
    opt.setColumnHidden(NS_JOB::ID, true);
    opt.setMajorColumn(NS_JOB::NAME);

    PrintDialog dlg(ui->jobView->model(), &opt);
    dlg.exec();
}
