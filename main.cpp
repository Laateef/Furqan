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

#include <QApplication>
#include <QSplashScreen>

#include "resourcemanager.h"
#include "settingwidget.h"
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("ShamSoft");
    QCoreApplication::setApplicationName("Furqan");

    QSplashScreen *splashScreen = RscMgr::createSplashScreen();
    splashScreen->show();

    // To show the splash at the very beginning.
    app.processEvents();

    RscMgr::settingHandler();

    RscMgr::translatorHandler()->translate(RscMgr::settingHandler()->interfaceLanguage());

    //initializing the database,
    //this prevents uninitialized access using static QSqlDatabase static methods.
    RscMgr::databaseHandler();

    if (!RscMgr::databaseHandler()->connection().isOpen()) {
        RscMgr::issueMessage(QObject::tr("Program couldn't open the current database, please select another one"), false);

        if (!SettingWidget::changeTheCurrentDatabase()) {
            RscMgr::issueMessage(QObject::tr("Database is damaged, Program cannot proceed"), false);
            app.quit();
        }
    }

    //initializing the models,
    RscMgr::modelHandler();

    MainWidget *widget = new MainWidget;

    app.setStyleSheet(widget->styleSheet());

    widget->show();

    splashScreen->finish(widget);

    return app.exec();
}
