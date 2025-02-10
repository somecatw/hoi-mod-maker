#include "focustree.h"
#include "ui_focustree.h"
#include "focuseditor.h"

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
    FocusEditor *a = new FocusEditor();
    a->show();
}

