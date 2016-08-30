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

#ifndef SETTINGENUMERATION_H
#define SETTINGENUMERATION_H

namespace NS_Setting {
enum SettingEntry {
    InterfaceLanguage,

    DatabasePath,

    PrinterName,
    PrinterPaperSize,
    PrinterColorMode,
    PrinterOrientation,
    PrinterTopMargin,
    PrinterBottomMargin,
    PrinterRightMargin,
    PrinterLeftMargin,

    HeaderType,
    HeaderImagePath,
    HeaderWidth,
    HeaderHeight,
    HeaderAlignment,

    PreambleFontFamily,
    PreambleFontSize,
    PreambleFontColor,

    CommentaryFontFamily,
    CommentaryFontSize,
    CommentaryFontColor,

    TableFontFamily,
    TableFontSize,
    TableFontColor
};
}

#endif // SETTINGENUMERATION_H
