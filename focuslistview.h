#ifndef FOCUSLISTVIEW_H
#define FOCUSLISTVIEW_H

#include <QWidget>
#include <QListView>
#include <QStringListModel>
#include <QMenu>
#include <QStyledItemDelegate>

namespace Ui {
class FocusListView;
}

class ExtendedListView : public QListView{
    Q_OBJECT
public:
    explicit ExtendedListView(QWidget *parent = nullptr);
signals:
    void mouseEntered(const QModelIndex &index);

    // 实际上这个信号表示鼠标不处于国策之上
    void mouseLeaved(const QModelIndex &index);
protected:
    void mouseMoveEvent(QMouseEvent *evt) override;
};

class focustree;

class ExtendedItemDelegate : public QStyledItemDelegate{
    Q_OBJECT
public:
    explicit ExtendedItemDelegate(focustree *tree,QObject *parent = nullptr);
    void paint(QPainter *painter,const QStyleOptionViewItem &opt,const QModelIndex &index)const override;
private:
    mutable QMap<QString,bool> noPreqHidden;
    mutable int updatedCount;
    focustree *tree;
    void checkoutPreq(const QModelIndex &index) const;
public slots:
    void preqChanged();
};

class FocusListView : public QWidget
{
    Q_OBJECT

public:
    explicit FocusListView(focustree *tree,QWidget *parent = nullptr);
    ~FocusListView();
protected:
    void contextMenuEvent(QContextMenuEvent *evt) override;
    void resizeEvent(QResizeEvent *evt) override;
    void leaveEvent(QEvent *evt) override;

public slots:
    void addFocus(const QString &id);
    void removeFocus();
    void removeFocusWithoutSync(const QString &id);
    void focusHovering(const QModelIndex& index);

signals:
    void focusAdded();
    void focusRemoved(const QString &str);
    void focusShownOnHover(const QString &str);

private:
    Ui::FocusListView *ui;
    ExtendedListView *view;
    ExtendedItemDelegate *delegate;
    QStringListModel *lst;
    QMenu *menu;
    QModelIndex selectedIdx;
};

#endif // FOCUSLISTVIEW_H
