QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    focuseditor.cpp \
    focusitem.cpp \
    focuslistview.cpp \
    focusmodel.cpp \
    focustree.cpp \
    focustreeview.cpp \
    idea_editer.cpp \
    lineitems.cpp \
    main.cpp \
    mainwindow.cpp \
    parser.cpp \
    prereqgroupselector.cpp \
    prereqgroupwidget.cpp \
    preset_ideas.cpp \
    preset_parser.cpp \
    undomanager.cpp

HEADERS += \
    focuseditor.h \
    focusitem.h \
    focuslistview.h \
    focusmodel.h \
    focustree.h \
    focustreeview.h \
    idea_editer.h \
    lineitems.h \
    mainwindow.h \
    parser.h \
    prereqgroupselector.h \
    prereqgroupwidget.h \
    preset_ideas.h \
    preset_parser.h \
    undomanager.h

FORMS += \
    focuseditor.ui \
    focusitem.ui \
    focuslistview.ui \
    focustree.ui \
    idea_editer.ui \
    mainwindow.ui \
    prereqgroupselector.ui \
    prereqgroupwidget.ui \
    preset_ideas.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
