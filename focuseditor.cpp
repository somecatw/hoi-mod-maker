#include "focuseditor.h"
#include "ui_focuseditor.h"

FocusEditor::FocusEditor(const Focus &f,QWidget *parent)
    : QMainWindow(parent)
    , focusui(new Ui::FocusEditor)
    , storedFocus(f)
{
    focusui->setupUi(this);
    focusui->widget->init(f.src);
    setCentralWidget(focusui->widget);
    focusui->widget->protector=defaultFocusProtector;
}

FocusEditor::~FocusEditor()
{
    delete focusui;
}
