#ifndef FOCUSTREE_H
#define FOCUSTREE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsView>
#include <QWheelEvent>
#include "focusmodel.h"


namespace Ui {
class focustree;
}

// 造一个容器用来放 FocusItem, 方便实现拖动、缩放国策树
class FocusTreeView : public QGraphicsView
{
public:
    FocusTreeView(QGraphicsScene *scene, QWidget *parent=nullptr);

protected:
    void wheelEvent(QWheelEvent *evt) override;
};

class focustree : public QMainWindow
{
    Q_OBJECT

public:
    explicit focustree(QWidget *parent = nullptr);
    ~focustree();

private slots:
    void on_focusa_clicked();

    void on_actionopen_triggered();

private:
    Ui::focustree *focustreeui;
    QGraphicsScene *treeScene;
    FocusTreeView *treeView;
    FocusModel *focusModel;
};

#endif // FOCUSTREE_H
