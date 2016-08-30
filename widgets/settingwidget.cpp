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

#include "settingwidget.h"
#include "ui_settingwidget.h"

#include "resourcemanager.h"
#include "printoption.h"

#include <QFileDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintPreviewWidget>
#include <QColorDialog>
#include <QTextDocument>
#include <QTextImageFormat>
#include <QTextCursor>

SettingWidget::SettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingWidget)
{
    ui->setupUi(this);

    initialize();
    createConnections();
    loadSettings();

    m_previewWidget->updatePreview();
    m_previewWidget->setFixedWidth(m_previewWidget->sizeHint().width());

    ui->settingListWidget->setCurrentRow(0);

    ui->languageComboBox->insertItem(NS_Language::Arabic, tr("Arabic"));
    ui->languageComboBox->insertItem(NS_Language::English, tr("English"));

    // No need to indicate the current language in the combo box other than here,
    // because SettingWidget is reconstructed upon each language change.
    ui->languageComboBox->setCurrentIndex(RscMgr::settingHandler()->interfaceLanguage());

    // To avoid infinite language change, we connect the "languageComboBox" here (after population).
    connect(ui->languageComboBox, SIGNAL(currentIndexChanged(int)), SLOT(postLanguageChangeEvent(int)));
}

SettingWidget::~SettingWidget()
{
    delete ui;
}

bool SettingWidget::changeTheCurrentDatabase()
{
    QString newDatabasePath = QFileDialog::getOpenFileName(0, tr("Select a database"),
                                                           QDir::currentPath(),
                                                           "Furqan Database (*.fdb)");

    if (newDatabasePath.isEmpty())
        return false;

    if (!RscMgr::databaseHandler()->ChangeCurrentDatabase(newDatabasePath)) {
        RscMgr::issueMessage(tr("Program cannot use the selected database"), false);
        return false;
    }

    RscMgr::issueMessage(tr("Database Changed"), true);

    return true;
}

void SettingWidget::initialize()
{
    ui->headerImageFrame->hide();

    m_previewWidget = new QPrintPreviewWidget(RscMgr::printHandler()->settingPreviewPrinter());
    m_previewWidget->fitToWidth();

    ui->previewFrame->layout()->addWidget(m_previewWidget);
    ui->previewFrame->setMinimumWidth(500);

    foreach (const QString printerName, QPrinterInfo::availablePrinterNames())
        ui->printerComboBox->addItem(printerName);

    m_pageSizeGroup = new QButtonGroup(this);
    m_pageSizeGroup->addButton(ui->a4RadioButton, QPrinter::A4);
    m_pageSizeGroup->addButton(ui->a5RadioButton, QPrinter::A5);

    m_orientationGroup = new QButtonGroup(this);
    m_orientationGroup->addButton(ui->portraitRadioButton, QPrinter::Portrait);
    m_orientationGroup->addButton(ui->landscapeRadioButton, QPrinter::Landscape);

    m_colorGroup = new QButtonGroup(this);
    m_colorGroup->addButton(ui->grayScaleRadioButton, QPrinter::GrayScale);
    m_colorGroup->addButton(ui->colorRadioButton, QPrinter::Color);

    m_headerTypeGroup = new QButtonGroup(this);
    m_headerTypeGroup->addButton(ui->headerNoneRadioButton, NS_HeaderType::None);
    m_headerTypeGroup->addButton(ui->headerImageRadioButton, NS_HeaderType::Image);

    m_headerAlignmentGroup = new QButtonGroup(this);
    m_headerAlignmentGroup->addButton(ui->headerAlignLeftButton, Qt::AlignLeft);
    m_headerAlignmentGroup->addButton(ui->headerAlignCenterButton, Qt::AlignHCenter);
    m_headerAlignmentGroup->addButton(ui->headerAlignRightButton, Qt::AlignRight);
}

void SettingWidget::createConnections() const
{
    connect(ui->printerComboBox, SIGNAL(currentIndexChanged(QString)), RscMgr::settingHandler(), SLOT(setPrinterName(QString)));
    connect(m_previewWidget, SIGNAL(paintRequested(QPrinter*)), SLOT(print(QPrinter*)));

    connect(RscMgr::databaseHandler(), SIGNAL(currentDatabaseChanged(QString)), RscMgr::settingHandler(), SLOT(setDatabasePath(QString)));
    connect(RscMgr::databaseHandler(), SIGNAL(currentDatabaseChanged(QString)), ui->dbPathLabel, SLOT(setText(QString)));

    connect(m_pageSizeGroup, SIGNAL(buttonClicked(int)), RscMgr::settingHandler(), SLOT(setPrinterPageSize(int)));
    connect(m_orientationGroup, SIGNAL(buttonClicked(int)), RscMgr::settingHandler(), SLOT(setPrinterOrientation(int)));
    connect(m_colorGroup, SIGNAL(buttonClicked(int)), RscMgr::settingHandler(), SLOT(setPrinterColorMode(int)));
    connect(m_headerTypeGroup, SIGNAL(buttonClicked(int)), RscMgr::settingHandler(), SLOT(setHeaderType(int)));
    connect(m_headerAlignmentGroup, SIGNAL(buttonClicked(int)), RscMgr::settingHandler(), SLOT(setHeaderAlignment(int)));

    connect(ui->leftSpinBox, SIGNAL(valueChanged(int)), RscMgr::settingHandler(), SLOT(setPrinterLeftMargin(int)));
    connect(ui->topSpinBox, SIGNAL(valueChanged(int)), RscMgr::settingHandler(), SLOT(setPrinterTopMargin(int)));
    connect(ui->rightSpinBox, SIGNAL(valueChanged(int)), RscMgr::settingHandler(), SLOT(setPrinterRightMargin(int)));
    connect(ui->bottomSpinBox, SIGNAL(valueChanged(int)), RscMgr::settingHandler(), SLOT(setPrinterBottomMargin(int)));

    connect(ui->headerWidthSpinBox, SIGNAL(valueChanged(int)), RscMgr::settingHandler(), SLOT(setHeaderWidthMM(int)));
    connect(ui->headerHeightSpinBox, SIGNAL(valueChanged(int)), RscMgr::settingHandler(), SLOT(setHeaderHeightMM(int)));

    connect(RscMgr::settingHandler(), SIGNAL(settingChanged()), SLOT(loadSettings()));
    connect(RscMgr::settingHandler(), SIGNAL(settingChanged()), m_previewWidget, SLOT(updatePreview()));
}

