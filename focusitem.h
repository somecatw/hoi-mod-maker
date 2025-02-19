#ifndef FOCUSITEM_H
#define FOCUSITEM_H

#include <QWidget>
#include <QPainter>

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
class FocusItem : public QWidget
{
    Q_OBJECT

public:
    explicit FocusItem(QWidget *parent = nullptr);
    ~FocusItem();
    void setup(const QString &id,focustree *tr);
    void setFrame(const QColor &color);
    void hide();
    void move(int x,int y,bool isManual=true);
    void setSelected();
    // 用来在右侧悬停时暂时显示国策
    void reveal();
    void unreveal();
    int visiblePreqCount;
    QPoint displayPos;
    QVector<FocusItem*> exclItems,preqItems,postItems;
    // 对应国策的 id
    QString focusid;
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
    void enterEvent(QEnterEvent *evt) override;
    void leaveEvent(QEvent *evt) override;
    void paintEvent(QPaintEvent *evt) override;
    //void mousePressEvent(QMouseEvent *evt) override;
    void contextMenuEvent(QContextMenuEvent *evt) override;
private:
    Ui::FocusItem *ui;
    bool hovering;
    bool frameEnabled;
    bool selected;
    bool isHidden;
    QMenu *menu;
    QColor frameColor;
    focustree *tree;
    void drawFrame(QPainter *painter,const QColor& color);
};

#endif // FOCUSITEM_H
