#ifndef FOCUS_H
#define FOCUS_H

#include <QMainWindow>

namespace Ui {
class focus;
}

class focus : public QMainWindow
{
    Q_OBJECT

public:
    explicit focus(QWidget *parent = nullptr);
    ~focus();

private:
    Ui::focus *focusui;
};

#endif // FOCUS_H
