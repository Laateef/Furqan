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

#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseHandler : public QObject
{
    Q_OBJECT

public:
    static DatabaseHandler *singleton();

    QSqlDatabase &connection();

    bool createNewDatabase(const QString &newDatabasePath);

    bool ChangeCurrentDatabase(const QString &newDatabasePath);

    bool backupCurrentDatabase(const QString &backupDatabasePath);

    ~DatabaseHandler();

signals:
    void currentDatabaseChanged(const QString &) const;

private:
    explicit DatabaseHandler(QObject *parent = 0);

    const QString databaseFileExtension;

    bool isPathValid(const QString &path) const;

    void validatePath(QString &path) const;

    bool copyDatabase(const QString &sourcePath, const QString &targetPath);

    QSqlDatabase m_database;
};

#endif // DATABASEHANDLER_H
