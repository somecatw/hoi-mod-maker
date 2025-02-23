#ifndef FOCUSEDITOR_H
#define FOCUSEDITOR_H

#include <QMainWindow>
#include "focusmodel.h"

namespace Ui {
class FocusEditor;
}

class FocusEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit FocusEditor(const Focus &f,QWidget *parent = nullptr);
    ~FocusEditor();

private slots:
    void on_action_triggered();

    void on_action_2_triggered();

private:
    Ui::FocusEditor *focusui;
    Focus storedFocus;
};

#endif // FOCUSEDITOR_H
