#ifndef FOCUSITEM_H
#define FOCUSITEM_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class FocusItem;
}
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

    // used as index
    QString focusid;

protected:
    void enterEvent(QEnterEvent *evt) override;
    void leaveEvent(QEvent *evt) override;
    void paintEvent(QPaintEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
private:
    Ui::FocusItem *ui;
    bool hovering;
};

#endif // FOCUSITEM_H
