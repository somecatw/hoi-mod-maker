#ifndef FOCUSTREE_H
#define FOCUSTREE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsProxyWidget>
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
    static constexpr double wgap = 80;
    static constexpr double hgap = 100;

    static inline const double itemW = 80;
    static inline const double itemH = 80;

    /* 同种颜色的前置国策，只需完成任意一个，详见 focusmodel.h
     * 不过好像换个跟红色（互斥国策）差别大一点的颜色比较好？有没有色彩老师指导一下
     */
    static inline QColor colorList[10]={0xFF9900,0xFFFF00};

    const FocusModel &model();
    void setPreqFrames(const QString& str);

signals:
    void resetSelection();


private slots:
    void on_focusa_clicked();

    void on_actionopen_triggered();

private:
    Ui::focustree *focustreeui;
    QGraphicsScene *treeScene;
    FocusTreeView *treeView;
    FocusModel *focusModel;
    QMap<QString,QGraphicsProxyWidget*> proxies;
    QMap<QString,bool> exLineDeployed;

    // 如果 id 已经存在，就直接 return
    void addFocusItem(const Focus& f);

    void addFocusPreqLine(const Focus& f);
    void addFocusExLine(const Focus& f);
    QGraphicsProxyWidget* getProxy(const QString& id) const;

};

#endif // FOCUSTREE_H
