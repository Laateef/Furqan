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

#include "competitorwidget.h"
#include "ui_competitorwidget.h"

#include "resourcemanager.h"
#include "sqldelegate.h"
#include "numericdelegate.h"
#include "printdialog.h"
#include "printoption.h"

#include <QSortFilterProxyModel>
#include <QScrollBar>

CompetitorWidget::CompetitorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompetitorWidget)
{
    ui->setupUi(this);

    m_competitorProxyModel = new QSortFilterProxyModel(this);
    m_competitorProxyModel->setSourceModel(RscMgr::modelHandler()->model(NS_TABLE::COMPETITOR));
    m_competitorProxyModel->setFilterKeyColumn(NS_COMPETITOR::COMPETITION);

    ui->competitorView->setModel(m_competitorProxyModel);
    ui->competitorView->hideColumn(NS_COMPETITOR::ID);
    ui->competitorView->hideColumn(NS_COMPETITOR::COMPETITION);
    ui->competitorView->setItemDelegateForColumn(NS_COMPETITOR::STUDENT, new SqlDelegate(true, ui->competitorView));
    ui->competitorView->setItemDelegateForColumn(NS_COMPETITOR::RANK, new NumericDelegate(new QIntValidator(0, INT_MAX, this), this));
    ui->competitorView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->competitorView->horizontalHeader()->setSectionResizeMode(NS_COMPETITOR::STUDENT, QHeaderView::Stretch);

    ui->competitionView->setModel(RscMgr::modelHandler()->model(NS_TABLE::COMPETITION));
    ui->competitionView->setModelColumn(NS_COMPETITION::NAME);
    ui->competitionView->setCurrentIndex(ui->competitionView->model()->index(0, NS_COMPETITION::NAME));
    ui->competitionView->setFixedWidth(ui->competitionView->minimumSizeHint().width()
                                       + ui->competitionView->verticalScrollBar()->sizeHint().width()
                                       + 7);

    connect(ui->competitionView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(adjustToCompetition()));
    connect(ui->competitionView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));

    adjustToCompetition();
}

CompetitorWidget::~CompetitorWidget()
{
    delete ui;
}

void CompetitorWidget::adjustToCompetition()
{
    m_competitorProxyModel->setFilterRegExp(QString("^%1$").arg(ui->competitionView->currentIndex().data().toString()));
}

void CompetitorWidget::refresh()
{
    RscMgr::modelHandler()->model(NS_TABLE::COMPETITOR)->select();
    adjustToCompetition();
}

void CompetitorWidget::add()
{
    if (!ui->competitionView->currentIndex().isValid()) {
        RscMgr::issueMessage(tr("Please select a competition first."), false);
        return;
    }

    QMap<int, QVariant> map;
    map.insert(NS_COMPETITOR::COMPETITION,
               ui->competitionView->currentIndex()
               .sibling(ui->competitionView->currentIndex().row(), NS_COMPETITION::ID).data());

    RscMgr::modelHandler()->insertRow(NS_TABLE::COMPETITOR, map);

    const QModelIndex &idx = ui->competitorView->model()->index(0, NS_COMPETITOR::STUDENT);

    ui->competitorView->setCurrentIndex(idx);
    ui->competitorView->edit(idx);
}

void CompetitorWidget::modify()
{
    if (!ui->competitorView->currentIndex().isValid())
        return;

    ui->competitorView->edit(ui->competitorView->currentIndex());
}

void CompetitorWidget::remove()
{
    if (!ui->competitorView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::COMPETITOR, ui->competitorView->currentIndex().row());

    ui->competitorView->setFocus();
    ui->competitorView->setCurrentIndex(ui->competitorView->model()->index(0, NS_COMPETITOR::STUDENT));
}

void CompetitorWidget::print()
{
    PrintOption opt;

    opt.setColumnRemoved(NS_COMPETITOR::ID, true);
    opt.setColumnHidden(NS_COMPETITOR::ID, true);
    opt.setColumnHidden(NS_COMPETITOR::COMPETITION, true);
    opt.setMajorColumn(NS_COMPETITOR::STUDENT);
    opt.setColumnAggregative(NS_COMPETITOR::COMPETITION, true);
    opt.setColumnAggregative(NS_COMPETITOR::RANK, true);
    opt.setCurrentAggregativeColumn(NS_COMPETITOR::COMPETITION);

    PrintDialog dlg(m_competitorProxyModel->sourceModel(), &opt);

    dlg.exec();
}
