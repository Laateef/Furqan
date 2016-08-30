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

#include "printdialog.h"

#include "resourcemanager.h"
#include "texteditor.h"
#include "printOption.h"

#include <QSqlTableModel>
#include <QPrintPreviewWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QListWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QFontComboBox>
#include <QColorDialog>

PrintDialog::PrintDialog(QAbstractItemModel *model, PrintOption *printOption, QWidget *parent) :
    QDialog(parent),
    m_model(model),
    m_printOption(printOption),
    m_preambleTextEdit(new TextEditor),
    m_commentaryTextEdit(new TextEditor),
    m_tableFontNameCombo(new QFontComboBox),
    m_tableFontSizeCombo(new QComboBox),
    m_tableColorButton(new QPushButton),
    m_columnVisibilityListWidget(new QListWidget),
    m_aggregativeColumnListWidget(new QListWidget),
    m_aggregativeColumnEntryListWidget(new QListWidget),
    m_rowVisibilityListWidget(new QListWidget),
    m_tabWidget(new QTabWidget),
    m_preview(new QPrintPreviewWidget(RscMgr::printHandler()->previewPrinter())),
    m_buttonGroup(new QButtonGroup(this)),
    m_zoomIn(new QPushButton),
    m_zoomOut(new QPushButton),
    m_printButton(new QPushButton(tr("Print"))),
    m_closeButton(new QPushButton(tr("Close")))
{
    setWindowTitle(tr("Print"));
    setWindowFlags(Qt::FramelessWindowHint);

    setLayoutDirection(RscMgr::dir());

    initialize();
    createLayouts();
    loadSettings();
    createConnections();

    m_tabWidget->setFixedWidth(m_columnVisibilityListWidget->sizeHint().width());
    m_preview->setFixedWidth(m_preview->width());

    showFullScreen();
}

PrintDialog::~PrintDialog()
{}

void PrintDialog::print() const
{
    m_printOption->setPreamble(m_preambleTextEdit->toHtml());
    m_printOption->setCommentary(m_commentaryTextEdit->toHtml());

    RscMgr::printHandler()->print(m_model, m_printOption, RscMgr::printHandler()->nativePrinter());
}

void PrintDialog::preview(QPrinter *printer) const
{
    m_printOption->setPreamble(m_preambleTextEdit->toHtml());
    m_printOption->setCommentary(m_commentaryTextEdit->toHtml());

    RscMgr::printHandler()->print(m_model, m_printOption, printer);
}

void PrintDialog::initialize()
{
    m_tableFontSizeCombo->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        m_tableFontSizeCombo->addItem(QString::number(size));

    m_tableColorButton->setFlat(true);
    m_tableColorButton->setMaximumWidth(25);
    m_tableColorButton->setCursor(Qt::PointingHandCursor);
    m_tableColorButton->setStyleSheet(QString("border: 1px solid black; background-color: %1;").arg(RscMgr::settingHandler()->tableFontColor()));

    m_tabWidget->setTabPosition(RscMgr::dir() == Qt::RightToLeft ? QTabWidget::East : QTabWidget::West);
    m_tabWidget->addTab(m_columnVisibilityListWidget, tr("Shown Columns"));
    m_tabWidget->addTab(m_aggregativeColumnListWidget, tr("Aggregative Columns"));
    m_tabWidget->addTab(m_aggregativeColumnEntryListWidget, tr("Aggragative Column Entries"));
    m_tabWidget->addTab(m_rowVisibilityListWidget, tr("Shown Rows"));

    for (int i = 0; i < m_model->columnCount(); ++i) {
        if (m_printOption->isColumnRemoved(i))
            continue;

        QListWidgetItem *item = new QListWidgetItem(m_model->headerData(i, Qt::Horizontal).toString());
        item->setCheckState(m_printOption->isColumnHidden(i) ? Qt::Unchecked : Qt::Checked);
        item->setData(Qt::UserRole, i);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

        //if it's the name column, then disable it.
        if (i == m_printOption->majorColumn())
            item->setFlags(Qt::NoItemFlags);

        m_columnVisibilityListWidget->addItem(item);
    }

    QRadioButton *noneRadioButton = new QRadioButton(tr("None"));
    noneRadioButton->setChecked(true);
    m_buttonGroup->addButton(noneRadioButton);
    QListWidgetItem *item = new QListWidgetItem;
    item->setFlags(Qt::ItemIsEnabled);
    m_aggregativeColumnListWidget->addItem(item);
    m_aggregativeColumnListWidget->setItemWidget(item, noneRadioButton);

    for (int i = 0; i < m_model->columnCount(); ++i) {
        if (m_printOption->isColumnAggregative(i)) {
            QRadioButton *radioButton = new QRadioButton(m_model->headerData(i, Qt::Horizontal).toString());
            m_buttonGroup->addButton(radioButton);
            radioButton->setChecked(i == m_printOption->currentAggregativeColumn());
            QListWidgetItem *item = new QListWidgetItem;
            item->setFlags(Qt::ItemIsEnabled);
            item->setData(Qt::UserRole, i);
            m_aggregativeColumnListWidget->addItem(item);
            m_aggregativeColumnListWidget->setItemWidget(item, radioButton);
        }
    }

    emit currentAggregationChanged(m_buttonGroup->checkedButton());
}

