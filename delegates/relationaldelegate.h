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

#ifndef RELATIONALDELEGATE_H
#define RELATIONALDELEGATE_H

#include <QStyledItemDelegate>

class QSqlTableModel;

class RelationalDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit RelationalDelegate(QSqlTableModel *model, int column, bool isEditable = true, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    QString displayText(const QVariant &value, const QLocale &locale) const;

private slots:
    void updateMap(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    QSqlTableModel *m_targetModel;

    int m_targetColumn;

    const bool m_editable;

    QMap<qlonglong, QString> m_map;
};

#endif // RELATIONALDELEGATE_H
