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

#include "resourcemanager.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSplashScreen>

TranslatorHandler *RscMgr::translatorHandler()
{
    return TranslatorHandler::singleton();
}

SettingHandler *RscMgr::settingHandler()
{
    return SettingHandler::singleton();
}

DatabaseHandler *RscMgr::databaseHandler()
{
    return DatabaseHandler::singleton();
}

MapHandler *RscMgr::mapHandler()
{
    return MapHandler::singleton();
}

ModelHandler *RscMgr::modelHandler()
{
    return ModelHandler::singleton();
}

PrinterHandler *RscMgr::printHandler()
{
    return PrinterHandler::singleton();
}

Qt::LayoutDirection RscMgr::dir()
{
    switch (RscMgr::settingHandler()->interfaceLanguage()) {
    case NS_Language::Arabic:
        return Qt::RightToLeft;
    case NS_Language::English:
        return Qt::LeftToRight;
    default:
        break;
    }

    return Qt::LeftToRight;
}

void RscMgr::issueMessage(const QString &text, bool ok)
{
    QMessageBox msg;
    msg.setLayoutDirection(RscMgr::dir());
    msg.setText(QString("<font size=+1>%1</font>").arg(text));
    msg.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    msg.setIconPixmap(ok ? QPixmap(":/emotion-happy") : QPixmap(":/emotion-sad"));
    msg.setStyleSheet("QMessageBox {"
                      "   background-color: lightgray;"
                      "}"
                      "QPushButton {"
                      "    background: transparent;"
                      "    padding: 5px;"
                      "}"
                      "QPushButton:hover {"
                      "    color: white;"
                      "    border: 1px solid darkgray;"
                      "}"
                      );

    QPushButton *backButton = new QPushButton("OK");
    backButton->setFocusPolicy(Qt::NoFocus);
    backButton->setIconSize(QSize(32, 32));
    backButton->setToolTip("Enter");

    msg.addButton(backButton, QMessageBox::AcceptRole);
    msg.exec();
}


QSplashScreen *RscMgr::createSplashScreen()
{
    QSplashScreen *splashScreen = new QSplashScreen;
    splashScreen->setAttribute(Qt::WA_DeleteOnClose);
    splashScreen->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    switch (RscMgr::settingHandler()->interfaceLanguage()) {
    case NS_Language::Arabic:
        splashScreen->setPixmap(QPixmap(":/splash-ar"));
        break;
    case NS_Language::English:
        splashScreen->setPixmap(QPixmap(":/splash-en"));
        break;
    default:
        break;
    }
    splashScreen->setWindowModality(Qt::ApplicationModal);
    return splashScreen;
}
