#ifndef FOCUSITEM_H
#define FOCUSITEM_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class FocusItem;
}

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
