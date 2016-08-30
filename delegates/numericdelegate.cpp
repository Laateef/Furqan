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

#include "numericdelegate.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QApplication>

NumericDelegate::NumericDelegate(QValidator *validator, QObject *parent) :
    QStyledItemDelegate(parent),
    m_validator(validator)
{}

QWidget *NumericDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QLineEdit *editor = new QLineEdit(parent);
    editor->setValidator(m_validator);

    return editor;
}

bool NumericDelegate::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Escape) {
            QApplication::postEvent(object, new QKeyEvent(event->type(), Qt::Key_Enter, Qt::NoModifier));
            event->ignore();
            return true;
        }
    }
    return QStyledItemDelegate::eventFilter(object, event);
}
