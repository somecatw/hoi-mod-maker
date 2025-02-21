#ifndef LINEITEMS_H
#define LINEITEMS_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QGraphicsItem>

class LineItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit LineItem(QObject *parent = nullptr);
    virtual void setEnd(const QPointF& _end);
    int beginX();
    int endX();
    QRectF boundingRect()const override;

protected:
    QPointF end;
    void updateSize();
};

class BrokenLine : public LineItem{
    Q_OBJECT
public:
    explicit BrokenLine(QObject *parent = nullptr);
    void setTurnPoint(int turnPoint);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QPen getPen() const = 0;
    int turnPoint;
    int visibleEndCount;

public slots:
    void hide();
    void show();
    void moveStart(int dx,int dy);
    void moveEnd(int dx,int dy);
};

class SolidLine : public BrokenLine{
    Q_OBJECT
public:
    explicit SolidLine(QObject *parent = nullptr);
protected:
    QPen getPen()const override;
};

class DotLine : public BrokenLine{
    Q_OBJECT
public:
    explicit DotLine(QObject *parent = nullptr);
protected:
    QPen getPen()const override;
};

class ExclusiveLine : public LineItem{
    Q_OBJECT
public:
    static constexpr int h=24;
    explicit ExclusiveLine(QObject *parent = nullptr);
    void setEnd(const QPointF &_end) override;
    QRectF boundingRect()const override;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

};

#endif // LINEITEMS_H
