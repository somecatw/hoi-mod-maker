#ifndef IDEA_EDITER_H
#define IDEA_EDITER_H

#include <QMainWindow>

namespace Ui {
class idea_editer;
}

class idea_editer : public QMainWindow
{
    Q_OBJECT

public:
    explicit idea_editer(QWidget *parent = nullptr);
    ~idea_editer();

private:
    Ui::idea_editer *ui;
};

#endif // IDEA_EDITER_H
