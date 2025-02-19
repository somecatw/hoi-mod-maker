#include "focusitem.h"
#include "ui_focusitem.h"
#include "focustree.h"
#include <QMenu>

FocusItem::FocusItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FocusItem)
{
    ui->setupUi(this);
    hovering=false;
    frameEnabled=false;
    selected=false;
    isHidden=false;
    visiblePreqCount=0;
    setAttribute(Qt::WA_TranslucentBackground);
}

FocusItem::~FocusItem()
{
    delete ui;
}

void FocusItem::contextMenuEvent(QContextMenuEvent *evt){
    menu->exec(QCursor::pos());
    qDebug()<<999;
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

/*
void FocusItem::mousePressEvent(QMouseEvent *evt){
    if(evt->button()==Qt::LeftButton){
        selected=true;
        disconnect(tree,&focustree::resetSelection,this,&FocusItem::deSelect);
        tree->handleSelection(this, evt->modifiers()&Qt::ShiftModifier);
        connect(tree,&focustree::resetSelection,this,&FocusItem::deSelect);
        update();
    }else if(evt->button()==Qt::RightButton){

    }
}
*/
void FocusItem::setSelected(){
    selected=true;
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
void FocusItem::hideFrame(){
    frameEnabled=false;
    update();
}
void FocusItem::hide(){
    if(isHidden)return;
    isHidden=true;
    setVisible(false);
    update();
    qDebug()<<focusid<<"is hidden";
    emit hidden();
    emit hidden_with_id(focusid);
}

void FocusItem::preqHidden(){
    visiblePreqCount--;
    if(!visiblePreqCount)hide();
}

void FocusItem::preqShown(){
    visiblePreqCount++;
    if(visiblePreqCount==1)show();
}

void FocusItem::show(){
    if(!isHidden)return;
    isHidden=false;
    setVisible(true);
    update();
    qDebug()<<focusid<<"is shown";
    emit shown();
    emit shown_with_id(focusid);
}
void FocusItem::reveal(){
    setVisible(true);
    update();
}
void FocusItem::unreveal(){
    setVisible(false);
    update();
}

void FocusItem::move(int dx,int dy,bool isManual){
    tree->model()->moveFocus(focusid,dx,dy,isManual);
}
