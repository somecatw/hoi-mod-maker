#include "focuseditor.h"
#include "ui_focuseditor.h"

FocusEditor::FocusEditor(QWidget *parent)
    : QMainWindow(parent)
    , focusui(new Ui::FocusEditor)
{
    focusui->setupUi(this);
}

FocusEditor::~FocusEditor()
{
    delete focusui;
}
