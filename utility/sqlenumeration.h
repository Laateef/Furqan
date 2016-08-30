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

#ifndef SQLENUMERATION_H
#define SQLENUMERATION_H

namespace NS_TABLE {
enum TABLE {
    CLASS,
    COURSE,
    STUDENT,
    TEACHER,
    TEACHING,
    RESULT,
    QURAN,
    COMPETITION,
    COMPETITOR,
    JOB,
    EMPLOYEE
};
}

namespace NS_CLASS {
enum {
    ID,
    NAME,
    LEVEL
};
}

namespace NS_COURSE {
enum {
    ID,
    NAME,
    SEMESTER
};
}

namespace NS_STUDENT {
enum {
    ID,
    FULL_NAME,
    FATHER_NAME,
    GENDER,
    BIRTH_PLACE,
    BIRTH_DATE,
    RESIDENCY,
    NATIONALITY,
    NATIONAL_ID,
    MOBILE,
    FATHER_JOB,
    LEVEL,
    CLASS,
};
}

namespace NS_QURAN {
enum {
    ID,
    STUDENT,
    RECITAL_TYPE,
    FROM_PAGE,
    TO_PAGE,
    DATE
};
}

namespace NS_TEACHER {
enum {
    ID,
    FULL_NAME,
    FATHER_NAME,
    GENDER,
    BIRTH_PLACE,
    BIRTH_DATE,
    RESIDENCY,
    NATIONALITY,
    NATIONAL_ID,
    MOBILE,
    QUALIFICATION,
    EXPERIENCE
};
}

namespace NS_JOB {
enum {
    ID,
    NAME
};
}

namespace NS_EMPLOYEE {
enum {
    ID,
    FULL_NAME,
    FATHER_NAME,
    GENDER,
    BIRTH_PLACE,
    BIRTH_DATE,
    RESIDENCY,
    NATIONALITY,
    NATIONAL_ID,
    MOBILE,
    JOB,
    OTHER_JOB
};
}

namespace NS_TEACHING {
enum {
    ID,
    TEACHER,
    CLASS,
    COURSE
};
}

namespace NS_RESULT {
enum {
    ID,
    COURSE,
    STUDENT,
    RESULT
};
}

namespace NS_COMPETITION {
enum {
    ID,
    NAME,
    TYPE,
    PARTS,
    DATE
};
}

namespace NS_COMPETITOR {
enum {
    ID,
    COMPETITION,
    STUDENT,
    RANK
};
}

#endif // SQLENUMERATION_H