void PrintDialog::createLayouts()
{
    QString styleSheet = "QFrame {"
            "background: lightgray;"
            "border: 1px solid black;"
            "padding: 1px;"
            "}"
            "QPushButton {"
            "   padding: 3px;"
            "}"
            "QPushButton:hover {"
            "   color: white;"
            "    border: 1px solid gray;"
            "}";

    QFrame *zoomFrame = new QFrame;
    zoomFrame->setStyleSheet(styleSheet);

    m_zoomIn->setIcon(QIcon(":/zoom-in"));
    m_zoomIn->setFlat(true);
    m_zoomIn->setIconSize(QSize(24, 24));
    m_zoomIn->setCursor(QCursor(Qt::PointingHandCursor));

    m_zoomOut->setIcon(QIcon(":/zoom-out"));
    m_zoomOut->setFlat(true);
    m_zoomOut->setIconSize(QSize(24, 24));
    m_zoomOut->setCursor(QCursor(Qt::PointingHandCursor));

    m_preview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_preview->fitToWidth();

    QFrame *printFrame = new QFrame;
    printFrame->setStyleSheet(styleSheet);

    m_printButton->setFlat(true);
    m_printButton->setIcon(QIcon(":/printer"));
    m_printButton->setIconSize(QSize(32, 32));
    m_printButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_closeButton->setFlat(true);
    m_closeButton->setIcon(RscMgr::dir() == Qt::LeftToRight ? QIcon(":/close-ltr") : QIcon(":/close-rtl"));
    m_closeButton->setIconSize(QSize(32, 32));
    m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));

    QHBoxLayout *tableHLayout = new QHBoxLayout;
    tableHLayout->addWidget(m_tableFontNameCombo);
    tableHLayout->addWidget(m_tableFontSizeCombo);
    tableHLayout->addWidget(m_tableColorButton);
    tableHLayout->addStretch();

    QVBoxLayout *editVLayout = new QVBoxLayout;
    editVLayout->addWidget(new QLabel(QString("%1<HR />").arg(tr("Preamble"))));
    editVLayout->addWidget(m_preambleTextEdit);
    editVLayout->addSpacing(7);
    editVLayout->addWidget(new QLabel(QString("%1<HR />").arg(tr("Table"))));
    editVLayout->addLayout(tableHLayout);
    editVLayout->addSpacing(7);
    editVLayout->addWidget(new QLabel(QString("%1<HR />").arg(tr("Commentary"))));
    editVLayout->addWidget(m_commentaryTextEdit);

    QHBoxLayout *zoomHLayout = new QHBoxLayout(zoomFrame);
    zoomHLayout->setContentsMargins(QMargins(0, 0, 0, 0));
    zoomHLayout->setSpacing(3);
    zoomHLayout->addStretch();
    zoomHLayout->addWidget(m_zoomIn);
    zoomHLayout->addWidget(m_zoomOut);

    QVBoxLayout *previewVLayout = new QVBoxLayout;
    previewVLayout->setSpacing(1);
    previewVLayout->addWidget(m_preview);
    previewVLayout->addWidget(zoomFrame);

    QHBoxLayout *mainHLayout = new QHBoxLayout;
    mainHLayout->addLayout(editVLayout);
    mainHLayout->addWidget(m_tabWidget);
    mainHLayout->addLayout(previewVLayout);

    QHBoxLayout *printHLayout = new QHBoxLayout(printFrame);
    printHLayout->setContentsMargins(QMargins(0, 0, 0, 0));
    printHLayout->setSpacing(15);
    printHLayout->addStretch();
    printHLayout->addWidget(m_printButton);
    printHLayout->addWidget(m_closeButton);

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(printFrame);
    mainVLayout->addLayout(mainHLayout);
}

