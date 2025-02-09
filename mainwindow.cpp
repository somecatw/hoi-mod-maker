#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "focustree.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

