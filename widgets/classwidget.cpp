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

#include "classwidget.h"
#include "ui_classwidget.h"

#include "resourcemanager.h"
#include "uniquedelegate.h"
#include "mapdelegate.h"
#include "printdialog.h"
#include "printoption.h"

#include <QSqlTableModel>

ClassWidget::ClassWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassWidget)
{
    ui->setupUi(this);

    ui->classView->setModel(RscMgr::modelHandler()->model(NS_TABLE::CLASS));
    ui->classView->hideColumn(NS_CLASS::ID);
    ui->classView->setItemDelegateForColumn(NS_CLASS::NAME, new UniqueDelegate(ui->classView));
    ui->classView->setItemDelegateForColumn(NS_CLASS::LEVEL, new MapDelegate(ui->classView));
    ui->classView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ClassWidget::~ClassWidget()
{
    delete ui;
}

void ClassWidget::add()
{
    QMap<int, QVariant> map;
    map.insert(NS_CLASS::LEVEL, NS_LEVEL::NONE);

    RscMgr::modelHandler()->insertRow(NS_TABLE::CLASS, map);

    const QModelIndex &idx = ui->classView->model()->index(0, NS_CLASS::NAME);

    ui->classView->setCurrentIndex(idx);
    ui->classView->edit(idx);
}

void ClassWidget::modify()
{
    if (!ui->classView->currentIndex().isValid())
        return;

    ui->classView->edit(ui->classView->currentIndex());
}

void ClassWidget::remove()
{
    if (!ui->classView->currentIndex().isValid())
        return;

    RscMgr::modelHandler()->removeRow(NS_TABLE::CLASS, ui->classView->currentIndex().row());

    ui->classView->setFocus();
    ui->classView->setCurrentIndex(ui->classView->model()->index(0, NS_CLASS::NAME));
}

void ClassWidget::print()
{
    PrintOption opt;

    opt.setColumnRemoved(NS_CLASS::ID, true);
    opt.setColumnHidden(NS_CLASS::ID, true);
    opt.setMajorColumn(NS_CLASS::NAME);
    opt.setColumnAggregative(NS_CLASS::LEVEL, true);
    opt.setCurrentAggregativeColumn(-1);

    PrintDialog dlg(ui->classView->model(), &opt);

    dlg.exec();
}
