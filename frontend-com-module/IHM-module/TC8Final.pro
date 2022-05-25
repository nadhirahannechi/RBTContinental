QT       += core gui xml quick network charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
LIBS += -lws2_32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

##################Development Variables#############################
DEPENDPATH += ./inc/ ../com-module/IComModule/pub
INCLUDEPATH += ./inc/ ../com-module/IComModule/pub
LIBS += -L..\com-module\build-IComModule-Desktop_Qt_6_1_2_MinGW_64_bit-Debug\debug -lIComModule

##################Deployment Variables#############################
#DEPENDPATH += ./inc/ ../com-module/IComModule/pub/
#INCLUDEPATH += ./inc/ ../com-module/IComModule/pub/
#LIBS += -L..\IComModule_build\release\ -lIComModule


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/reporting.cpp \
    src/testdata.cpp \
    src/testdatacreator.cpp

HEADERS += \
    inc/Structure_type.h \
    inc/mainwindow.h \
    inc/reporting.h \
    inc/testdata.h \
    inc/testdatacreator.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
