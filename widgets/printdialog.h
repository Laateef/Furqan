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

#ifndef PRINTDIALOG_H
#define PRINTDIALOG_H

#include <QDialog>

namespace Ui {
class PrintDialog;
}

class TextEditor;
class PrintOption;

class QAbstractItemModel;
class QPrintPreviewWidget;
class QPrinter;
class QListWidget;
class QTabWidget;
class QButtonGroup;
class QListWidgetItem;
class QAbstractButton;
class QFontComboBox;
class QComboBox;

class PrintDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrintDialog(QAbstractItemModel *model, PrintOption *printOption, QWidget *parent = 0);
    ~PrintDialog();

private slots:
    void print() const;

    void preview(QPrinter *) const;

    void initialize();
    void createLayouts();
    void createConnections();
    void loadSettings();

    void setTableFontColor() const;

    void columnVisibilityChanged(QListWidgetItem *);

    void rowVisibilityChanged(QListWidgetItem *);

    void currentAggregationChanged(QAbstractButton *);

    void aggregationEntryVisibilityChanged(QListWidgetItem *);

private:
    QAbstractItemModel *m_model;

    PrintOption *m_printOption;

    TextEditor *m_preambleTextEdit;
    TextEditor *m_commentaryTextEdit;

    QFontComboBox *m_tableFontNameCombo;
    QComboBox *m_tableFontSizeCombo;
    QPushButton *m_tableColorButton;

    QListWidget *m_columnVisibilityListWidget;
    QListWidget *m_aggregativeColumnListWidget;
    QListWidget *m_aggregativeColumnEntryListWidget;
    QListWidget *m_rowVisibilityListWidget;

    QTabWidget *m_tabWidget;

    QPrintPreviewWidget *m_preview;

    QButtonGroup *m_buttonGroup;

    QPushButton *m_zoomIn;
    QPushButton *m_zoomOut;
    QPushButton *m_printButton;
    QPushButton *m_closeButton;
};

#endif // PRINTDIALOG_H
