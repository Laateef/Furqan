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

#include "competitionwidget.h"
#include "ui_competitionwidget.h"

#include "resourcemanager.h"
#include "uniquedelegate.h"
#include "mapdelegate.h"
#include "datedelegate.h"
#include "printdialog.h"
#include "printoption.h"

CompetitionWidget::CompetitionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompetitionWidget)
{
    ui->setupUi(this);

    ui->competitionView->setModel(RscMgr::modelHandler()->model(NS_TABLE::COMPETITION));
    ui->competitionView->hideColumn(NS_COMPETITION::ID);
    ui->competitionView->setItemDelegateForColumn(NS_COMPETITION::NAME, new UniqueDelegate(ui->competitionView));
    ui->competitionView->setItemDelegateForColumn(NS_COMPETITION::TYPE, new MapDelegate(ui->competitionView));
    ui->competitionView->setItemDelegateForColumn(NS_COMPETITION::DATE, new DateDelegate(ui->competitionView));
    ui->competitionView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->competitionView->horizontalHeader()->setSectionResizeMode(NS_COMPETITION::NAME, QHeaderView::Stretch);
}

CompetitionWidget::~CompetitionWidget()
{
    delete ui;
}

void CompetitionWidget::add()
{
    RscMgr::modelHandler()->insertRow(NS_TABLE::COMPETITION);

    const QModelIndex &idx = ui->competitionView->model()->index(0, NS_COMPETITION::NAME);

    ui->competitionView->setCurrentIndex(idx);
    ui->competitionView->edit(idx);
}

void CompetitionWidget::modify()
{
    if (!ui->competitionView->currentIndex().isValid())
        return;

    ui->competitionView->edit(ui->competitionView->currentIndex());
}

void CompetitionWidget::remove()
{
    if (!ui->competitionView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::COMPETITION, ui->competitionView->currentIndex().row());

    ui->competitionView->setFocus();
    ui->competitionView->setCurrentIndex(ui->competitionView->model()->index(0, NS_COMPETITION::NAME));
}

void CompetitionWidget::print()
{
    PrintOption opt;

    opt.setColumnRemoved(NS_COMPETITION::ID, true);
    opt.setColumnHidden(NS_COMPETITION::ID, true);
    opt.setMajorColumn(NS_COMPETITION::NAME);
    opt.setColumnAggregative(NS_COMPETITION::TYPE, true);
    opt.setCurrentAggregativeColumn(-1);

    PrintDialog dlg(ui->competitionView->model(), &opt);
    dlg.exec();
}
