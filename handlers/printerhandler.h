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

#ifndef PRINTERHANDLER_H
#define PRINTERHANDLER_H

#include <QObject>
#include <QPrinter>

class QAbstractItemModel;

class PrintOption;

class PrinterHandler : public QObject
{
    Q_OBJECT

public:
    explicit PrinterHandler(QObject *parent = 0);

    static PrinterHandler *singleton();

    QPrinter *nativePrinter();

    QPrinter *previewPrinter();

    QPrinter *settingPreviewPrinter();

    static void print(QAbstractItemModel *model, PrintOption *printOption, QPrinter *printer);

private slots:
    void initialize();

private:
    QPrinter m_nativePrinter;
    QPrinter m_previewPrinter;
    QPrinter m_settingPreviewPrinter;
};

#endif // PRINTERHANDLER_H
