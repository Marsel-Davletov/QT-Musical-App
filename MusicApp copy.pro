QT       += core gui multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    changeiconbutton.cpp \
    clickedchangeiconbutton.cpp \
    hoverchangeiconbutton.cpp \
    iconlabel.cpp \
    main.cpp \
    mainwindow.cpp \
    musicbutton.cpp \
    musiclist.cpp \
    musicplayer.cpp \
    navigationbutton.cpp \
    navigationtabwidget.cpp \
    searchmusicwindow.cpp \
    verticaltabbar.cpp

HEADERS += \
    changeiconbutton.h \
    clickedchangeiconbutton.h \
    hoverchangeiconbutton.h \
    iconlabel.h \
    includes.h \
    mainwindow.h \
    musicbutton.h \
    musiclist.h \
    musicplayer.h \
    navigationbutton.h \
    navigationtabwidget.h \
    searchmusicwindow.h \
    verticaltabbar.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    icons/three_pont.png
