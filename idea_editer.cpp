#include "idea_editer.h"
#include "ui_idea_editer.h"

idea_editer::idea_editer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::idea_editer)
{
    ui->setupUi(this);
}

idea_editer::~idea_editer()
{
    delete ui;
}
