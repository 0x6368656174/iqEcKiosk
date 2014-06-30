!contains(INCLUDED_PRI, iqorm){
    INCLUDED_PRI += iqorm
    INCLUDEPATH  += $$PWD
    QT           += core sql quick

    SOURCES += \
        $$PWD/iqormmodel.cpp \
        $$PWD/iqormpropertydescription.cpp \
        $$PWD/iqormobject.cpp \
        $$PWD/iqormcore.cpp \
        $$PWD/iqormabstractsynchronizer.cpp \
        $$PWD/iqormlocalsynchronizer.cpp \
        $$PWD/iqormnullsynchronizer.cpp \
        $$PWD/iqormabstractdatasource.cpp \
        $$PWD/iqormsqldatasource.cpp \
        $$PWD/iqormnulldatasource.cpp \
        $$PWD/iqormobjectsmodel.cpp \
        $$PWD/iqormfilter.cpp \
        $$PWD/iqormabstractfilter.cpp \
        $$PWD/iqormabstractobjectdatasource.cpp \
        $$PWD/iqormabstractobjectsmodeldatasource.cpp \
        $$PWD/iqormsqlobjectdatasource.cpp \
        $$PWD/iqormandgroupfilter.cpp \
        $$PWD/iqormorgroupfilter.cpp \
        $$PWD/iqormsqlobjectsmodeldatasource.cpp \
        $$PWD/iqormgroupfilter.cpp \
        $$PWD/iqormerror.cpp


    HEADERS += \
        $$PWD/iqormpropertydescription.h \
        $$PWD/iqormobject.h \
        $$PWD/iqormmodel.h \
        $$PWD/iqormcore.h \
        $$PWD/iqormabstractsynchronizer.h \
        $$PWD/iqormlocalsynchronizer.h \
        $$PWD/iqormnullsynchronizer.h \
        $$PWD/iqormabstractdatasource.h \
        $$PWD/iqormsqldatasource.h \
        $$PWD/iqormnulldatasource.h \
        $$PWD/iqormobjectsmodel.h \
        $$PWD/iqormfilter.h \
        $$PWD/iqormabstractfilter.h \
        $$PWD/iqormabstractobjectdatasource.h \
        $$PWD/iqormabstractobjectsmodeldatasource.h \
        $$PWD/iqormsqlobjectdatasource.h \
        $$PWD/iqormandgroupfilter.h \
        $$PWD/iqormorgroupfilter.h \
        $$PWD/iqormsqlobjectsmodeldatasource.h \
        $$PWD/iqormgroupfilter.h \
        $$PWD/iqormerror.h
}

HEADERS += \
    iqorm/iqormqmlobject.h \
    iqorm/iqormdirectpropertydescription.h \
    iqorm/iqormmanytomanyreferencepropertydescription.h

SOURCES += \
    iqorm/iqormqmlobject.cpp \
    iqorm/iqormdirectpropertydescription.cpp \
    iqorm/iqormmanytomanyreferencepropertydescription.cpp
