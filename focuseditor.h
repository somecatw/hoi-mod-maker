#ifndef FOCUSEDITOR_H
#define FOCUSEDITOR_H

#include <QMainWindow>

namespace Ui {
class FocusEditor;
}

class FocusEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit FocusEditor(QWidget *parent = nullptr);
    ~FocusEditor();

private:
    Ui::FocusEditor *focusui;
};

#endif // FOCUSEDITOR_H
