#ifndef FOCUSTREE_H
#define FOCUSTREE_H

#include <QMainWindow>

namespace Ui {
class focustree;
}

class focustree : public QMainWindow
{
    Q_OBJECT

public:
    explicit focustree(QWidget *parent = nullptr);
    ~focustree();

private slots:
    void on_focusa_clicked();

private:
    Ui::focustree *focustreeui;
};

#endif // FOCUSTREE_H
