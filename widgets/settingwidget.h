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

#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>

class QButtonGroup;
class QPrintPreviewWidget;
class QPrinter;

namespace Ui {
class SettingWidget;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = 0);
    ~SettingWidget();

    static bool changeTheCurrentDatabase();

    void updatePreview() const;

private slots:
    void on_headerImageButton_clicked();
    void on_createDBButton_clicked();
    void on_backupDBButton_clicked();
    void on_changeDBButton_clicked();
    void on_zoomOutButton_clicked();
    void on_zoomInButton_clicked();

    void print(QPrinter *printer);

    QString getPathToSave(const QString &text);

    void loadSettings();

    void postLanguageChangeEvent(int) const;

private:
    void initialize();

    void createConnections() const;

    Ui::SettingWidget *ui;

    QPrintPreviewWidget *m_previewWidget;

    QButtonGroup *m_pageSizeGroup;
    QButtonGroup *m_orientationGroup;
    QButtonGroup *m_colorGroup;
    QButtonGroup *m_headerTypeGroup;
    QButtonGroup *m_headerAlignmentGroup;
};

#endif // SETTINGWIDGET_H
