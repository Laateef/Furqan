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

#ifndef MAPENUMERATION_H
#define MAPENUMERATION_H

namespace NS_MAP {
enum MAP{
    GENDER,
    LEVEL,
    SEMESTER,
    COMPETITION,
    RECITAL
};
}

namespace NS_GENDER {
enum {
    NONE,
    MALE,
    FEMALE
};
}

namespace NS_LEVEL {
enum {
    NONE,
    BASIC,
    INTERMEDIATE,
    ADVANCED
};
}

namespace NS_SEMESTER {
enum {
    NONE,
    FIRST,
    SECOND
};
}

namespace NS_COMPETITION_TYPE {
enum {
    NONE,
    DAILY,
    WEEKLY,
    QUARTERLY
};
}

namespace NS_RECITAL {
enum {
    NONE,
    READING,
    MEMORIZING,
    REVIEWING
};
}

#endif // MAPENUMERATION_H
