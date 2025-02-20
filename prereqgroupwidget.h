#ifndef PREREQGROUPWIDGET_H
#define PREREQGROUPWIDGET_H

#include <QWidget>

namespace Ui {
class prereqGroupWidget;
}

class PrereqGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrereqGroupWidget(const QVector<QString> &v,int groupId,QColor color,QWidget *parent = nullptr);
    ~PrereqGroupWidget();

private:
    Ui::prereqGroupWidget *ui;
};

#endif // PREREQGROUPWIDGET_H
