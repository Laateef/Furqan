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

#include "printerhandler.h"

#include "printoption.h"
#include "resourcemanager.h"

#include <QApplication>
#include <QSortFilterProxyModel>
#include <QTextDocument>
#include <QTextTable>
#include <QTextTableFormat>

PrinterHandler::PrinterHandler(QObject *parent) :
    QObject(parent)
{
    initialize();

    connect(RscMgr::settingHandler(), SIGNAL(settingChanged()), SLOT(initialize()));
}

PrinterHandler *PrinterHandler::singleton()
{
    static PrinterHandler *printerHandler = NULL;

    if (!printerHandler)
        printerHandler = new PrinterHandler(qApp);

    return printerHandler;
}

QPrinter *PrinterHandler::nativePrinter()
{
    return &m_nativePrinter;
}

QPrinter *PrinterHandler::previewPrinter()
{
    return &m_previewPrinter;
}

QPrinter *PrinterHandler::settingPreviewPrinter()
{
    return &m_settingPreviewPrinter;
}

void PrinterHandler::initialize()
{
    m_nativePrinter.setOutputFormat(QPrinter::NativeFormat);
    m_nativePrinter.setPaperSize(RscMgr::settingHandler()->printerPageSize());
    m_nativePrinter.setOrientation(RscMgr::settingHandler()->printerOrientation());
    m_nativePrinter.setColorMode(RscMgr::settingHandler()->printerColorMode());
    m_nativePrinter.setPageMargins(RscMgr::settingHandler()->printerLeftMargin(),
                                   RscMgr::settingHandler()->printerTopMargin(),
                                   RscMgr::settingHandler()->printerRightMargin(),
                                   RscMgr::settingHandler()->printerBottomMargin(),
                                   QPrinter::Millimeter);

    m_previewPrinter.setOutputFormat(QPrinter::PdfFormat);
    m_previewPrinter.setPaperSize(RscMgr::settingHandler()->printerPageSize());
    m_previewPrinter.setOrientation(RscMgr::settingHandler()->printerOrientation());
    m_previewPrinter.setColorMode(RscMgr::settingHandler()->printerColorMode());
    m_previewPrinter.setPageMargins(RscMgr::settingHandler()->printerLeftMargin(),
                                    RscMgr::settingHandler()->printerTopMargin(),
                                    RscMgr::settingHandler()->printerRightMargin(),
                                    RscMgr::settingHandler()->printerBottomMargin(),
                                    QPrinter::Millimeter);

    m_settingPreviewPrinter.setOutputFormat(QPrinter::PdfFormat);
    m_settingPreviewPrinter.setPaperSize(RscMgr::settingHandler()->printerPageSize());
    m_settingPreviewPrinter.setOrientation(RscMgr::settingHandler()->printerOrientation());
    m_settingPreviewPrinter.setColorMode(RscMgr::settingHandler()->printerColorMode());
    m_settingPreviewPrinter.setPageMargins(RscMgr::settingHandler()->printerLeftMargin(),
                                           RscMgr::settingHandler()->printerTopMargin(),
                                           RscMgr::settingHandler()->printerRightMargin(),
                                           RscMgr::settingHandler()->printerBottomMargin(),
                                           QPrinter::Millimeter);
}

