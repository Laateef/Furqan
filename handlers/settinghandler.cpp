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

#include "settinghandler.h"

#include "settingenumeration.h"
#include "resourcemanager.h"

#include <QApplication>

SettingHandler::SettingHandler(QObject *parent) :
    QObject(parent)
{
    m_map.insert(NS_Setting::InterfaceLanguage, "app/language");

    m_map.insert(NS_Setting::DatabasePath, "database/path");

    m_map.insert(NS_Setting::PrinterName, "printer/name");
    m_map.insert(NS_Setting::PrinterPaperSize, "printer/paper-size");
    m_map.insert(NS_Setting::PrinterColorMode, "printer/color-mode");
    m_map.insert(NS_Setting::PrinterOrientation, "printer/orientation");
    m_map.insert(NS_Setting::PrinterLeftMargin, "report/margin-left");
    m_map.insert(NS_Setting::PrinterTopMargin, "report/margin-top");
    m_map.insert(NS_Setting::PrinterRightMargin, "report/margin-right");
    m_map.insert(NS_Setting::PrinterBottomMargin, "report/margin-bottom");

    m_map.insert(NS_Setting::HeaderType, "report/header-type");
    m_map.insert(NS_Setting::HeaderImagePath, "report/header-path");
    m_map.insert(NS_Setting::HeaderWidth, "report/header-width");
    m_map.insert(NS_Setting::HeaderHeight, "report/header-height");
    m_map.insert(NS_Setting::HeaderAlignment, "report/header-alignment");

    m_map.insert(NS_Setting::PreambleFontFamily, "report/preamble/font-family");
    m_map.insert(NS_Setting::PreambleFontSize, "report/preamble/font-size");
    m_map.insert(NS_Setting::PreambleFontColor, "report/preamble/font-color");

    m_map.insert(NS_Setting::CommentaryFontFamily, "report/commentary/font-family");
    m_map.insert(NS_Setting::CommentaryFontSize, "report/commentary/font-size");
    m_map.insert(NS_Setting::CommentaryFontColor, "report/commentary/font-color");

    m_map.insert(NS_Setting::TableFontFamily, "report/table/font-family");
    m_map.insert(NS_Setting::TableFontSize, "report/table/font-size");
    m_map.insert(NS_Setting::TableFontColor, "report/table/-font-color");
}

SettingHandler *SettingHandler::singleton()
{
    static SettingHandler *settingHandler = NULL;

    if (!settingHandler)
        settingHandler = new SettingHandler(qApp);

    return settingHandler;
}

void SettingHandler::setValue(NS_Setting::SettingEntry key, const QVariant &value)
{
    m_settings.setValue(m_map.value(key), value);

    emit settingChanged();
}

QVariant SettingHandler::value(NS_Setting::SettingEntry key, const QVariant &defaultValue) const
{
    return m_settings.value(m_map.value(key), defaultValue);
}

void SettingHandler::setInterfaceLanguage(int lang)
{
    setValue(NS_Setting::InterfaceLanguage, lang);
}

NS_Language::Language SettingHandler::interfaceLanguage() const
{
    return static_cast<NS_Language::Language>(value(NS_Setting::InterfaceLanguage, NS_Language::Arabic).toInt());
}

void SettingHandler::setDatabasePath(const QString &path)
{
    setValue(NS_Setting::DatabasePath, path);
}

QString SettingHandler::databasePath() const
{
    return value(NS_Setting::DatabasePath, "./data.fdb").toString();
}

void SettingHandler::setPrinterName(const QString &name)
{
    setValue(NS_Setting::PrinterName, name);
}

QString SettingHandler::printerName() const
{
    return value(NS_Setting::PrinterName, QString()).toString();
}

void SettingHandler::setPrinterPageSize(int size)
{
    setValue(NS_Setting::PrinterPaperSize, size);
}

QPrinter::PageSize SettingHandler::printerPageSize() const
{
    return static_cast<QPrinter::PageSize>(value(NS_Setting::PrinterPaperSize, QPrinter::A4).toInt());
}

void SettingHandler::setPrinterOrientation(int orientation)
{
    setValue(NS_Setting::PrinterOrientation, orientation);
}

QPrinter::Orientation SettingHandler::printerOrientation() const
{
    return static_cast<QPrinter::Orientation>(value(NS_Setting::PrinterOrientation, QPrinter::Portrait).toInt());
}

void SettingHandler::setPrinterColorMode(int mode)
{
    setValue(NS_Setting::PrinterColorMode, mode);
}

QPrinter::ColorMode SettingHandler::printerColorMode() const
{
    return static_cast<QPrinter::ColorMode>(value(NS_Setting::PrinterColorMode, QPrinter::Color).toInt());
}

void SettingHandler::setPrinterLeftMargin(int margin)
{
    setValue(NS_Setting::PrinterLeftMargin, margin);
}

int SettingHandler::printerLeftMargin() const
{
    return value(NS_Setting::PrinterLeftMargin, 0).toInt();
}

void SettingHandler::setPrinterTopMargin(int margin)
{
    setValue(NS_Setting::PrinterTopMargin, margin);
}

int SettingHandler::printerTopMargin() const
{
    return value(NS_Setting::PrinterTopMargin, 0).toInt();
}

void SettingHandler::setPrinterRightMargin(int margin)
{
    setValue(NS_Setting::PrinterRightMargin, margin);
}

