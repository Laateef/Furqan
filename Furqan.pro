QT = core gui widgets sql printsupport

TEMPLATE = app

TARGET = Furqan

VERSION = 1.0.0

CONFIG += release

RESOURCES += \
    resource.qrc

RC_ICONS = furqan.ico

TRANSLATIONS += translations\en.ts translations\ar.ts

INCLUDEPATH += widgets structures relations delegates handlers utility

FORMS += \
    interfaces\classwidget.ui \
    interfaces\competitionwidget.ui \
    interfaces\competitorwidget.ui \
    interfaces\coursewidget.ui \
    interfaces\mainwidget.ui \
    interfaces\employeewidget.ui \
    interfaces\jobwidget.ui \
    interfaces\quranwidget.ui \
    interfaces\resultwidget.ui \
    interfaces\settingwidget.ui \
    interfaces\studentwidget.ui \
    interfaces\teacherwidget.ui \
    interfaces\teachingwidget.ui

HEADERS += \
    delegates/datedelegate.h \
    delegates/mapdelegate.h \
    delegates/numericdelegate.h \
    delegates/relationaldelegate.h \
    delegates/sqldelegate.h \
    delegates/uniquedelegate.h \
    handlers/databasehandler.h \
    handlers/maphandler.h \
    handlers/modelhandler.h \
    handlers/printerhandler.h \
    handlers/settinghandler.h \
    handlers/translatorhandler.h \
    relations/abstractrelation.h \
    relations/maprelation.h \
    relations/sqlrelation.h \
    structures/printOption.h \
    structures/richsqlmodel.h \
    utility/enumeration.h \
    utility/generalenumeration.h \
    utility/mapenumeration.h \
    utility/resourcemanager.h \
    utility/settingenumeration.h \
    utility/sqlenumeration.h \
    widgets/classwidget.h \
    widgets/competitionwidget.h \
    widgets/competitorwidget.h \
    widgets/coursewidget.h \
    widgets/mainwidget.h \
    widgets/employeewidget.h \
    widgets/jobwidget.h \
    widgets/quranwidget.h \
    widgets/resultwidget.h \
    widgets/settingwidget.h \
    widgets/studentwidget.h \
    widgets/teacherwidget.h \
    widgets/teachingwidget.h \
    widgets/printdialog.h \
    widgets/texteditor.h

SOURCES += \
    delegates/datedelegate.cpp \
    delegates/mapdelegate.cpp \
    delegates/numericdelegate.cpp \
    delegates/relationaldelegate.cpp \
    delegates/sqldelegate.cpp \
    delegates/uniquedelegate.cpp \
    handlers/databasehandler.cpp \
    handlers/maphandler.cpp \
    handlers/modelhandler.cpp \
    handlers/printerhandler.cpp \
    handlers/settinghandler.cpp \
    handlers/translatorhandler.cpp \
    relations/abstractrelation.cpp \
    relations/maprelation.cpp \
    relations/sqlrelation.cpp \
    structures/printoption.cpp \
    structures/richsqlmodel.cpp \
    utility/resourcemanager.cpp \
    widgets/classwidget.cpp \
    widgets/competitionwidget.cpp \
    widgets/competitorwidget.cpp \
    widgets/coursewidget.cpp \
    widgets/mainwidget.cpp \
    widgets/employeewidget.cpp \
    widgets/jobwidget.cpp \
    widgets/quranwidget.cpp \
    widgets/resultwidget.cpp \
    widgets/settingwidget.cpp \
    widgets/studentwidget.cpp \
    widgets/teacherwidget.cpp \
    widgets/teachingwidget.cpp \
    widgets/printdialog.cpp \
    widgets/texteditor.cpp \
    main.cpp
