/********************************************************************************
** Form generated from reading UI file 'focus.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOCUS_H
#define UI_FOCUS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_focus
{
public:
    QAction *actionA;
    QAction *actionB;
    QAction *actionC;
    QAction *actionA_2;
    QAction *actionB_2;
    QAction *actionC_2;
    QWidget *centralwidget;
    QTextBrowser *textBrowser;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menu_5;
    QMenu *menu_6;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *focus)
    {
        if (focus->objectName().isEmpty())
            focus->setObjectName("focus");
        focus->resize(1058, 690);
        actionA = new QAction(focus);
        actionA->setObjectName("actionA");
        actionB = new QAction(focus);
        actionB->setObjectName("actionB");
        actionC = new QAction(focus);
        actionC->setObjectName("actionC");
        actionA_2 = new QAction(focus);
        actionA_2->setObjectName("actionA_2");
        actionB_2 = new QAction(focus);
        actionB_2->setObjectName("actionB_2");
        actionC_2 = new QAction(focus);
        actionC_2->setObjectName("actionC_2");
        centralwidget = new QWidget(focus);
        centralwidget->setObjectName("centralwidget");
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(80, 0, 961, 651));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(0, 0, 86, 26));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(0, 30, 86, 26));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(0, 60, 321, 71));
        focus->setCentralWidget(centralwidget);
        menubar = new QMenuBar(focus);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1058, 24));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName("menu_4");
        menu_5 = new QMenu(menubar);
        menu_5->setObjectName("menu_5");
        menu_6 = new QMenu(menubar);
        menu_6->setObjectName("menu_6");
        focus->setMenuBar(menubar);
        statusbar = new QStatusBar(focus);
        statusbar->setObjectName("statusbar");
        focus->setStatusBar(statusbar);

        menubar->addAction(menu_4->menuAction());
        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_5->menuAction());
        menubar->addAction(menu_6->menuAction());
        menu_6->addAction(actionA_2);
        menu_6->addAction(actionB_2);
        menu_6->addAction(actionC_2);

        retranslateUi(focus);

        QMetaObject::connectSlotsByName(focus);
    } // setupUi

    void retranslateUi(QMainWindow *focus)
    {
        focus->setWindowTitle(QCoreApplication::translate("focus", "MainWindow", nullptr));
        actionA->setText(QCoreApplication::translate("focus", "A", nullptr));
        actionB->setText(QCoreApplication::translate("focus", "B", nullptr));
        actionC->setText(QCoreApplication::translate("focus", "C", nullptr));
        actionA_2->setText(QCoreApplication::translate("focus", "A", nullptr));
        actionB_2->setText(QCoreApplication::translate("focus", "B", nullptr));
        actionC_2->setText(QCoreApplication::translate("focus", "C", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("focus", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt;\">\344\270\200\344\270\252\345\217\257\344\272\244\344\272\222\347\232\204\346\226\207\346\234\254\346\241\206\357\274\214\346\240\271\346\215\256\345\267\245\345\205\267\346\240\217\347\232\204\351\200\211\351\241\271\346\230\276\347\244\272\345\257\271\345\272\224\345\206\205\345\256\271\346\235\245\345\256\236\347\216\260\347\274\226\350\276\221</span></p></"
                        "body></html>", nullptr));
        pushButton->setText(QCoreApplication::translate("focus", "\345\233\275\347\255\226id", nullptr));
        pushButton_2->setText(QCoreApplication::translate("focus", "\345\233\275\347\255\226\345\233\276\346\240\207", nullptr));
        pushButton_3->setText(QCoreApplication::translate("focus", "\344\270\200\344\272\233\345\217\257\344\273\245\345\277\253\346\215\267\347\274\226\350\276\221\347\232\204\346\210\220\345\210\206\351\203\275\344\274\232\346\224\276\345\234\250\345\267\246\344\276\247\345\277\253\351\200\237\347\274\226\350\276\221\357\274\210\357\274\211", nullptr));
        menu->setTitle(QCoreApplication::translate("focus", "\346\233\264\346\224\271\345\233\275\347\255\226\345\233\276\346\240\207", nullptr));
        menu_2->setTitle(QCoreApplication::translate("focus", "\346\233\264\346\224\271\345\233\275\347\255\226\346\235\241\344\273\266", nullptr));
        menu_3->setTitle(QCoreApplication::translate("focus", "\346\233\264\346\224\271\345\233\275\347\255\226\345\256\214\346\210\220\346\225\210\346\236\234", nullptr));
        menu_4->setTitle(QCoreApplication::translate("focus", "\346\211\213\345\212\250\347\274\226\350\276\221\346\226\207\346\234\254", nullptr));
        menu_5->setTitle(QCoreApplication::translate("focus", "\350\207\252\345\256\232\344\271\211\345\233\275\347\255\226\345\206\205\345\256\271\346\217\217\350\277\260", nullptr));
        menu_6->setTitle(QCoreApplication::translate("focus", "\346\267\273\345\212\240\345\233\275\347\255\226\346\225\210\346\236\234\351\242\204\350\256\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class focus: public Ui_focus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOCUS_H
