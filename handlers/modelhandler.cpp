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

#include "modelhandler.h"

#include "resourcemanager.h"
#include "richsqlmodel.h"
#include "maprelation.h"
#include "sqlrelation.h"

#include <QApplication>
#include <QSqlQuery>

ModelHandler::ModelHandler(QObject *parent) :
    QObject(parent),
    m_classModel(new RichSqlModel(this)),
    m_courseModel(new RichSqlModel(this)),
    m_studentModel(new RichSqlModel(this)),
    m_teacherModel(new RichSqlModel(this)),
    m_teachingModel(new RichSqlModel(this)),
    m_resultModel(new RichSqlModel(this)),
    m_quranModel(new RichSqlModel(this)),
    m_competitionModel(new RichSqlModel(this)),
    m_competitorModel(new RichSqlModel(this)),
    m_jobModel(new RichSqlModel(this)),
    m_employeeModel(new RichSqlModel(this)),
    m_previewModel(new QStandardItemModel(3, 3, this)),
    m_modelMap()
{    
    mapModels();

    initializeModels();
    initializeHeaders();
    initializePreviewModel();

    connect(RscMgr::translatorHandler(), SIGNAL(languageChanged()), SLOT(initializeHeaders()));
    connect(RscMgr::translatorHandler(), SIGNAL(languageChanged()), SLOT(initializePreviewModel()));
    connect(RscMgr::databaseHandler(), SIGNAL(currentDatabaseChanged(QString)), SLOT(reset()));
}

ModelHandler *ModelHandler::singleton()
{
    static ModelHandler *modelHandler = NULL;

    if (!modelHandler)
        modelHandler = new ModelHandler(qApp);

    return modelHandler;
}

void ModelHandler::initializeModels()
{
    m_classModel->setTable("CLASS");
    m_classModel->setSort(NS_CLASS::ID, Qt::AscendingOrder);
    m_classModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_classModel->setRelation(NS_CLASS::LEVEL, new MapRelation(RscMgr::mapHandler()->map(NS_MAP::LEVEL), m_classModel));
    m_classModel->select();

    m_courseModel->setTable("COURSE");
    m_courseModel->setSort(NS_COURSE::ID, Qt::AscendingOrder);
    m_courseModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_courseModel->setRelation(NS_COURSE::SEMESTER, new MapRelation(RscMgr::mapHandler()->map(NS_MAP::SEMESTER), m_courseModel));
    m_courseModel->select();

    m_studentModel->setTable("STUDENT");
    m_studentModel->setSort(NS_STUDENT::ID, Qt::AscendingOrder);
    m_studentModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_studentModel->setRelation(NS_STUDENT::GENDER, new MapRelation(RscMgr::mapHandler()->map(NS_MAP::GENDER), m_studentModel));
    m_studentModel->setRelation(NS_STUDENT::LEVEL, new MapRelation(RscMgr::mapHandler()->map(NS_MAP::LEVEL), m_studentModel));
    m_studentModel->setRelation(NS_STUDENT::CLASS, new SqlRelation(m_classModel, NS_CLASS::ID, NS_CLASS::NAME, m_studentModel));
    m_studentModel->select();

    m_teacherModel->setTable("TEACHER");
    m_teacherModel->setSort(NS_TEACHER::ID, Qt::AscendingOrder);
    m_teacherModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_teacherModel->setRelation(NS_TEACHER::GENDER, new MapRelation(RscMgr::mapHandler()->map(NS_MAP::GENDER), m_teacherModel));
    m_teacherModel->select();

    m_teachingModel->setTable("TEACHING");
    m_teachingModel->setSort(NS_TEACHING::ID, Qt::AscendingOrder);
    m_teachingModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_teachingModel->setRelation(NS_TEACHING::TEACHER, new SqlRelation(m_teacherModel, NS_TEACHER::ID, NS_TEACHER::FULL_NAME, m_teachingModel));
    m_teachingModel->setRelation(NS_TEACHING::CLASS, new SqlRelation(m_classModel, NS_CLASS::ID, NS_CLASS::NAME, m_teachingModel));
    m_teachingModel->setRelation(NS_TEACHING::COURSE, new SqlRelation(m_courseModel, NS_COURSE::ID, NS_COURSE::NAME, m_teachingModel));
    m_teachingModel->select();

    m_resultModel->setTable("RESULT");
    m_resultModel->setSort(NS_RESULT::ID, Qt::AscendingOrder);
    m_resultModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_resultModel->setRelation(NS_RESULT::COURSE, new SqlRelation(m_courseModel, NS_COURSE::ID, NS_COURSE::NAME, m_resultModel));
    m_resultModel->setRelation(NS_RESULT::STUDENT, new SqlRelation(m_studentModel, NS_STUDENT::ID, NS_STUDENT::FULL_NAME, m_resultModel));
    m_resultModel->select();

    m_quranModel->setTable("QURAN");
    m_quranModel->setSort(NS_QURAN::ID, Qt::AscendingOrder);
    m_quranModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_quranModel->setRelation(NS_QURAN::RECITAL_TYPE, new MapRelation(RscMgr::mapHandler()->map(NS_MAP::RECITAL), m_quranModel));
    m_quranModel->setRelation(NS_QURAN::STUDENT, new SqlRelation(m_studentModel, NS_STUDENT::ID, NS_STUDENT::FULL_NAME, m_quranModel));
    m_quranModel->select();

    m_competitionModel->setTable("COMPETITION");
    m_competitionModel->setSort(NS_COMPETITION::ID, Qt::AscendingOrder);
    m_competitionModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_competitionModel->setRelation(NS_COMPETITION::TYPE, new MapRelation(RscMgr::mapHandler()->map(NS_MAP::COMPETITION), m_competitionModel));
    m_competitionModel->select();

    m_competitorModel->setTable("COMPETITOR");
    m_competitorModel->setSort(NS_COMPETITOR::ID, Qt::AscendingOrder);
    m_competitorModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_competitorModel->setRelation(NS_COMPETITOR::COMPETITION, new SqlRelation(m_competitionModel, NS_COMPETITION::ID, NS_COMPETITION::NAME, m_competitorModel));
    m_competitorModel->setRelation(NS_COMPETITOR::STUDENT, new SqlRelation(m_studentModel, NS_STUDENT::ID, NS_STUDENT::FULL_NAME, m_competitorModel));
    m_competitorModel->select();

    m_jobModel->setTable("JOB");
    m_jobModel->setSort(NS_JOB::ID, Qt::AscendingOrder);
    m_jobModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_jobModel->select();

    m_employeeModel->setTable("EMPLOYEE");
    m_employeeModel->setSort(NS_EMPLOYEE::ID, Qt::AscendingOrder);
    m_employeeModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_employeeModel->setRelation(NS_EMPLOYEE::GENDER, new MapRelation(RscMgr::mapHandler()->map(NS_MAP::GENDER), m_employeeModel));
    m_employeeModel->setRelation(NS_EMPLOYEE::JOB, new SqlRelation(m_jobModel, NS_JOB::ID, NS_JOB::NAME, m_employeeModel));
    m_employeeModel->select();
}

