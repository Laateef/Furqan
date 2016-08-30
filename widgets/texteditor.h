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

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>
#include <QWidget>

class QColorDialog;
class QDoubleSpinBox;
class QFontComboBox;
class QTextCharFormat;
class QToolBar;

class TextEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditor(QWidget *parent=0);

    QString toHtml() const { return m_textEdit->toHtml(); }
    QString toPlainText() const { return m_textEdit->toPlainText(); }
    QTextDocument *document() const { return m_textEdit->document(); }

    void setFontFamily(const QString &family) { m_textEdit->setFontFamily(family); }

    void setFontSize(const QString &size) { m_textEdit->setFontPointSize(size.toInt()); }

    void setFontColor(const QString &color) {
        m_textEdit->setTextColor(QColor(color));
        updateColorSwatch();
    }

signals:
    void textChanged();

    void fontFamilyChanged(const QString &fontFamily) const;
    void fontSizeChanged(const QString &fontSize) const;
    void fontColorChanged(const QString &fontColor) const;

public slots:
    void setHtml(const QString &html) { m_textEdit->setHtml(html); }

private slots:
    void setBold(bool on) { m_textEdit->setFontWeight(on ? QFont::Bold : QFont::Normal); }

    void setColor();

    void updateColor(const QColor &color);

    void setFontPointSize(double points) {
        m_textEdit->setFontPointSize(static_cast<qreal>(points));
        emit fontSizeChanged(QString::number(points));
    }

    void setFontFamily(const QFont &font) {
        m_textEdit->setFontFamily(font.family());
        emit fontFamilyChanged(font.family());
    }

    void currentCharFormatChanged(const QTextCharFormat &format);

private:
    void createWidgets();
    void createLayout();
    void createConnections();
    void updateColorSwatch();

    QToolBar *m_fontToolBar;

    QAction *m_boldAction;
    QAction *m_italicAction;
    QAction *m_underlineAction;
    QAction *m_colorAction;

    QColorDialog *m_colorDialog;

    QFontComboBox *m_fontFamilyCombo;

    QDoubleSpinBox *m_fontSizeSpin;

    QTextEdit *m_textEdit;
};

#endif // TEXTEDITOR_H
