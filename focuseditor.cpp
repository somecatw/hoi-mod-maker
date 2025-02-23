#include "focuseditor.h"
#include "ui_focuseditor.h"

FocusEditor::FocusEditor(const Focus &f,QWidget *parent)
    : QMainWindow(parent)
    , focusui(new Ui::FocusEditor)
    , storedFocus(f)
{
    focusui->setupUi(this);
    focusui->widget->protector=defaultFocusProtector;
    focusui->widget->init(f.toLangObj());
    setCentralWidget(focusui->widget);
}

FocusEditor::~FocusEditor()
{
    delete focusui;
}

void FocusEditor::on_action_triggered()
{
    focusui->widget->undo();
}


void FocusEditor::on_action_2_triggered()
{
    focusui->widget->redo();
}

