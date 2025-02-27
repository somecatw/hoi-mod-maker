#ifndef FOCUSITEM_H
#define FOCUSITEM_H

#include <QWidget>
#include <QPainter>
#include "lineitems.h"

namespace Ui {
class FocusItem;
}
class focustree;
/*
 * 应有一个 FocusModel 用来放所有国策的信息
 * parser 读取文件 -> 生成 FocusModel 对象 -> 绑定到 FocusTree
 * FocusTree 新建一系列的 FocusItem （是国策图标的 widget）
 * 点击 FocusItem 之后新建一个 FocusEditor 用来编辑国策内容
 */
class FocusItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit FocusItem(QObject *parent = nullptr);
    void setup(const QString &id,bool useLines,focustree *tr);
    void setFrame(const QColor &color);
    void hide();
    void setSelected();
    // 用来在右侧悬停时暂时显示国策
    void reveal();
    void unreveal();
    QRectF boundingRect()const override;

    int visiblePreqCount;
    QPoint displayPos;
    QVector<FocusItem*> exclItems,preqItems,postItems;
    QVector<BrokenLine*> preqLines;
    // 对应国策的 id
    QString focusid;
    bool hovering;

public slots:
    void hideFrame();
    void selectSubtree();
    void deSelect();
    void preqHidden();
    void preqShown();
    void show();
signals:
    void implicitlySelected(FocusItem *item);
    void hidden();
    void hidden_with_id(const QString &id);
    void shown();
    void shown_with_id(const QString &id);
    void moved(int dx,int dy);
    void neededSelectSubtree();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    bool frameEnabled;
    bool selected;
    bool isHidden;
    // 对应 Focus::useLines, 如果前置、互斥国策中存在未知国策，就框起来
    bool useLines;
    QMenu *menu;
    QColor frameColor;
    focustree *tree;
    void drawFrame(QPainter *painter,const QColor& color);
};

#endif // FOCUSITEM_H
