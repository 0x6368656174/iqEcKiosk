# Add more folders to ship with the application, here
folder_01.source = qml/eckiosk
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

QT += network

include (iqorm/iqorm.pri)

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    cardinfo/iqloyaltycard.cpp \
    cardinfo/iqcardaccountoperation.cpp \
    cardinfo/iqgameobject.cpp \
    cardinfo/iqcardaccountoperationtype.cpp \
    cardinfo/iqcardaccountoperations.cpp \
    cardinfo/iqloyaltycards.cpp \
    iqcardcontroller.cpp \
    cardinfo/iqvcardoperation.cpp \
    cardinfo/iqvcardoperations.cpp \
    iqsortfilterproxymodel.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    cardinfo/iqloyaltycard.h \
    cardinfo/iqcardinfo.h \
    cardinfo/iqcardaccountoperation.h \
    cardinfo/iqgameobject.h \
    cardinfo/iqcardaccountoperationtype.h \
    cardinfo/iqcardaccountoperations.h \
    cardinfo/iqloyaltycards.h \
    iqcardcontroller.h \
    cardinfo/iqvcardoperation.h \
    cardinfo/iqvcardoperations.h \
    iqsortfilterproxymodel.h

RESOURCES += \
    icons.qrc

RC_FILE = iqtemp.rc
