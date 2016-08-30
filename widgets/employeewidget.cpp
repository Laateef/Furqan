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

#include "employeewidget.h"
#include "ui_employeewidget.h"

#include "resourcemanager.h"
#include "uniquedelegate.h"
#include "mapdelegate.h"
#include "datedelegate.h"
#include "printdialog.h"
#include "printoption.h"

#include <QSortFilterProxyModel>
#include <QScrollBar>

EmployeeWidget::EmployeeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmployeeWidget)
{
    ui->setupUi(this);

    m_employeeProxyModel = new QSortFilterProxyModel(this);
    m_employeeProxyModel->setSourceModel(RscMgr::modelHandler()->model(NS_TABLE::EMPLOYEE));
    m_employeeProxyModel->setFilterKeyColumn(NS_EMPLOYEE::JOB);

    ui->employeeView->setModel(m_employeeProxyModel);
    ui->employeeView->hideColumn(NS_EMPLOYEE::ID);
    ui->employeeView->hideColumn(NS_EMPLOYEE::JOB);
    ui->employeeView->setItemDelegateForColumn(NS_EMPLOYEE::FULL_NAME, new UniqueDelegate(ui->employeeView));
    ui->employeeView->setItemDelegateForColumn(NS_EMPLOYEE::FATHER_NAME, new UniqueDelegate(ui->employeeView));
    ui->employeeView->setItemDelegateForColumn(NS_EMPLOYEE::GENDER, new MapDelegate(ui->employeeView));
    ui->employeeView->setItemDelegateForColumn(NS_EMPLOYEE::BIRTH_DATE, new DateDelegate(ui->employeeView));
    ui->employeeView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->employeeView->horizontalHeader()->setSectionResizeMode(NS_EMPLOYEE::FULL_NAME, QHeaderView::Stretch);

    ui->jobView->setModel(RscMgr::modelHandler()->model(NS_TABLE::JOB));
    ui->jobView->setModelColumn(NS_JOB::NAME);
    ui->jobView->setCurrentIndex(ui->jobView->model()->index(0, NS_JOB::NAME));
    ui->jobView->setFixedWidth(ui->jobView->minimumSizeHint().width()
                               + ui->jobView->verticalScrollBar()->sizeHint().width()
                               + 7);

    connect(ui->jobView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(adjustToJob()));
    connect(ui->jobView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));

    adjustToJob();
}

EmployeeWidget::~EmployeeWidget()
{
    delete ui;
}

void EmployeeWidget::adjustToJob()
{
    m_employeeProxyModel->setFilterRegExp(QString("^%1$").arg(ui->jobView->currentIndex().data().toString()));
}

void EmployeeWidget::refresh()
{
    RscMgr::modelHandler()->model(NS_TABLE::EMPLOYEE)->select();
    adjustToJob();
}

void EmployeeWidget::add()
{
    if (!ui->jobView->currentIndex().isValid()) {
        RscMgr::issueMessage(tr("Please select a job first."), false);
        return;
    }

    QMap<int, QVariant> map;
    map.insert(NS_EMPLOYEE::JOB,
               ui->jobView->currentIndex()
               .sibling(ui->jobView->currentIndex().row(), NS_JOB::ID).data());

    RscMgr::modelHandler()->insertRow(NS_TABLE::EMPLOYEE, map);

    const QModelIndex &idx = ui->employeeView->model()->index(0, NS_EMPLOYEE::FULL_NAME);

    ui->employeeView->setCurrentIndex(idx);
    ui->employeeView->edit(idx);
}

void EmployeeWidget::modify()
{
    if (!ui->employeeView->currentIndex().isValid())
        return;

    ui->employeeView->edit(ui->employeeView->currentIndex());
}

void EmployeeWidget::remove()
{
    if (!ui->employeeView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::EMPLOYEE, ui->employeeView->currentIndex().row());

    ui->employeeView->setFocus();
    ui->employeeView->setCurrentIndex(ui->employeeView->model()->index(0, NS_EMPLOYEE::FULL_NAME));
}

void EmployeeWidget::print()
{
    PrintOption opt;
    opt.setColumnRemoved(NS_EMPLOYEE::ID, true);
    opt.setColumnHidden(NS_EMPLOYEE::ID, true);
    opt.setColumnHidden(NS_EMPLOYEE::JOB, true);
    opt.setMajorColumn(NS_EMPLOYEE::FULL_NAME);
    opt.setColumnAggregative(NS_EMPLOYEE::GENDER, true);
    opt.setColumnAggregative(NS_EMPLOYEE::JOB, true);
    opt.setColumnAggregative(NS_EMPLOYEE::NATIONALITY, true);
    opt.setColumnAggregative(NS_EMPLOYEE::RESIDENCY, true);
    opt.setCurrentAggregativeColumn(NS_EMPLOYEE::JOB);

    PrintDialog dlg(m_employeeProxyModel->sourceModel(), &opt);

    dlg.exec();
}