void PrintDialog::createConnections()
{
    connect(m_preambleTextEdit, SIGNAL(fontFamilyChanged(QString)), RscMgr::settingHandler(), SLOT(setPreambleFontFamily(QString)));
    connect(m_preambleTextEdit, SIGNAL(fontSizeChanged(QString)), RscMgr::settingHandler(), SLOT(setPreambleFontSize(QString)));
    connect(m_preambleTextEdit, SIGNAL(fontColorChanged(QString)), RscMgr::settingHandler(), SLOT(setPreambleFontColor(QString)));

    connect(m_commentaryTextEdit, SIGNAL(fontFamilyChanged(QString)), RscMgr::settingHandler(), SLOT(setCommentaryFontFamily(QString)));
    connect(m_commentaryTextEdit, SIGNAL(fontSizeChanged(QString)), RscMgr::settingHandler(), SLOT(setCommentaryFontSize(QString)));
    connect(m_commentaryTextEdit, SIGNAL(fontColorChanged(QString)), RscMgr::settingHandler(), SLOT(setCommentaryFontColor(QString)));

    connect(m_tableFontNameCombo, SIGNAL(currentIndexChanged(QString)), RscMgr::settingHandler(), SLOT(setTableFontFamily(QString)));
    connect(m_tableFontSizeCombo, SIGNAL(currentTextChanged(QString)), RscMgr::settingHandler(), SLOT(setTableFontSize(QString)));
    connect(m_tableColorButton, SIGNAL(clicked()), SLOT(setTableFontColor()));

    connect(m_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(currentAggregationChanged(QAbstractButton*)));

    connect(m_columnVisibilityListWidget, SIGNAL(itemChanged(QListWidgetItem*)), SLOT(columnVisibilityChanged(QListWidgetItem*)));
    connect(m_aggregativeColumnEntryListWidget, SIGNAL(itemChanged(QListWidgetItem*)), SLOT(aggregationEntryVisibilityChanged(QListWidgetItem*)));
    connect(m_rowVisibilityListWidget, SIGNAL(itemChanged(QListWidgetItem*)), SLOT(rowVisibilityChanged(QListWidgetItem*)));

    connect(m_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), m_preview, SLOT(updatePreview()));
    connect(m_columnVisibilityListWidget, SIGNAL(itemChanged(QListWidgetItem*)), m_preview, SLOT(updatePreview()));
    connect(m_aggregativeColumnEntryListWidget, SIGNAL(itemChanged(QListWidgetItem*)), m_preview, SLOT(updatePreview()));
    connect(m_rowVisibilityListWidget, SIGNAL(itemChanged(QListWidgetItem*)), m_preview, SLOT(updatePreview()));
    connect(m_preambleTextEdit, SIGNAL(textChanged()), m_preview, SLOT(updatePreview()));
    connect(m_commentaryTextEdit, SIGNAL(textChanged()), m_preview, SLOT(updatePreview()));
    connect(m_tableFontNameCombo, SIGNAL(currentFontChanged(QFont)), m_preview, SLOT(updatePreview()));
    connect(m_tableFontSizeCombo, SIGNAL(currentIndexChanged(int)), m_preview, SLOT(updatePreview()));
    connect(RscMgr::settingHandler(), SIGNAL(settingChanged()), m_preview, SLOT(updatePreview()));

    connect(m_preview, SIGNAL(paintRequested(QPrinter*)), SLOT(preview(QPrinter*)));

    connect(m_zoomIn, SIGNAL(clicked()), m_preview, SLOT(zoomIn()));
    connect(m_zoomOut, SIGNAL(clicked()), m_preview, SLOT(zoomOut()));

    connect(m_printButton, SIGNAL(clicked()), SLOT(print()));
    connect(m_printButton, SIGNAL(clicked()), SLOT(accept()));
    connect(m_closeButton, SIGNAL(clicked()), SLOT(reject()));

}