void SettingWidget::on_headerImageButton_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this,
                                                     QString(),
                                                     QDir::currentPath(),
                                                     "Image Files (*.png *.jpg *.jpeg *.bmp)");
    if (!imagePath.isEmpty()) {
        imagePath = QDir::toNativeSeparators(imagePath);
        ui->headerImagePathLabel->setText(imagePath);
        RscMgr::settingHandler()->setHeaderImagePath(imagePath);
    }
}

void SettingWidget::loadSettings()
{
    ui->printerComboBox->setCurrentText(RscMgr::settingHandler()->printerName());
    ui->dbPathLabel->setText(RscMgr::settingHandler()->databasePath());

    m_pageSizeGroup->button(RscMgr::settingHandler()->printerPageSize())->setChecked(true);
    m_orientationGroup->button(RscMgr::settingHandler()->printerOrientation())->setChecked(true);
    m_colorGroup->button(RscMgr::settingHandler()->printerColorMode())->setChecked(true);
    m_headerTypeGroup->button(RscMgr::settingHandler()->headerType())->setChecked(true);
    m_headerAlignmentGroup->button(RscMgr::settingHandler()->headerAlignment())->setChecked(true);

    ui->leftSpinBox->setValue(RscMgr::settingHandler()->printerLeftMargin());
    ui->topSpinBox->setValue(RscMgr::settingHandler()->printerTopMargin());
    ui->rightSpinBox->setValue(RscMgr::settingHandler()->printerRightMargin());
    ui->bottomSpinBox->setValue(RscMgr::settingHandler()->printerBottomMargin());

    ui->headerWidthSpinBox->setValue(RscMgr::settingHandler()->headerWidthMM());
    ui->headerHeightSpinBox->setValue(RscMgr::settingHandler()->headerHeightMM());

    ui->headerImagePathLabel->setText(RscMgr::settingHandler()->headerImagePath());
}

void SettingWidget::postLanguageChangeEvent(int lang) const
{
    if (lang < 0 || lang > NS_Language::English || lang == RscMgr::settingHandler()->interfaceLanguage())
        return;

    // Beacause subsequent operations depend on the settingHandler's new language,
    // we set the new language here.
    RscMgr::settingHandler()->setInterfaceLanguage(lang);

    RscMgr::translatorHandler()->translate(static_cast<NS_Language::Language>(lang));

    // Post a language change to re-initialize the main widget.
    qApp->postEvent(qApp, new QEvent(QEvent::LanguageChange));
}

void SettingWidget::updatePreview() const
{
    m_previewWidget->updatePreview();
}

void SettingWidget::print(QPrinter *printer)
{
    PrintOption opt;
    RscMgr::printHandler()->print(RscMgr::modelHandler()->previewModel(), &opt, printer);
}

QString SettingWidget::getPathToSave(const QString &text)
{
    QString path = QFileDialog::getSaveFileName(this, text,
                                                QDir::currentPath(),
                                                "Furqan Database (*.fdb)",
                                                0,
                                                QFileDialog::DontConfirmOverwrite);

    if (QFile(path).exists()) {
        RscMgr::issueMessage(tr("A database with the same name exists !"), false);
        return QString();
    }

    if (!path.isEmpty() && !path.endsWith(".fdb", Qt::CaseInsensitive))
        path.append(".fdb");

    return path;
}

void SettingWidget::on_backupDBButton_clicked()
{
    const QString &path = getPathToSave(tr("Select where to save the back up"));
    if (path.isEmpty())
        return;

    if (!RscMgr::databaseHandler()->backupCurrentDatabase(path)) {
        RscMgr::issueMessage(tr("Process cannot complete, an error occured"), false);
        return;
    }

    RscMgr::issueMessage(tr("Backup Created"), true);
}

void SettingWidget::on_changeDBButton_clicked()
{
    changeTheCurrentDatabase();
}

void SettingWidget::on_zoomOutButton_clicked()
{
    m_previewWidget->zoomOut();
}

void SettingWidget::on_zoomInButton_clicked()
{
    m_previewWidget->zoomIn();
}

void SettingWidget::on_createDBButton_clicked()
{    
    const QString &path = getPathToSave(tr("Select where to save the new database"));
    if (path.isEmpty())
        return;

    if (!RscMgr::databaseHandler()->createNewDatabase(path)) {
        RscMgr::issueMessage(tr("Process cannot complete, an error occured"), false);
        return;
    }

    RscMgr::issueMessage(tr("Database Created"), true);
}
