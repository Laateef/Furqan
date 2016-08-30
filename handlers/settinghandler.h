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

#ifndef SETTINGHANDLER_H
#define SETTINGHANDLER_H

#include <QObject>
#include <QPrinter>
#include <QSettings>

#include "generalenumeration.h"
#include "settingenumeration.h"

class SettingHandler : public QObject
{
    Q_OBJECT

public:
    static SettingHandler *singleton();

public slots:
    void setDatabasePath(const QString &);
    QString databasePath() const;

    void setInterfaceLanguage(int);
    NS_Language::Language interfaceLanguage() const;

    void setPrinterName(const QString &);
    QString printerName() const;

    void setPrinterPageSize(int);
    QPrinter::PageSize printerPageSize() const;

    void setPrinterOrientation(int);
    QPrinter::Orientation printerOrientation() const;

    void setPrinterColorMode(int);
    QPrinter::ColorMode printerColorMode() const;

    void setPrinterTopMargin(int);
    int printerTopMargin() const;

    void setPrinterBottomMargin(int);
    int printerBottomMargin() const;

    void setPrinterRightMargin(int);
    int printerRightMargin() const;

    void setPrinterLeftMargin(int);
    int printerLeftMargin() const;

    void setHeaderType(int);
    NS_HeaderType::HeaderType headerType() const;

    void setHeaderImagePath(const QString &);
    QString headerImagePath() const;

    void setHeaderWidth(int);
    int headerWidth() const;

    void setHeaderHeight(int);
    int headerHeight() const;

    void setHeaderWidthMM(int);
    int headerWidthMM() const;

    void setHeaderHeightMM(int);
    int headerHeightMM() const;

    void setHeaderAlignment(int);
    Qt::Alignment headerAlignment() const;

    void setPreambleFontFamily(const QString &);
    QString preambleFontFamily() const;

    void setPreambleFontSize(const QString &);
    QString preambleFontSize() const;

    void setPreambleFontColor(const QString &);
    QString preambleFontColor() const;

    void setCommentaryFontFamily(const QString &);
    QString commentaryFontFamily() const;

    void setCommentaryFontSize(const QString &);
    QString commentaryFontSize() const;

    void setCommentaryFontColor(const QString &);
    QString commentaryFontColor() const;

    void setTableFontFamily(const QString &);
    QString tableFontFamily() const;

    void setTableFontSize(const QString &);
    QString tableFontSize() const;

    void setTableFontColor(const QString &);
    QString tableFontColor() const;

signals:
    void settingChanged();

private:
    explicit SettingHandler(QObject *parent = 0);

    void setValue(NS_Setting::SettingEntry key, const QVariant &value);
    QVariant value(NS_Setting::SettingEntry key, const QVariant &defaultValue = QVariant()) const;

    QMap<NS_Setting::SettingEntry, QString> m_map;
    QSettings m_settings;
};

#endif // SETTINGHANDLER_H
