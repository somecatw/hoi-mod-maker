#ifndef FOCUSTREEVIEW_H
#define FOCUSTREEVIEW_H

#include <QGraphicsView>
#include "focusitem.h"

class focustree;

class MultipleFocusSelection : public QObject{
    Q_OBJECT
public:
    explicit MultipleFocusSelection(QObject *parent=nullptr);
    void addItem(FocusItem *item);
    void removeItem(FocusItem *item);
    void clear();
    int limitY(int targety);
    size_t size() const;
    bool contains(FocusItem *item)const;
    void move(focustree *tree,int dx,int dy);
    const QSet<FocusItem*> &itemSet()const;
private:
    QSet<FocusItem*> items;
    int uLimit,dLimit;
    QSet<FocusItem*> exclItems;
    void updateLimits();
};

// 造一个容器用来放 FocusItem, 方便实现拖动、缩放国策树
class FocusTreeView : public QGraphicsView
{
    Q_OBJECT
public:
    FocusTreeView(focustree *_tree,QGraphicsScene *scene, QWidget *parent=nullptr);

public slots:
    void select(FocusItem *item);
    void deSelect(FocusItem *item);
protected:
    void wheelEvent(QWheelEvent *evt) override;
    void contextMenuEvent(QContextMenuEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;
    void mouseDoubleClickEvent(QMouseEvent *evt) override;
private:
    QMenu *menu;
    MultipleFocusSelection *selection;
    FocusItem *menuTargetItem;
    focustree *tree;
    bool dragging;
    FocusItem *moveReferenceItem;

    FocusItem *getFocusAtGlobalPos(const QPoint &p)const;
    void clearSelection();

signals:
    void cleared();
    void frameResetNeeded();
public slots:
    void hideFocus();
    void selectSubtree();
    void setPrereq();
    void removePrereq();
};

#endif // FOCUSTREEVIEW_H
