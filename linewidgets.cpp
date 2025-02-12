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
    QPen pen(Qt::gray);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    painter.drawLine(beginX(),0,beginX(),end.y()-focustree::hgap/2);
    painter.drawLine(beginX(),end.y()-focustree::hgap/2,endX(),end.y()-focustree::hgap/2);
    painter.drawLine(endX(),end.y()-focustree::hgap/2,endX(),end.y());
}

DotLine::DotLine(QWidget *parent)
    :LineWidget(parent){}

void DotLine::paintEvent(QPaintEvent *evt){
    if(end.y()<0)return;

    QPainter painter(this);
    QPen pen(Qt::gray);
    pen.setWidth(2);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    painter.drawLine(beginX(),0,beginX(),end.y()-focustree::hgap/2);
    painter.drawLine(beginX(),end.y()-focustree::hgap/2,endX(),end.y()-focustree::hgap/2);
    painter.drawLine(endX(),end.y()-focustree::hgap/2,endX(),end.y());
}

ExclusiveLine::ExclusiveLine(QWidget *parent)
    :LineWidget(parent){}

void ExclusiveLine::setEnd(const QPointF &_end){

}

void ExclusiveLine::paintEvent(QPaintEvent *evt){

}
