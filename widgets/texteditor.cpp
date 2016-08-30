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

#include "texteditor.h"

#include "resourcemanager.h"

#include <QAction>
#include <QApplication>
#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QFontComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextCharFormat>
#include <QToolBar>
#include <QVBoxLayout>

TextEditor::TextEditor(QWidget *parent)
    : QWidget(parent), m_colorDialog(0)
{
    createWidgets();
    createLayout();
    createConnections();

    updateColorSwatch();

    m_textEdit->setFocus();
}

void TextEditor::createWidgets()
{
    QFont font("Arial", 14);

    m_textEdit = new QTextEdit;
    m_textEdit->setAcceptRichText(true);
    m_textEdit->setAlignment(RscMgr::dir() == Qt::LeftToRight ? Qt::AlignLeft : Qt::AlignRight);
    m_textEdit->setTabChangesFocus(true);
    m_textEdit->setCurrentFont(font);

    m_fontToolBar = new QToolBar;
    m_fontToolBar->setIconSize(QSize(20, 20));

    m_fontFamilyCombo = new QFontComboBox;
    m_fontFamilyCombo->setCurrentFont(font);

    m_fontToolBar->addWidget(m_fontFamilyCombo);

    m_fontSizeSpin = new QDoubleSpinBox;
    m_fontSizeSpin->setFixedHeight(20);
    m_fontSizeSpin->setDecimals(0);
    m_fontSizeSpin->setRange(6, 280);
    m_fontSizeSpin->setValue(font.pointSize());

    m_fontToolBar->addWidget(m_fontSizeSpin);

    m_boldAction = m_fontToolBar->addAction(QIcon(":/bold"), tr("Bold"));
    m_boldAction->setCheckable(true);
    m_boldAction->setShortcut(tr("Ctrl+B"));

    m_italicAction = m_fontToolBar->addAction(QIcon(":/italic"), tr("Italic"));
    m_italicAction->setCheckable(true);
    m_italicAction->setShortcut(tr("Ctrl+I"));

    m_underlineAction = m_fontToolBar->addAction(QIcon(":/underline"), tr("Underline"));
    m_underlineAction->setCheckable(true);
    m_underlineAction->setShortcut(tr("Ctrl+U"));

    m_colorAction = m_fontToolBar->addAction(tr("Color"));
}

void TextEditor::createLayout()
{
    QHBoxLayout *toolbarLayout = new QHBoxLayout;
    toolbarLayout->setContentsMargins(2, 2, 2, 2);
    toolbarLayout->addWidget(m_fontToolBar);

    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    frame->setLayout(toolbarLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(3);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(frame);
    layout->addWidget(m_textEdit);
    setLayout(layout);
}

void TextEditor::createConnections()
{
    connect(m_fontFamilyCombo, SIGNAL(currentFontChanged(const QFont&)),
            this, SLOT(setFontFamily(const QFont&)));
    connect(m_fontSizeSpin, SIGNAL(valueChanged(double)),
            this, SLOT(setFontPointSize(double)));
    connect(m_boldAction, SIGNAL(toggled(bool)),
            this, SLOT(setBold(bool)));
    connect(m_italicAction, SIGNAL(toggled(bool)),
            m_textEdit, SLOT(setFontItalic(bool)));
    connect(m_underlineAction, SIGNAL(toggled(bool)),
            m_textEdit, SLOT(setFontUnderline(bool)));
    connect(m_colorAction, SIGNAL(triggered()),
            this, SLOT(setColor()));
    connect(m_textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat&)),
            this, SLOT(currentCharFormatChanged(const QTextCharFormat&)));
    connect(m_textEdit, SIGNAL(textChanged()),
            this, SIGNAL(textChanged()));
}

void TextEditor::currentCharFormatChanged(const QTextCharFormat &format)
{
    m_fontFamilyCombo->setCurrentFont(format.font());
    m_fontSizeSpin->setValue(format.fontPointSize());
    m_boldAction->setChecked(format.fontWeight() == QFont::Bold);
    m_italicAction->setChecked(format.fontItalic());
    m_underlineAction->setChecked(format.fontUnderline());

    updateColorSwatch();
}

void TextEditor::setColor()
{
    if (!m_colorDialog) {
        m_colorDialog = new QColorDialog(this);
        connect(m_colorDialog, SIGNAL(colorSelected(const QColor&)),
                this, SLOT(updateColor(const QColor&)));
    }
    m_colorDialog->setCurrentColor(m_textEdit->textColor());
    m_colorDialog->open();
}

void TextEditor::updateColor(const QColor &color)
{
    m_textEdit->setTextColor(color);
    updateColorSwatch();
    emit fontColorChanged(color.name(QColor::HexRgb));
}

void TextEditor::updateColorSwatch()
{
    QPixmap colorSwatch(32, 32);
    colorSwatch.fill(m_textEdit->textColor());
    m_colorAction->setIcon(QIcon(colorSwatch));
}
