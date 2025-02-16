#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "focustree.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"114514\n";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    focustree *a = new focustree();
    a->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    preset_add_point *a =new preset_add_point();
    a->show();
}

