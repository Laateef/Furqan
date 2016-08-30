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

#include "mapdelegate.h"

#include "maprelation.h"
#include "richsqlmodel.h"

#include <QComboBox>
#include <QSortFilterProxyModel>

MapDelegate::MapDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{}

QWidget *MapDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    AbstractRelation *relation = NULL;

    if (index.model()->inherits("QAbstractProxyModel")) {
        const QSortFilterProxyModel *proxyModel = qobject_cast<const QSortFilterProxyModel *>(index.model());
        if (proxyModel) {
            relation = qobject_cast<const RichSqlModel *>(proxyModel->sourceModel())->relation(proxyModel->mapToSource(index).column());
        }
    } else {
        relation = qobject_cast<const RichSqlModel *>(index.model())->relation(index.column());
    }

    if (!relation || relation->type() != AbstractRelation::Map)
        return NULL;

    MapRelation *mapRelation = qobject_cast<MapRelation *>(relation);

    QComboBox *combo = new QComboBox(parent);

    foreach (const int key, mapRelation->map()->keys())
        combo->insertItem(key, mapRelation->map()->value(key));

    combo->setCurrentText(index.data().toString());

    return combo;
}

void MapDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
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

    if (!relation || relation->type() != AbstractRelation::Map)
        return;

    MapRelation *mapRelation = qobject_cast<MapRelation *>(relation);

    sqlModel->setData(idx, mapRelation->backwardLookup(combo->currentText()));
    sqlModel->submit();
}
