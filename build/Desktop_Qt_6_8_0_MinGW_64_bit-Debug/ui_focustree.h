/********************************************************************************
** Form generated from reading UI file 'focustree.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOCUSTREE_H
#define UI_FOCUSTREE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_focustree
{
public:
    QAction *actionnew;
    QAction *actionopen;
    QAction *actionlocate;
    QAction *actionlocated;
    QAction *actiondaochu;
    QAction *actionA;
    QAction *actionB;
    QAction *actionC;
    QAction *actionD;
    QWidget *centralwidget;
    QPushButton *focusa;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menu_5;
    QMenu *menu_6;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *focustree)
    {
        if (focustree->objectName().isEmpty())
            focustree->setObjectName("focustree");
        focustree->resize(1042, 674);
        actionnew = new QAction(focustree);
        actionnew->setObjectName("actionnew");
        actionopen = new QAction(focustree);
        actionopen->setObjectName("actionopen");
        actionlocate = new QAction(focustree);
        actionlocate->setObjectName("actionlocate");
        actionlocated = new QAction(focustree);
        actionlocated->setObjectName("actionlocated");
        actiondaochu = new QAction(focustree);
        actiondaochu->setObjectName("actiondaochu");
        actionA = new QAction(focustree);
        actionA->setObjectName("actionA");
        actionB = new QAction(focustree);
        actionB->setObjectName("actionB");
        actionC = new QAction(focustree);
        actionC->setObjectName("actionC");
        actionD = new QAction(focustree);
        actionD->setObjectName("actionD");
        centralwidget = new QWidget(focustree);
        centralwidget->setObjectName("centralwidget");
        focusa = new QPushButton(centralwidget);
        focusa->setObjectName("focusa");
        focusa->setGeometry(QRect(420, 110, 151, 121));
        QFont font;
        font.setPointSize(20);
        focusa->setFont(font);
        focustree->setCentralWidget(centralwidget);
        menubar = new QMenuBar(focustree);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1042, 24));
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
        focustree->setMenuBar(menubar);
        statusbar = new QStatusBar(focustree);
        statusbar->setObjectName("statusbar");
        focustree->setStatusBar(statusbar);

        menubar->addAction(menu_6->menuAction());
        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_4->menuAction());
        menubar->addAction(menu_5->menuAction());
        menu->addAction(actionnew);
        menu->addAction(actionopen);
        menu->addAction(actionlocated);
        menu->addAction(actiondaochu);
        menu_6->addAction(actionA);
        menu_6->addAction(actionB);
        menu_6->addAction(actionC);
        menu_6->addAction(actionD);

        retranslateUi(focustree);

        QMetaObject::connectSlotsByName(focustree);
    } // setupUi

    void retranslateUi(QMainWindow *focustree)
    {
        focustree->setWindowTitle(QCoreApplication::translate("focustree", "MainWindow", nullptr));
        actionnew->setText(QCoreApplication::translate("focustree", "\346\226\260\345\273\272", nullptr));
        actionopen->setText(QCoreApplication::translate("focustree", "\345\257\274\345\205\245", nullptr));
        actionlocate->setText(QCoreApplication::translate("focustree", "\346\234\254\345\234\260\345\214\226", nullptr));
        actionlocated->setText(QCoreApplication::translate("focustree", "\346\234\254\345\234\260\345\214\226", nullptr));
        actiondaochu->setText(QCoreApplication::translate("focustree", "\345\257\274\345\207\272", nullptr));
        actionA->setText(QCoreApplication::translate("focustree", "\345\220\257\347\224\250\346\235\241\344\273\266", nullptr));
        actionB->setText(QCoreApplication::translate("focustree", "\346\230\257\345\220\246\344\270\272\346\210\220\345\221\230\345\233\275\347\255\226\346\240\221", nullptr));
        actionC->setText(QCoreApplication::translate("focustree", "\346\230\257\345\220\246\344\270\272\351\200\232\347\224\250\345\233\275\347\255\226\346\240\221", nullptr));
        actionD->setText(QCoreApplication::translate("focustree", "\345\233\275\347\255\226\346\225\260\351\207\217\347\273\237\350\256\241", nullptr));
        focusa->setText(QCoreApplication::translate("focustree", "\345\233\275\347\255\226A", nullptr));
        menu->setTitle(QCoreApplication::translate("focustree", "\346\226\207\344\273\266", nullptr));
        menu_2->setTitle(QCoreApplication::translate("focustree", "\344\272\222\346\226\245\347\272\277", nullptr));
        menu_3->setTitle(QCoreApplication::translate("focustree", "\345\211\215\351\251\261", nullptr));
        menu_4->setTitle(QCoreApplication::translate("focustree", "\345\244\232\351\200\211\345\211\215\351\251\261", nullptr));
        menu_5->setTitle(QCoreApplication::translate("focustree", "\346\267\273\345\212\240\351\242\204\350\256\276\345\233\275\347\255\226", nullptr));
        menu_6->setTitle(QCoreApplication::translate("focustree", "\345\233\275\347\255\226\346\240\221\345\261\236\346\200\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class focustree: public Ui_focustree {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOCUSTREE_H
