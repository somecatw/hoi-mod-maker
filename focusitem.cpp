#include "focusitem.h"
#include "ui_focusitem.h"

FocusItem::FocusItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FocusItem)
{
    ui->setupUi(this);
    hovering=false;
    frameEnabled=false;
    selected=false;
    setAttribute(Qt::WA_TranslucentBackground);
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
        //QImage bd(":/resource/icon/focus_hover.png");
        //painter.drawImage(0,0,bd);
        drawFrame(&painter,Qt::green);
    }else if(selected){
        drawFrame(&painter,0x1effff);
    }else if(frameEnabled){
        drawFrame(&painter,frameColor);
    }
}

void FocusItem::drawFrame(QPainter *painter,const QColor& color){
    painter->setBrush(QBrush(color));
    painter->setPen(Qt::NoPen);

    const int siz=80,len=20,wid=6;
    painter->drawRect(0,0,len,wid);
    painter->drawRect(0,0,wid,len);

    painter->drawRect(siz-len,0,len,wid);
    painter->drawRect(siz-wid,0,wid,len);

    painter->drawRect(0,siz-wid,len,wid);
    painter->drawRect(0,siz-len,wid,len);

    painter->drawRect(siz-len,siz-wid,len,wid);
    painter->drawRect(siz-wid,siz-len,wid,len);
}

void FocusItem::mousePressEvent(QMouseEvent *evt){
    selected=true;
    disconnect(tree,&focustree::resetSelection,this,&FocusItem::deSelect);
    emit tree->resetSelection();
    tree->setPreqFrames(this->focusid);
    connect(tree,&focustree::resetSelection,this,&FocusItem::deSelect);
    update();
}

void FocusItem::setup(const QString& id,focustree *tr){
    focusid=id;
    tree=tr;
}

void FocusItem::deSelect(){
    selected=false;
    frameEnabled=false;
    update();
}

void FocusItem::setFrame(const QColor &color){
    frameEnabled=true;
    frameColor=color;
}
