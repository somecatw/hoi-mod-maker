#ifndef FOCUSTREE_H
#define FOCUSTREE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsProxyWidget>
#include <QSplitter>
#include "focusmodel.h"
#include "focusitem.h"
#include "focuslistview.h"
#include "undomanager.h"

namespace Ui {
class focustree;
}

// 造一个容器用来放 FocusItem, 方便实现拖动、缩放国策树
class FocusTreeView : public QGraphicsView
{
    Q_OBJECT
public:
    FocusTreeView(focustree *_tree,QGraphicsScene *scene, QWidget *parent=nullptr);

protected:
    void wheelEvent(QWheelEvent *evt) override;
    void contextMenuEvent(QContextMenuEvent *evt) override;
private:
    QMenu *menu;
    FocusItem *selectedItem;
    focustree *tree;
public slots:
    void hideFocus();
};

class focustree : public QMainWindow
{
    Q_OBJECT

public:
    explicit focustree(QWidget *parent = nullptr);
    ~focustree();
    static constexpr double wgap = 80;
    static constexpr double hgap = 100;

    static inline const double itemW = 80;
    static inline const double itemH = 80;

    /* 同种颜色的前置国策，只需完成任意一个，详见 focusmodel.h
     * 不过好像换个跟红色（互斥国策）差别大一点的颜色比较好？有没有色彩老师指导一下
     */
    static inline QColor colorList[10]={0xFF9900,0xFFFF00};
    UndoManager *uManager;

    const FocusModel &model();
    void setPreqFrames(const QString& str);
    bool noPreqHidden(const QString &str);

protected:
    void resizeEvent(QResizeEvent *evt) override;

signals:
    void resetSelection();
    void focusHidden(const QString &id);
    void focusShown(const QString &id);

public slots:
    void showFocus(const QString &id);

private slots:
    void on_focusa_clicked();
    void updateExclusiveFocus(const QString &name);
    void on_actionopen_triggered();
    void revealFocus(const QString &id);

    void on_action_redo_triggered();
    void on_action_undo_triggered();

private:
    Ui::focustree *focustreeui;
    QSplitter *splitter;
    FocusListView *listView;
    QGraphicsScene *treeScene;
    FocusTreeView *treeView;
    FocusModel *focusModel;
    QMap<QString,QGraphicsProxyWidget*> proxies;
    QMap<QString,QPoint> displayPos;
    QMap<std::pair<int,int>,QVector<FocusItem*>> focusGrid;
    QMap<QPair<FocusItem*,FocusItem*>,QGraphicsProxyWidget*> exclLines;

    // 如果 id 已经存在，就直接 return
    void addFocusItem(const Focus& f);

    void addFocusPreqLine(const Focus& f);
    void addFocusExLine(const Focus& f);
    void removeFocusExLine(const Focus &f);
    QGraphicsProxyWidget* getProxy(const QString& id) const;

    bool xQuery(int x1,int x2,int y,std::function<bool(FocusItem*)> f)const;
    bool yQuery(int y1,int y2,int x,std::function<bool(FocusItem*)> f)const;
    QGraphicsProxyWidget *getExclLine(FocusItem *a,FocusItem *b)const;
};

#endif // FOCUSTREE_H
