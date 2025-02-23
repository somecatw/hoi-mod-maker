#ifndef HOILANGVIEW_H
#define HOILANGVIEW_H

#include <QWidget>
#include <QStandardItemModel>
#include "parser.h"
#include "qabstractbutton.h"
#include "qpushbutton.h"

namespace Ui {
class HoiLangView;
}

class EditProtector{
public:
    EditProtector();
    EditProtector(std::initializer_list<QString> l);
    bool check(const QString &x);
private:
    QSet<QString> protectedTerms;
};
extern EditProtector defaultFocusProtector;

class HoiLangItem : public QStandardItem{
public:
    explicit HoiLangItem(HoiLangItem *parent,AttrPointer ptr);
    AttrPointer ptr;
    QString path;
    bool isProtected;
};

class HoiLangView : public QWidget
{
    Q_OBJECT

public:
    explicit HoiLangView(QWidget *parent = nullptr);
    ~HoiLangView();
    void init(AttrPointer attr);
    EditProtector protector;

protected:
    void resizeEvent(QResizeEvent *evt) override;
private slots:
    void handleClick(const QModelIndex &curr);
    void handleChange();
    void handleButtonClick(QAbstractButton *btn);

private:
    Ui::HoiLangView *ui;
    QStandardItemModel *itemModel;
    HoiLangItem *editingItem;
    AttrPointer containedObj;
    bool hasUnsavedChange;

    void recursivelyBuild(ObjPointer obj,HoiLangItem *parent);
    void updateItem(QStandardItem *parent,AttrPointer ptr);
};

#endif // HOILANGVIEW_H