void PrintDialog::loadSettings()
{
    m_preambleTextEdit->setFontFamily(RscMgr::settingHandler()->preambleFontFamily());
    m_preambleTextEdit->setFontColor(RscMgr::settingHandler()->preambleFontColor());
    m_preambleTextEdit->setFontSize(RscMgr::settingHandler()->preambleFontSize());

    m_commentaryTextEdit->setFontFamily(RscMgr::settingHandler()->commentaryFontFamily());
    m_commentaryTextEdit->setFontColor(RscMgr::settingHandler()->commentaryFontColor());
    m_commentaryTextEdit->setFontSize(RscMgr::settingHandler()->commentaryFontSize());

    m_tableFontNameCombo->setCurrentText(RscMgr::settingHandler()->tableFontFamily());

    m_tableFontSizeCombo->setCurrentText(RscMgr::settingHandler()->tableFontSize());

}

void PrintDialog::setTableFontColor() const
{
    QColor color = QColorDialog::getColor(Qt::black);
    if (!color.isValid())
        return;

    RscMgr::settingHandler()->setTableFontColor(color.name());
    m_tableColorButton->setStyleSheet(QString("border: 1px solid black; background-color: %1;").arg(color.name(QColor::HexRgb)));
}

void PrintDialog::columnVisibilityChanged(QListWidgetItem *item)
{
    //the addition by on is for the ID column of each table.
    m_printOption->setColumnHidden(item->data(Qt::UserRole).toInt(), item->checkState() == Qt::Unchecked);
}

void PrintDialog::rowVisibilityChanged(QListWidgetItem *item)
{
    m_printOption->setRowHidden(item->data(Qt::UserRole).toInt(), item->checkState() == Qt::Unchecked);
}

void PrintDialog::currentAggregationChanged(QAbstractButton *button)
{
    m_aggregativeColumnEntryListWidget->clear();

    for (int i = 0; i < m_model->columnCount(); ++i) {
        if (m_model->headerData(i, Qt::Horizontal).toString() == button->text()) {
            m_printOption->setCurrentAggregativeColumn(i);

            for (int j = 0; j < m_model->rowCount(); ++j) {
                const QString &itemText = m_model->index(j, i).data().toString();

                QList<QListWidgetItem *> itemList = m_aggregativeColumnEntryListWidget->findItems(itemText, Qt::MatchExactly);
                if (itemList.isEmpty()) {
                    QListWidgetItem *item = new QListWidgetItem(itemText);
                    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
                    item->setCheckState(Qt::Checked);
                    m_aggregativeColumnEntryListWidget->addItem(item);
                }
            }

            emit aggregationEntryVisibilityChanged(0);

            return;
        }
    }

    m_printOption->setCurrentAggregativeColumn(-1);

    emit aggregationEntryVisibilityChanged(0);
}

void PrintDialog::aggregationEntryVisibilityChanged(QListWidgetItem *item)
{
    if (item)
        m_printOption->setAggregationEntryHidden(item->text(), item->checkState() == Qt::Unchecked);

    m_rowVisibilityListWidget->clear();

    if (m_printOption->currentAggregativeColumn() >= 0)
        m_model->sort(m_printOption->currentAggregativeColumn(), Qt::AscendingOrder);
    else
        m_model->sort(0, Qt::AscendingOrder);

    for (int i = 0; i < m_model->rowCount(); ++i) {
        if (m_printOption->currentAggregativeColumn() >= 0) {
            const QString &entry = m_model->index(i, m_printOption->currentAggregativeColumn()).data().toString();
            if (m_printOption->isAggregationEntryHidden(entry))
                continue;
        }
        QListWidgetItem *rowItem = new QListWidgetItem(QString("[%1]  %2").arg(i + 1).arg(m_model->index(i, m_printOption->majorColumn()).data().toString()));
        rowItem->setCheckState(m_printOption->isRowHidden(m_model->index(i, 0).data().toInt()) ? Qt::Unchecked : Qt::Checked);
        rowItem->setData(Qt::UserRole, m_model->index(i, 0).data().toInt());
        rowItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        m_rowVisibilityListWidget->addItem(rowItem);
    }
}
