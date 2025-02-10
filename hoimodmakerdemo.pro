QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    focuseditor.cpp \
    focusitem.cpp \
    focustree.cpp \
    main.cpp \
    mainwindow.cpp \
    parser.cpp \
    preset_add_point.cpp

HEADERS += \
    focuseditor.h \
    focusitem.h \
    focustree.h \
    mainwindow.h \
    parser.h \
    preset.h \
    preset_add_point.h \
    presetcpp

FORMS += \
    focuseditor.ui \
    focusitem.ui \
    focustree.ui \
    mainwindow.ui \
    preset_add_point.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
