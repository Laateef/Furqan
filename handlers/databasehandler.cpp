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

#include "databasehandler.h"
#include "resourcemanager.h"

#include <QApplication>
#include <QFile>

DatabaseHandler::DatabaseHandler(QObject *parent) :
    QObject(parent),
    databaseFileExtension(".fdb")
{
    m_database = QSqlDatabase::addDatabase("QIBASE");
    m_database.setDatabaseName(RscMgr::settingHandler()->databasePath());
    m_database.setUserName("SYSDBA");
    m_database.open();
}

DatabaseHandler::~DatabaseHandler() {
    if (m_database.isOpen())
        m_database.close();
}

DatabaseHandler *DatabaseHandler::singleton()
{
    static DatabaseHandler *databaseHandler = NULL;

    if (!databaseHandler)
        databaseHandler = new DatabaseHandler(qApp);

    return databaseHandler;
}

QSqlDatabase &DatabaseHandler::connection()
{
    return m_database;
}

bool DatabaseHandler::createNewDatabase(const QString &newDatabasePath)
{
    return copyDatabase(":/empty-database", newDatabasePath);
}

bool DatabaseHandler::ChangeCurrentDatabase(const QString &newDatabasePath)
{
    // Check whether the given path ends with the extension indicated to by "databaseFileExtension" data member.
    if (!isPathValid(newDatabasePath))
        return false;

    if (m_database.isOpen())
        m_database.close();

    m_database.setDatabaseName(newDatabasePath);

    if (!m_database.open()) {
        return false;
    }

    RscMgr::settingHandler()->setDatabasePath(newDatabasePath);

    emit currentDatabaseChanged(newDatabasePath);

    return true;
}

bool DatabaseHandler::backupCurrentDatabase(const QString &backupDatabasePath)
{
    return copyDatabase(RscMgr::settingHandler()->databasePath(), backupDatabasePath);
}

bool DatabaseHandler::isPathValid(const QString &path) const
{
    return path.endsWith(databaseFileExtension);
}

void DatabaseHandler::validatePath(QString &path) const
{
    if (!path.endsWith(databaseFileExtension))
        path.append(databaseFileExtension);
}

bool DatabaseHandler::copyDatabase(const QString &sourcePath, const QString &targetPath)
{
    QFile file;
    file.setFileName(sourcePath);

    if (file.copy(targetPath)) {
        QFile(targetPath).setPermissions(QFile::ReadOther | QFile::WriteOther);
        return true;
    }

    return false;
}
