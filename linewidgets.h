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

protected:
    QPointF end;
signals:
};

class SolidLine : public LineWidget{
    Q_OBJECT
public:
    explicit SolidLine(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *evt) override;
};

class DotLine : public LineWidget{
    Q_OBJECT
public:
    explicit DotLine(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *evt) override;
};

class ExclusiveLine : public LineWidget{
    Q_OBJECT
public:
    explicit ExclusiveLine(QWidget *parent = nullptr);
    void setEnd(const QPointF &_end) override;
protected:
    void paintEvent(QPaintEvent *evt) override;

};

#endif // LINEWIDGETS_H
