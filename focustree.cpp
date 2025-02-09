#include "focustree.h"
#include "ui_focustree.h"
#include "focus.h"

focustree::focustree(QWidget *parent)
    : QMainWindow(parent)
    , focustreeui(new Ui::focustree)
{
    focustreeui->setupUi(this);
}

focustree::~focustree()
{
    delete focustreeui;
}

void focustree::on_focusa_clicked()
{
    focus *a = new focus();
    a->show();
}

