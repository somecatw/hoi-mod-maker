#include "focusitem.h"
#include "ui_focusitem.h"

FocusItem::FocusItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FocusItem)
{
    ui->setupUi(this);
    hovering=false;
}

FocusItem::~FocusItem()
{
    delete ui;
}

void FocusItem::enterEvent(QEnterEvent *evt){
    hovering=true;
}

void FocusItem::leaveEvent(QEvent *evt){
    hovering=false;
}

void FocusItem::paintEvent(QPaintEvent *evt){
    QPainter painter(this);

    painter.setBrush(Qt::gray);
    painter.drawEllipse({25,25},10,10);
}

void FocusItem::mousePressEvent(QMouseEvent *evt){

}
