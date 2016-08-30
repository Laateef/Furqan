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

#include "resultwidget.h"
#include "ui_resultwidget.h"

#include "resourcemanager.h"
#include "sqldelegate.h"
#include "numericdelegate.h"
#include "printdialog.h"
#include "printoption.h"

#include <QSortFilterProxyModel>
#include <QScrollBar>

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);

    m_resultProxyModel = new QSortFilterProxyModel(this);
    m_resultProxyModel->setSourceModel(RscMgr::modelHandler()->model(NS_TABLE::RESULT));
    m_resultProxyModel->setFilterKeyColumn(NS_RESULT::COURSE);

    ui->resultView->setModel(m_resultProxyModel);
    ui->resultView->hideColumn(NS_RESULT::ID);
    ui->resultView->hideColumn(NS_RESULT::COURSE);
    ui->resultView->setItemDelegateForColumn(NS_RESULT::STUDENT, new SqlDelegate(false, ui->resultView));

    QDoubleValidator *validator = new QDoubleValidator(0, 100, 2, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->resultView->setItemDelegateForColumn(NS_RESULT::RESULT, new NumericDelegate(validator, this));
    ui->resultView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->courseView->setModel(RscMgr::modelHandler()->model(NS_TABLE::COURSE));
    ui->courseView->setModelColumn(NS_COURSE::NAME);
    ui->courseView->setCurrentIndex(ui->courseView->model()->index(0, NS_COURSE::NAME));
    ui->courseView->setFixedWidth(ui->courseView->minimumSizeHint().width()
                                   + ui->courseView->verticalScrollBar()->sizeHint().width()
                                   + 7);

    connect(ui->courseView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(adjustToCourse()));
    //maintaining the foreign key's relations between tables
    connect(ui->courseView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));
    connect(RscMgr::modelHandler()->model(NS_TABLE::CLASS), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));
    connect(RscMgr::modelHandler()->model(NS_TABLE::STUDENT), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));

    adjustToCourse();
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::adjustToCourse()
{
    m_resultProxyModel->setFilterRegExp(QString("^%1$").arg(ui->courseView->currentIndex().data().toString()));
}

void ResultWidget::refresh()
{
    RscMgr::modelHandler()->model(NS_TABLE::RESULT)->select();
    adjustToCourse();
}

void ResultWidget::print()
{
    PrintOption opt;
    opt.setColumnRemoved(NS_RESULT::ID, true);
    opt.setColumnHidden(NS_RESULT::ID, true);
    opt.setColumnHidden(NS_RESULT::COURSE, true);
    opt.setMajorColumn(NS_RESULT::STUDENT);
    opt.setColumnAggregative(NS_RESULT::COURSE, true);
    opt.setColumnAggregative(NS_RESULT::STUDENT, true);
    opt.setCurrentAggregativeColumn(NS_RESULT::COURSE);

    PrintDialog dlg(m_resultProxyModel->sourceModel(), &opt);

    dlg.exec();
}
