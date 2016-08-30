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

#include "relationaldelegate.h"

#include "resourcemanager.h"

#include <QComboBox>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSortFilterProxyModel>

RelationalDelegate::RelationalDelegate(QSqlTableModel *model, int column, bool isEditable, QObject *parent) :
    QStyledItemDelegate(parent),
    m_targetModel(model),
    m_targetColumn(column),
    m_editable(isEditable),
    m_map()
{
    updateMap(m_targetModel->index(0, 0), m_targetModel->index(m_targetModel->rowCount() - 1, 0));

    connect(m_targetModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(updateMap(QModelIndex,QModelIndex)));
}

QWidget *RelationalDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QComboBox *combo = NULL;

    if (m_editable) {
        combo = new QComboBox(parent);
        combo->setModel(m_targetModel);
        combo->setModelColumn(m_targetColumn);
    }

    return combo;
}

void RelationalDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
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

    QSqlTableModel *sqlModel = qobject_cast<QSqlTableModel *>(mdl);

    if (combo && sqlModel) {
        sqlModel->setData(idx, m_map.key(combo->currentText()));

        if (!sqlModel->submit()) {
            RscMgr::issueMessage(QString(tr("Database Error   ")).append(sqlModel->lastError().text()), false);
            sqlModel->selectRow(idx.row());
        }
    }
}

QString RelationalDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    Q_UNUSED(locale)

    return m_map.value(value.toLongLong());
}

void RelationalDelegate::updateMap(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    for (int i = topLeft.row(); i <= bottomRight.row(); ++i) {
        m_map.insert(m_targetModel->index(i, 0).data().toLongLong(),
                     m_targetModel->index(i, m_targetColumn).data().toString());
    }
}
