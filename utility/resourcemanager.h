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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "settinghandler.h"
#include "translatorhandler.h"
#include "databasehandler.h"
#include "maphandler.h"
#include "modelhandler.h"
#include "printerhandler.h"

class QSqlDatabase;
class QSplashScreen;

namespace RscMgr {

SettingHandler *settingHandler();

TranslatorHandler *translatorHandler();

DatabaseHandler *databaseHandler();

MapHandler *mapHandler();

ModelHandler *modelHandler();

PrinterHandler *printHandler();

Qt::LayoutDirection dir();

void issueMessage(const QString &text, bool ok);

QSplashScreen *createSplashScreen();
}

#endif // RESOURCEMANAGER_H
