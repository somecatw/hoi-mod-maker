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
    update();
}

void FocusItem::leaveEvent(QEvent *evt){
    hovering=false;
    update();
}

void FocusItem::paintEvent(QPaintEvent *evt){
    QPainter painter(this);
    painter.setBrush(Qt::gray);
    painter.drawEllipse({40,20},15,15);

    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WrapAnywhere);
    textOption.setAlignment(Qt::AlignCenter);
    painter.drawText(QRect({0,40,80,40}),focusid,textOption);

    if(hovering){
        QImage bd(":/resource/icon/focus_hover.png");
        painter.drawImage(0,0,bd);
    }
}

void FocusItem::mousePressEvent(QMouseEvent *evt){
    update();
}

void FocusItem::setId(const QString& id){
    focusid=id;
}
