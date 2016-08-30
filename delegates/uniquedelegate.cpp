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

#include "uniquedelegate.h"

#include "resourcemanager.h"

#include <QLineEdit>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QSqlError>

UniqueDelegate::UniqueDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{}

void UniqueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);

    QAbstractItemModel *mdl = model;
    QModelIndex idx = index;

    if (model->inherits("QAbstractProxyModel")) {
        QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(model);
        if (proxyModel) {
            mdl = proxyModel->sourceModel();
            idx = proxyModel->mapToSource(index);
        }
    }

    QSqlTableModel *sqlModel = qobject_cast<QSqlTableModel *>(mdl);

    if (lineEdit && sqlModel) {
        sqlModel->setData(idx, lineEdit->text().trimmed());

        if (!sqlModel->submit()) {
            RscMgr::issueMessage(QString(tr("Database Error   ")).append(sqlModel->lastError().text()), false);
            sqlModel->selectRow(idx.row());
        }
    }
}
