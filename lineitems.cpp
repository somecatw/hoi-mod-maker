#include "lineitems.h"
#include "focustree.h"

LineItem::LineItem(QObject *parent)
    : QObject{parent}
{
    // setAttribute(Qt::WA_TranslucentBackground);
    // setAttribute(Qt::WA_TransparentForMouseEvents);
}

void LineItem::setEnd(const QPointF& _end){
    //end   = QPointF(int(_end.x()/focustree::wgap)*focustree::wgap+focustree::wgap/2,
    //              int(_end.y()/focustree::hgap)*focustree::hgap+focustree::hgap/2);
    end=_end;
}
void LineItem::updateSize(){
    this->update();
    // scene()->update();
}
int LineItem::beginX(){
    if(end.x()>=0)return 1;
    else return -end.x()+1;
}
int LineItem::endX(){
    if(end.x()>=0)return end.x()+1;
    else return 1;
}
void BrokenLine::hide(){
    visibleEndCount--;
    setVisible(false);
}
void BrokenLine::show(){
    visibleEndCount++;
    if(visibleEndCount==2)setVisible(true);
}
QRectF LineItem::boundingRect() const{
    return QRectF(0,0,abs(end.x())+2,end.y());
}

SolidLine::SolidLine(QObject *parent)
    :BrokenLine(parent){turnPoint=0;}

QPen SolidLine::getPen()const{
    QPen pen(QColor(0x99,0x99,0x99,int(255*0.75)));
    pen.setWidth(2);
    return pen;
}

DotLine::DotLine(QObject *parent)
    :BrokenLine(parent){turnPoint=0;}

QPen DotLine::getPen()const{
    QPen pen(QColor(0x99,0x99,0x99,int(255*0.75)));
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    return pen;
}

ExclusiveLine::ExclusiveLine(QObject *parent)
    :LineItem(parent){}

void ExclusiveLine::setEnd(const QPointF &_end){
    end=_end;
    end.setY(0);
}

QRectF ExclusiveLine::boundingRect() const{
    return QRectF(0,0,(abs(end.x())),h);
}

void ExclusiveLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    const int x1=focustree::itemW/2,
        x2=end.x()/2-(20)/2,
        x3=end.x()-focustree::itemW/2-6;

    painter->setClipping(false);
    QPen pen(QColor(0x99,0x99,0x99,int(255*0.75)));

    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    if(end.x()<100){
        painter->drawImage(x1,0,QImage(":/resource/icon/exclusive.png"));
    }else{
        painter->drawLine(x1,h/2+1,x3,h/2+1);
        painter->drawImage(x2,0,QImage(":/resource/icon/exclusive.png"));
        painter->drawImage(x1,6,QImage(":/resource/icon/exclusive_l.png"));
        painter->drawImage(x3,6,QImage(":/resource/icon/exclusive_r.png"));
    }
}

BrokenLine::BrokenLine(QObject *parent):LineItem(parent){
    visibleEndCount=2;
}
void BrokenLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(end.y()<0)return;

    painter->setClipping(false);
    QPen pen=getPen();
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    // painter.drawRect(this->rect());

    double midy=focustree::itemH/2+focustree::hgap*turnPoint;
    painter->drawLine(beginX(),0,beginX(),midy);
    painter->drawLine(beginX(),midy,endX(),midy);
    painter->drawLine(endX(),midy,endX(),end.y());
}
void BrokenLine::setTurnPoint(int _turnPoint){
    turnPoint=_turnPoint;
}

void BrokenLine::moveStart(int dx,int dy){
    QRectF oldpos=boundingRect();
    this->moveBy(dx,dy);
    moveEnd(-dx,-dy);
    //update(oldpos.translated(-dx,-dy));
}

void BrokenLine::moveEnd(int dx,int dy){
    QRectF oldpos=boundingRect();
    if(end.x()<0&&end.x()+dx<0){
        this->moveBy(dx,0);
        //oldpos.translate(-dx,0);
    }else if(end.x()<0){
        this->moveBy(-end.x(),0);
        //oldpos.translate(end.x(),0);
    }else if(end.x()>=0&&end.x()+dx<0){
        this->moveBy((end.x()+dx),0);
        //oldpos.translate(-(end.x()+dx),0);
    }
    end+=QPoint(dx,dy);
    scene()->update(mapRectToScene(oldpos));
    updateSize();
}
