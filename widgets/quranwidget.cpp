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

#include "quranwidget.h"
#include "ui_quranwidget.h"

#include "resourcemanager.h"
#include "mapdelegate.h"
#include "numericdelegate.h"
#include "datedelegate.h"
#include "printdialog.h"
#include "printoption.h"

#include <QSortFilterProxyModel>
#include <QScrollBar>

QuranWidget::QuranWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuranWidget)
{
    ui->setupUi(this);

    m_quranProxyModel = new QSortFilterProxyModel(this);
    m_quranProxyModel->setSourceModel(RscMgr::modelHandler()->model(NS_TABLE::QURAN));
    m_quranProxyModel->setFilterKeyColumn(NS_QURAN::STUDENT);

    ui->quranView->setModel(m_quranProxyModel);
    ui->quranView->hideColumn(NS_QURAN::ID);
    ui->quranView->hideColumn(NS_QURAN::STUDENT);
    ui->quranView->setItemDelegateForColumn(NS_QURAN::RECITAL_TYPE, new MapDelegate(ui->quranView));
    ui->quranView->setItemDelegateForColumn(NS_QURAN::FROM_PAGE, new NumericDelegate(new QIntValidator(0, 604, this), this));
    ui->quranView->setItemDelegateForColumn(NS_QURAN::TO_PAGE, new NumericDelegate(new QIntValidator(0, 604, this), this));
    ui->quranView->setItemDelegateForColumn(NS_QURAN::DATE, new DateDelegate(ui->quranView));
    ui->quranView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->quranView->horizontalHeader()->setSectionResizeMode(NS_QURAN::RECITAL_TYPE, QHeaderView::Stretch);

    ui->studentView->setModel(RscMgr::modelHandler()->model(NS_TABLE::STUDENT));
    ui->studentView->setModelColumn(NS_STUDENT::FULL_NAME);
    ui->studentView->setCurrentIndex(ui->studentView->model()->index(0, NS_STUDENT::FULL_NAME));
    ui->studentView->setFixedWidth(ui->studentView->minimumSizeHint().width()
                                   + ui->studentView->verticalScrollBar()->sizeHint().width()
                                   + 7);

    connect(ui->studentView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(adjustToStudent()));
    //maintaining the foreign key's relations between tables
    connect(ui->studentView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));
    connect(RscMgr::modelHandler()->model(NS_TABLE::CLASS), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(refresh()));

    adjustToStudent();
}

QuranWidget::~QuranWidget()
{
    delete ui;
}

void QuranWidget::adjustToStudent()
{
    m_quranProxyModel->setFilterRegExp(QString("^%1$").arg(ui->studentView->currentIndex().data().toString()));
}

void QuranWidget::refresh()
{
    RscMgr::modelHandler()->model(NS_TABLE::QURAN)->select();
    adjustToStudent();
}

void QuranWidget::add()
{
    if (!ui->studentView->currentIndex().isValid()) {
        RscMgr::issueMessage(tr("Please select a student first."), false);
        return;
    }

    QMap<int, QVariant> map;
    map.insert(NS_QURAN::STUDENT,
               ui->studentView->currentIndex()
               .sibling(ui->studentView->currentIndex().row(), NS_STUDENT::ID).data());

    RscMgr::modelHandler()->insertRow(NS_TABLE::QURAN, map);

    const QModelIndex &idx = ui->quranView->model()->index(0, NS_QURAN::RECITAL_TYPE);

    ui->quranView->setCurrentIndex(idx);
    ui->quranView->edit(idx);
}

void QuranWidget::modify()
{
    if (!ui->quranView->currentIndex().isValid())
        return;

    ui->quranView->edit(ui->quranView->currentIndex());
}

void QuranWidget::remove()
{
    if (!ui->quranView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::QURAN, ui->quranView->currentIndex().row());

    ui->quranView->setFocus();
    ui->quranView->setCurrentIndex(ui->quranView->model()->index(0, NS_QURAN::STUDENT));
}

void QuranWidget::print()
{
    PrintOption opt;
    opt.setColumnRemoved(NS_QURAN::ID, true);
    opt.setColumnHidden(NS_QURAN::ID, true);
    opt.setMajorColumn(NS_QURAN::STUDENT);
    opt.setColumnAggregative(NS_QURAN::DATE, true);
    opt.setColumnAggregative(NS_QURAN::RECITAL_TYPE, true);
    opt.setCurrentAggregativeColumn(-1);

    PrintDialog dlg(m_quranProxyModel->sourceModel(), &opt);

    dlg.exec();
}
