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

#ifndef MODELHANDLER_H
#define MODELHANDLER_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QSqlTableModel>
#include <QStandardItemModel>

#include "sqlenumeration.h"

class RichSqlModel;

class ModelHandler : public QObject
{
    Q_OBJECT

public:
    static ModelHandler *singleton();

    QSqlTableModel *model(NS_TABLE::TABLE) const;

    QStandardItemModel *previewModel() const;

    void insertRow(NS_TABLE::TABLE table, const QMap<int, QVariant> &map = QMap<int, QVariant>()) const;

    void removeRow(NS_TABLE::TABLE table, int row) const;

    long long newRowId(const QSqlTableModel *model) const;

private slots:
    void initializeModels();
    void initializeHeaders();
    void initializePreviewModel();

    void mapModels();
    void reset();

private:
    explicit ModelHandler(QObject *parent = 0);

    RichSqlModel *m_classModel;
    RichSqlModel *m_courseModel;
    RichSqlModel *m_studentModel;
    RichSqlModel *m_teacherModel;
    RichSqlModel *m_teachingModel;
    RichSqlModel *m_resultModel;
    RichSqlModel *m_quranModel;
    RichSqlModel *m_competitionModel;
    RichSqlModel *m_competitorModel;
    RichSqlModel *m_jobModel;
    RichSqlModel *m_employeeModel;

    QStandardItemModel *m_previewModel;

    QMap<NS_TABLE::TABLE, RichSqlModel *> m_modelMap;
};

#endif // MODELHANDLER_H
