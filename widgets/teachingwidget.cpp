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

#include "teachingwidget.h"
#include "ui_teachingwidget.h"

#include "resourcemanager.h"
#include "sqldelegate.h"
#include "printdialog.h"
#include "printoption.h"

#include <QSortFilterProxyModel>
#include <QScrollBar>

TeachingWidget::TeachingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeachingWidget)
{
    ui->setupUi(this);

    m_teachingProxyModel = new QSortFilterProxyModel(this);
    m_teachingProxyModel->setSourceModel(RscMgr::modelHandler()->model(NS_TABLE::TEACHING));
    m_teachingProxyModel->setFilterKeyColumn(NS_TEACHING::TEACHER);

    ui->teachingView->setModel(m_teachingProxyModel);
    ui->teachingView->hideColumn(NS_TEACHING::ID);
    ui->teachingView->hideColumn(NS_TEACHING::TEACHER);
    ui->teachingView->setItemDelegateForColumn(NS_TEACHING::TEACHER, new SqlDelegate(true, ui->teachingView));
    ui->teachingView->setItemDelegateForColumn(NS_TEACHING::CLASS, new SqlDelegate(true, ui->teachingView));
    ui->teachingView->setItemDelegateForColumn(NS_TEACHING::COURSE, new SqlDelegate(true, ui->teachingView));
    ui->teachingView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->teacherView->setModel(RscMgr::modelHandler()->model(NS_TABLE::TEACHER));
    ui->teacherView->setModelColumn(NS_TEACHER::FULL_NAME);
    ui->teacherView->setCurrentIndex(ui->teacherView->model()->index(0, NS_TEACHER::FULL_NAME));
    ui->teacherView->setFixedWidth(ui->teacherView->minimumSizeHint().width()
                                   + ui->teacherView->verticalScrollBar()->sizeHint().width()
                                   + 7);

    connect(ui->teacherView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(adjustToTeacher()));
    //maintaining the foreign key's relations between tables
    connect(ui->teacherView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));
    connect(RscMgr::modelHandler()->model(NS_TABLE::CLASS), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));
    connect(RscMgr::modelHandler()->model(NS_TABLE::COURSE), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));

    adjustToTeacher();
}

TeachingWidget::~TeachingWidget()
{
    delete ui;
}

void TeachingWidget::adjustToTeacher()
{
    m_teachingProxyModel->setFilterRegExp(QString("^%1$").arg(ui->teacherView->currentIndex().data().toString()));
}

void TeachingWidget::refresh()
{
    RscMgr::modelHandler()->model(NS_TABLE::TEACHING)->select();
    adjustToTeacher();
}

void TeachingWidget::add()
{
    if (!ui->teacherView->currentIndex().isValid()) {
        RscMgr::issueMessage(tr("Please select a teacher first."), false);
        return;
    }

    QMap<int, QVariant> map;
    map.insert(NS_TEACHING::TEACHER,
               ui->teacherView->currentIndex()
               .sibling(ui->teacherView->currentIndex().row(), NS_TEACHER::ID).data());

    RscMgr::modelHandler()->insertRow(NS_TABLE::TEACHING, map);

    const QModelIndex &idx = ui->teachingView->model()->index(0, NS_TEACHING::CLASS);

    ui->teachingView->setCurrentIndex(idx);
    ui->teachingView->edit(idx);
}

void TeachingWidget::modify()
{
    if (!ui->teachingView->currentIndex().isValid())
        return;

    ui->teachingView->edit(ui->teachingView->currentIndex());
}

void TeachingWidget::remove()
{
    if (!ui->teachingView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::TEACHING, ui->teachingView->currentIndex().row());

    ui->teachingView->setFocus();
    ui->teachingView->setCurrentIndex(ui->teachingView->model()->index(0, NS_TEACHING::CLASS));
}

void TeachingWidget::print()
{
    PrintOption opt;

    opt.setColumnRemoved(NS_TEACHING::ID, true);
    opt.setColumnHidden(NS_TEACHING::ID, true);
    opt.setMajorColumn(-1);
    opt.setColumnAggregative(NS_TEACHING::TEACHER, true);
    opt.setColumnAggregative(NS_TEACHING::CLASS, true);
    opt.setColumnAggregative(NS_TEACHING::COURSE, true);

    opt.setCurrentAggregativeColumn(NS_TEACHING::TEACHER);

    PrintDialog dlg(m_teachingProxyModel->sourceModel(), &opt);

    dlg.exec();
}
