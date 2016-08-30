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

#include "studentwidget.h"
#include "ui_studentwidget.h"

#include "resourcemanager.h"
#include "uniquedelegate.h"
#include "datedelegate.h"
#include "sqldelegate.h"
#include "mapdelegate.h"
#include "printdialog.h"
#include "printoption.h"

#include <QSortFilterProxyModel>
#include <QScrollBar>

StudentWidget::StudentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentWidget)
{
    ui->setupUi(this);

    m_studentProxyModel = new QSortFilterProxyModel(this);
    m_studentProxyModel->setSourceModel(RscMgr::modelHandler()->model(NS_TABLE::STUDENT));
    m_studentProxyModel->setFilterKeyColumn(NS_STUDENT::CLASS);

    ui->studentView->setModel(m_studentProxyModel);
    ui->studentView->hideColumn(NS_STUDENT::ID);
    ui->studentView->hideColumn(NS_STUDENT::CLASS);
    ui->studentView->setItemDelegateForColumn(NS_STUDENT::FULL_NAME, new UniqueDelegate(ui->studentView));
    ui->studentView->setItemDelegateForColumn(NS_STUDENT::FATHER_NAME, new UniqueDelegate(ui->studentView));
    ui->studentView->setItemDelegateForColumn(NS_STUDENT::GENDER, new MapDelegate(ui->studentView));
    ui->studentView->setItemDelegateForColumn(NS_STUDENT::LEVEL, new MapDelegate(ui->studentView));
    ui->studentView->setItemDelegateForColumn(NS_STUDENT::BIRTH_DATE, new DateDelegate(ui->studentView));
    ui->studentView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->studentView->horizontalHeader()->setSectionResizeMode(NS_STUDENT::FULL_NAME, QHeaderView::Stretch);

    ui->classView->setModel(RscMgr::modelHandler()->model(NS_TABLE::CLASS));
    ui->classView->setModelColumn(NS_CLASS::NAME);
    ui->classView->setCurrentIndex(ui->classView->model()->index(0, NS_CLASS::NAME));
    ui->classView->setFixedWidth(ui->classView->minimumSizeHint().width()
                                   + ui->classView->verticalScrollBar()->sizeHint().width()
                                   + 7);

    connect(ui->classView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(adjustToClass()));
    //maintaining the foreign key's relations between tables
    connect(ui->classView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));

    adjustToClass();
}

StudentWidget::~StudentWidget()
{
    delete ui;
}

void StudentWidget::adjustToClass()
{
    m_studentProxyModel->setFilterRegExp(QString("^%1$").arg(ui->classView->currentIndex().data().toString()));
}

void StudentWidget::refresh()
{
    RscMgr::modelHandler()->model(NS_TABLE::STUDENT)->select();
    adjustToClass();
}

void StudentWidget::add()
{
    if (!ui->classView->currentIndex().isValid()) {
        RscMgr::issueMessage(tr("Please select a class first."), false);
        return;
    }

    QMap<int, QVariant> map;
    map.insert(NS_STUDENT::CLASS,
               ui->classView->currentIndex()
               .sibling(ui->classView->currentIndex().row(), NS_CLASS::ID).data());

    RscMgr::modelHandler()->insertRow(NS_TABLE::STUDENT, map);

    const QModelIndex &idx = ui->studentView->model()->index(0, NS_STUDENT::FULL_NAME);

    ui->studentView->setCurrentIndex(idx);
    ui->studentView->edit(idx);
}

void StudentWidget::modify()
{
    if (!ui->studentView->currentIndex().isValid())
        return;

    ui->studentView->edit(ui->studentView->currentIndex());
}

void StudentWidget::remove()
{
    if (!ui->studentView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::STUDENT, ui->studentView->currentIndex().row());

    ui->studentView->setFocus();
    ui->studentView->setCurrentIndex(ui->studentView->model()->index(0, NS_STUDENT::FULL_NAME));
}

void StudentWidget::print()
{
    PrintOption opt;
    opt.setColumnRemoved(NS_STUDENT::ID, true);
    opt.setColumnHidden(NS_STUDENT::ID, true);
    opt.setColumnHidden(NS_STUDENT::CLASS, true);
    opt.setMajorColumn(NS_STUDENT::FULL_NAME);
    opt.setColumnAggregative(NS_STUDENT::LEVEL, true);
    opt.setColumnAggregative(NS_STUDENT::CLASS, true);
    opt.setCurrentAggregativeColumn(NS_STUDENT::CLASS);

    PrintDialog dlg(m_studentProxyModel->sourceModel(), &opt);

    dlg.exec();
}
