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

#include "sqldelegate.h"

#include "sqlrelation.h"
#include "richsqlmodel.h"

#include <QComboBox>
#include <QSortFilterProxyModel>

SqlDelegate::SqlDelegate(bool isEditable, QObject *parent) :
    QStyledItemDelegate(parent),
    m_isEditable(isEditable)
{}

QWidget *SqlDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    const QAbstractItemModel *model = index.model();

    const QAbstractItemModel *mdl = model;
    QModelIndex idx = index;
    AbstractRelation *relation = NULL;

    if (model->inherits("QAbstractProxyModel")) {
        const QSortFilterProxyModel *proxyModel = qobject_cast<const QSortFilterProxyModel *>(model);
        if (proxyModel) {
            mdl = proxyModel->sourceModel();
            idx = proxyModel->mapToSource(index);
            relation = qobject_cast<const RichSqlModel *>(mdl)->relation(idx.column());
        }
    } else {
        relation = qobject_cast<const RichSqlModel *>(model)->relation(index.column());
    }

    if (!relation || relation->type() != AbstractRelation::Sql)
        return NULL;

    SqlRelation *sqlRelation = qobject_cast<SqlRelation *>(relation);
    QSqlTableModel *sqlRemoteModel = const_cast<QSqlTableModel *>(sqlRelation->remoteModel());

    if (m_isEditable) {
        QComboBox *combo = new QComboBox(parent);
        combo->setModel(sqlRemoteModel);
        combo->setModelColumn(sqlRelation->displayColumn());
        combo->setCurrentText(sqlRelation->forwardLookup(idx.data(Qt::EditRole).toLongLong()));
        return combo;
    }

    return NULL;
}

void SqlDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combo = qobject_cast<QComboBox *>(editor);

    QAbstractItemModel *mdl = model;
    QModelIndex idx = index;

    if (model->inherits("QAbstractProxyModel")) {
        QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(model);
        if (proxyModel) {
            mdl = proxyModel->sourceModel();
            idx = proxyModel->mapToSource(index);
        }
    }

    RichSqlModel *sqlModel = qobject_cast<RichSqlModel *>(mdl);

    AbstractRelation *relation = sqlModel->relation(idx.column());

    if (!relation || relation->type() != AbstractRelation::Sql)
        return;

    SqlRelation *sqlRelation = qobject_cast<SqlRelation *>(relation);

    sqlModel->setData(idx, sqlRelation->backwardLookup(combo->currentText()));
    sqlModel->submit();
}
