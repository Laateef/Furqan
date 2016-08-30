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

#include "datedelegate.h"

#include <QDateEdit>
#include <QSortFilterProxyModel>

DateDelegate::DateDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{}

QWidget *DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    QDateEdit *dateEdit = new QDateEdit(parent);

    if (index.data().isNull())
        dateEdit->setDate(QDate::currentDate());
    else
        dateEdit->setDate(index.data().toDate());

    return dateEdit;
}

void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit *dateEdit = qobject_cast<QDateEdit *>(editor);

    if (!dateEdit)
        return;

    QAbstractItemModel *mdl = model;
    QModelIndex idx = index;

    if (model->inherits("QAbstractProxyModel")) {
        QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(model);
        if (proxyModel) {
            mdl = proxyModel->sourceModel();
            idx = proxyModel->mapToSource(index);
        }
    }

    mdl->setData(idx, dateEdit->date());
    mdl->submit();
}
