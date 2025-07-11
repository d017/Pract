QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithmselectionscreen.cpp \
    andrew.cpp \
    bank.cpp \
    bidTable.cpp \
    gameEnd.cpp \
    infotable.cpp \
    initial.cpp \
    main.cpp \
    mainwindow.cpp \
    moveselectionscreen.cpp \
    playerCount.cpp \
    playermanager.cpp \
    polina.cpp \
    prokhor.cpp \
    rulesscreen.cpp \
    vitaliy.cpp

HEADERS += \
    algorithmselectionscreen.h \
    bank.h \
    bidTable.h \
    gameEnd.h \
    infotable.h \
    initial.h \
    mainwindow.h \
    moveselectionscreen.h \
    playerCount.h \
    playeralgorithm.h \
    playermanager.h \
    polina.h \
    rulesscreen.h \
    structures.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