void ModelHandler::initializeHeaders()
{
    m_classModel->setHeaderData(NS_CLASS::ID, Qt::Horizontal, tr("ID"));
    m_classModel->setHeaderData(NS_CLASS::NAME, Qt::Horizontal, tr("Name"));
    m_classModel->setHeaderData(NS_CLASS::LEVEL, Qt::Horizontal, tr("Level"));

    m_courseModel->setHeaderData(NS_COURSE::ID, Qt::Horizontal, tr("ID"));
    m_courseModel->setHeaderData(NS_COURSE::NAME, Qt::Horizontal, tr("Name"));
    m_courseModel->setHeaderData(NS_COURSE::SEMESTER, Qt::Horizontal, tr("Semester"));

    m_studentModel->setHeaderData(NS_STUDENT::ID, Qt::Horizontal, tr("ID"));
    m_studentModel->setHeaderData(NS_STUDENT::FULL_NAME, Qt::Horizontal, tr("Full Name"));
    m_studentModel->setHeaderData(NS_STUDENT::FATHER_NAME, Qt::Horizontal, tr("Father Name"));
    m_studentModel->setHeaderData(NS_STUDENT::GENDER, Qt::Horizontal, tr("Gender"));
    m_studentModel->setHeaderData(NS_STUDENT::BIRTH_PLACE, Qt::Horizontal, tr("Birth Place"));
    m_studentModel->setHeaderData(NS_STUDENT::BIRTH_DATE, Qt::Horizontal, tr("Birth Date"));
    m_studentModel->setHeaderData(NS_STUDENT::RESIDENCY, Qt::Horizontal, tr("Residency"));
    m_studentModel->setHeaderData(NS_STUDENT::NATIONALITY, Qt::Horizontal, tr("Nationality"));
    m_studentModel->setHeaderData(NS_STUDENT::NATIONAL_ID, Qt::Horizontal, tr("National Number"));
    m_studentModel->setHeaderData(NS_STUDENT::MOBILE, Qt::Horizontal, tr("Mobile"));
    m_studentModel->setHeaderData(NS_STUDENT::FATHER_JOB, Qt::Horizontal, tr("Father Job"));
    m_studentModel->setHeaderData(NS_STUDENT::LEVEL, Qt::Horizontal, tr("Level"));
    m_studentModel->setHeaderData(NS_STUDENT::CLASS, Qt::Horizontal, tr("Class"));

    m_teacherModel->setHeaderData(NS_TEACHER::ID, Qt::Horizontal, tr("ID"));
    m_teacherModel->setHeaderData(NS_TEACHER::FULL_NAME, Qt::Horizontal, tr("Full Name"));
    m_teacherModel->setHeaderData(NS_TEACHER::FATHER_NAME, Qt::Horizontal, tr("Father Name"));
    m_teacherModel->setHeaderData(NS_TEACHER::GENDER, Qt::Horizontal, tr("Gender"));
    m_teacherModel->setHeaderData(NS_TEACHER::BIRTH_PLACE, Qt::Horizontal, tr("Birth Place"));
    m_teacherModel->setHeaderData(NS_TEACHER::BIRTH_DATE, Qt::Horizontal, tr("Birth Date"));
    m_teacherModel->setHeaderData(NS_TEACHER::RESIDENCY, Qt::Horizontal, tr("Residency"));
    m_teacherModel->setHeaderData(NS_TEACHER::NATIONALITY, Qt::Horizontal, tr("Nationality"));
    m_teacherModel->setHeaderData(NS_TEACHER::NATIONAL_ID, Qt::Horizontal, tr("National Number"));
    m_teacherModel->setHeaderData(NS_TEACHER::MOBILE, Qt::Horizontal, tr("Mobile"));
    m_teacherModel->setHeaderData(NS_TEACHER::QUALIFICATION, Qt::Horizontal, tr("Qualification"));
    m_teacherModel->setHeaderData(NS_TEACHER::EXPERIENCE, Qt::Horizontal, tr("Experience"));

    m_teachingModel->setHeaderData(NS_TEACHING::ID, Qt::Horizontal, tr("ID"));
    m_teachingModel->setHeaderData(NS_TEACHING::TEACHER, Qt::Horizontal, tr("Teacher"));
    m_teachingModel->setHeaderData(NS_TEACHING::CLASS, Qt::Horizontal, tr("Class"));
    m_teachingModel->setHeaderData(NS_TEACHING::COURSE, Qt::Horizontal, tr("Course"));

    m_resultModel->setHeaderData(NS_RESULT::ID, Qt::Horizontal, tr("ID"));
    m_resultModel->setHeaderData(NS_RESULT::COURSE, Qt::Horizontal, tr("Course"));
    m_resultModel->setHeaderData(NS_RESULT::STUDENT, Qt::Horizontal, tr("Student"));
    m_resultModel->setHeaderData(NS_RESULT::RESULT, Qt::Horizontal, tr("Result"));

    m_quranModel->setHeaderData(NS_QURAN::ID, Qt::Horizontal, tr("ID"));
    m_quranModel->setHeaderData(NS_QURAN::STUDENT, Qt::Horizontal, tr("Student"));
    m_quranModel->setHeaderData(NS_QURAN::RECITAL_TYPE, Qt::Horizontal, tr("Recital Type"));
    m_quranModel->setHeaderData(NS_QURAN::FROM_PAGE, Qt::Horizontal, tr("From Page"));
    m_quranModel->setHeaderData(NS_QURAN::TO_PAGE, Qt::Horizontal, tr("To Page"));
    m_quranModel->setHeaderData(NS_QURAN::DATE, Qt::Horizontal, tr("Date"));

    m_competitionModel->setHeaderData(NS_COMPETITION::ID, Qt::Horizontal, tr("ID"));
    m_competitionModel->setHeaderData(NS_COMPETITION::NAME, Qt::Horizontal, tr("Name"));
    m_competitionModel->setHeaderData(NS_COMPETITION::TYPE, Qt::Horizontal, tr("Type"));
    m_competitionModel->setHeaderData(NS_COMPETITION::PARTS, Qt::Horizontal, tr("Part(s)"));
    m_competitionModel->setHeaderData(NS_COMPETITION::DATE, Qt::Horizontal, tr("Date"));

    m_competitorModel->setHeaderData(NS_COMPETITOR::ID, Qt::Horizontal, tr("ID"));
    m_competitorModel->setHeaderData(NS_COMPETITOR::COMPETITION, Qt::Horizontal, tr("Competition"));
    m_competitorModel->setHeaderData(NS_COMPETITOR::STUDENT, Qt::Horizontal, tr("Student"));
    m_competitorModel->setHeaderData(NS_COMPETITOR::RANK, Qt::Horizontal, tr("Rank"));

    m_jobModel->setHeaderData(NS_JOB::ID, Qt::Horizontal, tr("ID"));
    m_jobModel->setHeaderData(NS_JOB::NAME, Qt::Horizontal, tr("Name"));

    m_employeeModel->setHeaderData(NS_EMPLOYEE::ID, Qt::Horizontal, tr("ID"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::FULL_NAME, Qt::Horizontal, tr("Full Name"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::FATHER_NAME, Qt::Horizontal, tr("Father Name"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::GENDER, Qt::Horizontal, tr("Gender"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::BIRTH_PLACE, Qt::Horizontal, tr("Birth Place"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::BIRTH_DATE, Qt::Horizontal, tr("Birth Date"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::RESIDENCY, Qt::Horizontal, tr("Residency"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::NATIONALITY, Qt::Horizontal, tr("Nationality"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::NATIONAL_ID, Qt::Horizontal, tr("National Number"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::MOBILE, Qt::Horizontal, tr("Mobile"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::JOB, Qt::Horizontal, tr("Job"));
    m_employeeModel->setHeaderData(NS_EMPLOYEE::OTHER_JOB, Qt::Horizontal, tr("Other Job"));
}

void ModelHandler::initializePreviewModel()
{
    for (int column = 0; column < m_previewModel->columnCount(); ++column) {
        m_previewModel->setHeaderData(column, Qt::Horizontal, tr("Header") + QString("%1").arg(column + 1));
    }

    for (int row = 0; row < m_previewModel->rowCount(); ++row) {
        for (int column = 0; column < m_previewModel->columnCount(); ++column) {
            QStandardItem *item = new QStandardItem(tr("Cell") + QString("%1%2").arg(column + 1).arg(row + 1));
            m_previewModel->setItem(row, column, item);
        }
    }
}

void ModelHandler::mapModels()
{
    m_modelMap.clear();
    m_modelMap.insert(NS_TABLE::CLASS, m_classModel);
    m_modelMap.insert(NS_TABLE::COURSE, m_courseModel);
    m_modelMap.insert(NS_TABLE::STUDENT, m_studentModel);
    m_modelMap.insert(NS_TABLE::TEACHER, m_teacherModel);
    m_modelMap.insert(NS_TABLE::TEACHING, m_teachingModel);
    m_modelMap.insert(NS_TABLE::RESULT, m_resultModel);
    m_modelMap.insert(NS_TABLE::QURAN, m_quranModel);
    m_modelMap.insert(NS_TABLE::COMPETITION, m_competitionModel);
    m_modelMap.insert(NS_TABLE::COMPETITOR, m_competitorModel);
    m_modelMap.insert(NS_TABLE::JOB, m_jobModel);
    m_modelMap.insert(NS_TABLE::EMPLOYEE, m_employeeModel);
}

void ModelHandler::reset()
{
    foreach (RichSqlModel *model, m_modelMap.values()) {
        //To use the new database connection, we reset the table name.
        model->setTable(model->tableName());
        model->select();
    }

    initializeHeaders();
}

QSqlTableModel *ModelHandler::model(NS_TABLE::TABLE table) const
{
    return m_modelMap.value(table, NULL);
}

QStandardItemModel *ModelHandler::previewModel() const
{
    return m_previewModel;
}

void ModelHandler::insertRow(NS_TABLE::TABLE table, const QMap<int, QVariant> &map) const
{
    QSqlTableModel *m = model(table);

    m->insertRow(0);
    m->setData(m->index(0, 0), newRowId(m));

    foreach (const QVariant &value, map)
        m->setData(m->index(0, map.key(value)), value);

    m->submit();
}

long long ModelHandler::newRowId(const QSqlTableModel *model) const
{
    QSqlQuery qry(QString("SELECT MAX(ID) FROM %1 ;").arg(model->tableName()), model->database());
    qry.exec();
    qry.next();

    return qry.value(0).toLongLong() + 1;
}

void ModelHandler::removeRow(NS_TABLE::TABLE table, int row) const
{
    QSqlTableModel *m = model(table);

    m->removeRow(row);
    m->submit();
    m->select();
}
