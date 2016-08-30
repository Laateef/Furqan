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

#ifndef TRANSLATORHANDLER_H
#define TRANSLATORHANDLER_H

#include <QObject>
#include <QTranslator>

#include "generalenumeration.h"

class TranslatorHandler : public QObject
{
    Q_OBJECT

public:
    static TranslatorHandler *singleton();

    void translate(NS_Language::Language);

signals:
    void languageChanged() const;

private:
    explicit TranslatorHandler(QObject *parent = 0);

    QTranslator *m_translator;
};

#endif // TRANSLATORHANDLER_H
