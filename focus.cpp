#include "focus.h"
#include "ui_focus.h"

focus::focus(QWidget *parent)
    : QMainWindow(parent)
    , focusui(new Ui::focus)
{
    focusui->setupUi(this);
}

focus::~focus()
{
    delete focusui;
}
