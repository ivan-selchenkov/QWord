# #####################################################################
# Automatically generated by qmake (2.01a) Sun May 3 10:50:28 2009
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += /home/share/qt/QWord/. \
    .
QT += sql

# Input
HEADERS += adddialog.h \
    mainwindow.h \
    showdialog.h \
    inquiryinitial.h
FORMS += adddialog.ui \
    mainwindow.ui \
    showdialog.ui \
    inquiryinitial.ui
SOURCES += adddialog.cpp \
    main.cpp \
    mainwindow.cpp \
    showdialog.cpp \
    inquiryinitial.cpp
RESOURCES += qword.qrc
TRANSLATIONS += main_ru.ts

desktop.path = /usr/share/applications
desktop.files = applications/qword.desktop

svg.path = /usr/share/pixmaps
svg.files = images/qword.svg

target.path = /usr/bin
INSTALLS += target desktop svg
