QT -= gui
QT += widgets network

TEMPLATE = lib
DEFINES += ETLCOMMODULE_LIBRARY
LIBS += -lws2_32

CONFIG += c++11
CONFIG += dll

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEPENDPATH += . ../IComModule/pub/ ../IComModule/inc/ ../IComModule/src/
INCLUDEPATH += ../IComModule/pub/ ../IComModule/inc/

SOURCES += \
    src/IComModule.cpp

HEADERS += \
    inc/ETLComModule_global.h \
    pub/IComModule.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/testcase.ini
