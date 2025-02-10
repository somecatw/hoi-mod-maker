#ifndef PRESET_ADD_POINT_H
#define PRESET_ADD_POINT_H

#include <QMainWindow>

namespace Ui {
class preset_add_point;
}

class preset_add_point : public QMainWindow
{
    Q_OBJECT
private:
public:
    explicit preset_add_point(QWidget *parent = nullptr);
    ~preset_add_point();
    Ui::preset_add_point *ui;
    QString out_str;

private slots:
    void on_pushButton_2_clicked();



};

#endif // PRESET_ADD_POINT_H