int SettingHandler::printerRightMargin() const
{
    return value(NS_Setting::PrinterRightMargin, 0).toInt();
}

void SettingHandler::setPrinterBottomMargin(int margin)
{
    setValue(NS_Setting::PrinterBottomMargin, margin);
}

int SettingHandler::printerBottomMargin() const
{
    return value(NS_Setting::PrinterBottomMargin, 0).toInt();
}

void SettingHandler::setHeaderType(int type)
{
    setValue(NS_Setting::HeaderType, type);
}

NS_HeaderType::HeaderType SettingHandler::headerType() const
{
    return static_cast<NS_HeaderType::HeaderType>(value(NS_Setting::HeaderType, NS_HeaderType::None).toInt());
}

void SettingHandler::setHeaderImagePath(const QString &path)
{
    setValue(NS_Setting::HeaderImagePath, path);
}

QString SettingHandler::headerImagePath() const
{
    return value(NS_Setting::HeaderImagePath, QString("...")).toString();
}

void SettingHandler::setHeaderWidth(int width)
{
    setHeaderWidthMM(double(width) / RscMgr::printHandler()->previewPrinter()->width() * RscMgr::printHandler()->previewPrinter()->widthMM());
}

int SettingHandler::headerWidth() const
{
    return double(headerWidthMM()) / RscMgr::printHandler()->previewPrinter()->widthMM() * RscMgr::printHandler()->previewPrinter()->width();
}

void SettingHandler::setHeaderHeight(int height)
{
    setHeaderHeightMM(double(height) / RscMgr::printHandler()->previewPrinter()->height() * RscMgr::printHandler()->previewPrinter()->heightMM());
}

int SettingHandler::headerHeight() const
{
    return double(headerHeightMM()) / RscMgr::printHandler()->previewPrinter()->heightMM() * RscMgr::printHandler()->previewPrinter()->height();
}

void SettingHandler::setHeaderWidthMM(int width)
{
    setValue(NS_Setting::HeaderWidth, width);
}

int SettingHandler::headerWidthMM() const
{
    return value(NS_Setting::HeaderWidth, 50).toInt();
}

void SettingHandler::setHeaderHeightMM(int height)
{
    setValue(NS_Setting::HeaderHeight, height);
}

int SettingHandler::headerHeightMM() const
{
    return value(NS_Setting::HeaderHeight, 20).toInt();
}

void SettingHandler::setHeaderAlignment(int align)
{
    setValue(NS_Setting::HeaderAlignment, align);
}

Qt::Alignment SettingHandler::headerAlignment() const
{
    return static_cast<Qt::Alignment>(value(NS_Setting::HeaderAlignment, Qt::AlignHCenter).toInt());
}

void SettingHandler::setPreambleFontFamily(const QString &fontFamily)
{
    setValue(NS_Setting::PreambleFontFamily, fontFamily);
}

QString SettingHandler::preambleFontFamily() const
{
    return value(NS_Setting::PreambleFontFamily, "sakkal majalla").toString();
}

void SettingHandler::setPreambleFontSize(const QString &fontSize)
{
    setValue(NS_Setting::PreambleFontSize, fontSize);
}

QString SettingHandler::preambleFontSize() const
{
    return value(NS_Setting::PreambleFontSize, "14").toString();
}

void SettingHandler::setPreambleFontColor(const QString &fontColor)
{
    setValue(NS_Setting::PreambleFontColor, fontColor);
}

QString SettingHandler::preambleFontColor() const
{
    return value(NS_Setting::PreambleFontColor, QColor(Qt::black).name(QColor::HexRgb)).toString();
}

void SettingHandler::setCommentaryFontFamily(const QString &fontFamily)
{
    setValue(NS_Setting::CommentaryFontFamily, fontFamily);
}

QString SettingHandler::commentaryFontFamily() const
{
    return value(NS_Setting::CommentaryFontFamily, "sakkal majalla").toString();
}

void SettingHandler::setCommentaryFontSize(const QString &fontSize)
{
    setValue(NS_Setting::CommentaryFontSize, fontSize);
}

QString SettingHandler::commentaryFontSize() const
{
    return value(NS_Setting::CommentaryFontSize, "14").toString();
}

void SettingHandler::setCommentaryFontColor(const QString &fontColor)
{
    setValue(NS_Setting::CommentaryFontColor, fontColor);
}

QString SettingHandler::commentaryFontColor() const
{
    return value(NS_Setting::CommentaryFontColor, QColor(Qt::black).name(QColor::HexRgb)).toString();
}

void SettingHandler::setTableFontFamily(const QString &fontFamily)
{
    setValue(NS_Setting::TableFontFamily, fontFamily);
}

QString SettingHandler::tableFontFamily() const
{
    return value(NS_Setting::TableFontFamily, "sakkal majalla").toString();
}

void SettingHandler::setTableFontSize(const QString &fontSize)
{
    setValue(NS_Setting::TableFontSize, fontSize);
}

QString SettingHandler::tableFontSize() const
{
    return value(NS_Setting::TableFontSize, "14").toString();
}

void SettingHandler::setTableFontColor(const QString &fontColor)
{
    setValue(NS_Setting::TableFontColor, fontColor);
}

QString SettingHandler::tableFontColor() const
{
    return value(NS_Setting::TableFontColor, QColor(Qt::black).name(QColor::HexRgb)).toString();
}