void PrinterHandler::print(QAbstractItemModel *model,
                           PrintOption *printOption,
                           QPrinter *printer)
{
    QTextDocument doc;
    doc.setDocumentMargin(1);
    doc.setTextWidth(printer->pageRect().width());
    doc.setPageSize(printer->pageRect().size());
    doc.setDefaultTextOption(QTextOption(RscMgr::dir() == Qt::RightToLeft ? Qt::AlignRight : Qt::AlignLeft));

    QTextCursor curser(&doc);

    if (RscMgr::settingHandler()->headerType() == NS_HeaderType::Image) {

        QTextImageFormat textImageFormat;
        textImageFormat.setName(RscMgr::settingHandler()->headerImagePath());
        textImageFormat.setWidth(RscMgr::settingHandler()->headerWidth());
        textImageFormat.setHeight(RscMgr::settingHandler()->headerHeight());

        QTextBlockFormat textBlockFormat;
        textBlockFormat.setAlignment(RscMgr::settingHandler()->headerAlignment());

        curser.setBlockFormat(textBlockFormat);

        curser.insertImage(textImageFormat);

        curser.insertBlock();
        curser.setBlockFormat(QTextBlockFormat());
    }

    curser.insertHtml(printOption->preamble());

    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignCenter);
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(Qt::lightGray);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Double);
    tableFormat.setCellPadding(3);
    tableFormat.setCellSpacing(1);
    tableFormat.setMargin(3);
    tableFormat.setWidth(doc.textWidth());
    tableFormat.setHeaderRowCount(1);

    //the addition by one is for the vertical header.
    const int tableColumnCount = model->columnCount() - printOption->hiddenColumnCount() + 1;

    QVector<QTextLength> textLengthVector(tableColumnCount);
    textLengthVector.replace(RscMgr::dir() == Qt::RightToLeft ? tableColumnCount - 1 : 0, QTextLength(QTextLength::PercentageLength, 5));
    tableFormat.setColumnWidthConstraints(textLengthVector);

    QTextTable *textTable = curser.insertTable(1, tableColumnCount, tableFormat);

    QTextCharFormat cornerCellFormat;
    cornerCellFormat.setBackground(Qt::black);

    QTextCharFormat headerCellFormat;
    headerCellFormat.setBackground(Qt::darkGray);
    headerCellFormat.setForeground(Qt::white);
    headerCellFormat.setFontFamily(RscMgr::settingHandler()->tableFontFamily());
    headerCellFormat.setFontPointSize(RscMgr::settingHandler()->tableFontSize().toInt() + 4);

    QTextCharFormat sectionCellFormat;
    sectionCellFormat.setBackground(Qt::lightGray);
    sectionCellFormat.setForeground(Qt::white);
    sectionCellFormat.setFontFamily(RscMgr::settingHandler()->tableFontFamily());
    sectionCellFormat.setFontPointSize(RscMgr::settingHandler()->tableFontSize().toInt() + 2);

    QTextCharFormat tableCellFormat;
    tableCellFormat.setFontFamily(RscMgr::settingHandler()->tableFontFamily());
    tableCellFormat.setFontPointSize(RscMgr::settingHandler()->tableFontSize().toInt());
    tableCellFormat.setForeground(QColor(RscMgr::settingHandler()->tableFontColor()));

    //filling the corner in black
    if (RscMgr::dir() == Qt::RightToLeft)
        textTable->cellAt(0, textTable->columns() - 1).setFormat(cornerCellFormat);
    else
        textTable->cellAt(0, 0).setFormat(cornerCellFormat);

    //    textTable column index, beginning at 1 due to the existence of id column
    int y = 1;
    for (int j = 0; j < model->columnCount(); ++j) {
        if (printOption->isColumnHidden(j))
            continue;

        QTextTableCell cell = textTable->cellAt(0, RscMgr::dir() == Qt::RightToLeft ? textTable->columns() - y - 1 : y);
        cell.setFormat(headerCellFormat);
        cell.firstCursorPosition().insertText(model->headerData(j, Qt::Horizontal).toString());
        ++y;
    }

    if (printOption->currentAggregativeColumn() >= 0)
        model->sort(printOption->currentAggregativeColumn(), Qt::AscendingOrder);
    else
        model->sort(0, Qt::AscendingOrder);

    QVariant lastValue;
    for (int i = 0, x = 1; i < model->rowCount(); ++i, ++x) {
        if (printOption->isRowHidden(model->index(i, 0).data().toInt())) {
            --x;
            continue;
        }

        if (printOption->currentAggregativeColumn() >= 0) {
            const QString &entry = model->index(i, printOption->currentAggregativeColumn()).data().toString();
            if (lastValue != entry) {
                if (printOption->isAggregationEntryHidden(entry)) {
                    --x;
                    continue;
                }

                lastValue = entry;
                textTable->insertRows(x, 1);
                textTable->mergeCells(x, 0, 1, textTable->columns());
                textTable->cellAt(x, 0).setFormat(sectionCellFormat);
                textTable->cellAt(x, 0).firstCursorPosition().insertText(lastValue.toString());
                ++x;
            }
        }

        textTable->insertRows(x, 1);
        QTextTableCell cell = textTable->cellAt(x, RscMgr::dir() == Qt::RightToLeft ? textTable->columns() - 1 : 0);
        cell.setFormat(headerCellFormat);
        cell.firstCursorPosition().insertText(model->headerData(i, Qt::Vertical).toString());

        int y = 1; //textTable column index.
        for (int j = 0; j < model->columnCount(); ++j) {
            if (printOption->isColumnHidden(j))
                continue;


            textTable->cellAt(x, RscMgr::dir() == Qt::RightToLeft ? textTable->columns() - y - 1 : y)
                    .firstCursorPosition()
                    .insertText(model->index(i, j).data().toString(), tableCellFormat);
            ++y;
        }
    }

    curser.movePosition(QTextCursor::End);
    curser.insertBlock();

    curser.insertHtml(printOption->commentary());

    doc.print(printer);
}
