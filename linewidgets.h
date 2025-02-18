#ifndef LINEWIDGETS_H
#define LINEWIDGETS_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class LineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LineWidget(QWidget *parent = nullptr);
    virtual void setEnd(const QPointF& _end);
    int beginX();
    int endX();
    QSize sizeHint()const override;
    QGraphicsProxyWidget *proxy;

protected:
    QPointF end;
    void updateSize();
};

class BrokenLine : public LineWidget{
    Q_OBJECT
public:
    explicit BrokenLine(QWidget *parent = nullptr,bool _type=0);
    void setType(bool _type);

protected:
    void paintEvent(QPaintEvent *evt) override;
    virtual QPen getPen() const = 0;
    bool type;
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
    explicit SolidLine(QWidget *parent = nullptr,bool _type=0);
protected:
    QPen getPen()const override;
};

class DotLine : public BrokenLine{
    Q_OBJECT
public:
    explicit DotLine(QWidget *parent = nullptr,bool _type=0);
protected:
    QPen getPen()const override;
};

class ExclusiveLine : public LineWidget{
    Q_OBJECT
public:
    static constexpr int h=24;
    explicit ExclusiveLine(QWidget *parent = nullptr);
    void setEnd(const QPointF &_end) override;
    QSize sizeHint()const override;
protected:
    void paintEvent(QPaintEvent *evt) override;

};

#endif // LINEWIDGETS_H
