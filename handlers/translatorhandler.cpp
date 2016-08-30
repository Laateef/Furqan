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

#include "translatorhandler.h"

#include "resourcemanager.h"

#include <QApplication>

TranslatorHandler::TranslatorHandler(QObject *parent) :
    QObject(parent),
    m_translator(new QTranslator(this))
{
    qApp->installTranslator(m_translator);
}

TranslatorHandler *TranslatorHandler::singleton()
{
    static TranslatorHandler *translatorHandler = NULL;

    if (!translatorHandler)
        translatorHandler = new TranslatorHandler(qApp);

    return translatorHandler;
}

void TranslatorHandler::translate(NS_Language::Language lang)
{
    switch (lang) {
    case NS_Language::Arabic:
        m_translator->load("ar");
        break;
    case NS_Language::English:
        m_translator->load("en");
        break;
    default:
        break;
    }

    emit languageChanged();
}
