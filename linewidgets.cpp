#include "linewidgets.h"
#include "focustree.h"

LineWidget::LineWidget(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void LineWidget::setEnd(const QPointF& _end){
    //end   = QPointF(int(_end.x()/focustree::wgap)*focustree::wgap+focustree::wgap/2,
    //              int(_end.y()/focustree::hgap)*focustree::hgap+focustree::hgap/2);
    end=_end;
    this->setMinimumWidth(end.x());
}

int LineWidget::beginX(){
    if(end.x()>=0)return 1;
    else return -end.x()+1;
}
int LineWidget::endX(){
    if(end.x()>=0)return end.x()+1;
    else return 1;
}

QSize LineWidget::sizeHint() const{
    return {int(abs(end.x()))+2,int(end.y())};
}

SolidLine::SolidLine(QWidget *parent)
    :LineWidget(parent){}

void SolidLine::paintEvent(QPaintEvent *evt){
    if(end.y()<0)return;

    QPainter painter(this);
    painter.setClipping(false);
    QPen pen(QColor(0x99,0x99,0x99,int(255*0.75)));
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    painter.drawLine(beginX(),0,beginX(),focustree::itemH/2);
    painter.drawLine(beginX(),focustree::itemH/2,endX(),focustree::itemH/2);
    painter.drawLine(endX(),focustree::itemH/2,endX(),end.y());
}

DotLine::DotLine(QWidget *parent)
    :LineWidget(parent){}

void DotLine::paintEvent(QPaintEvent *evt){
    if(end.y()<0)return;

    QPainter painter(this);
    painter.setClipping(false);
    QPen pen(QColor(0x99,0x99,0x99,int(255*0.75)));
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    painter.drawLine(beginX(),0,beginX(),focustree::itemH/2);
    painter.drawLine(beginX(),focustree::itemH/2,endX(),focustree::itemH/2);
    painter.drawLine(endX(),focustree::itemH/2,endX(),end.y());
}

ExclusiveLine::ExclusiveLine(QWidget *parent)
    :LineWidget(parent){}

void ExclusiveLine::setEnd(const QPointF &_end){
    end=_end;
    end.setY(0);
    this->setMinimumWidth(end.x());
}

QSize ExclusiveLine::sizeHint() const{
    return {int(abs(end.x())),h};
}

void ExclusiveLine::paintEvent(QPaintEvent *evt){
    const int x1=focustree::itemW/2,
        x2=end.x()/2-(20)/2,
        x3=end.x()-focustree::itemW/2-6;

    QPainter painter(this);
    painter.setClipping(false);
    QPen pen(QColor(0x99,0x99,0x99,int(255*0.75)));

    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    if(end.x()<100){
        painter.drawImage(0,0,QImage(":/resource/icon/exclusive.png"));
    }else{
        painter.drawLine(x1,h/2+1,x3,h/2+1);
        painter.drawImage(x2,0,QImage(":/resource/icon/exclusive.png"));
        painter.drawImage(x1,6,QImage(":/resource/icon/exclusive_l.png"));
        painter.drawImage(x3,6,QImage(":/resource/icon/exclusive_r.png"));
    }
}
