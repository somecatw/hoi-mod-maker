#ifndef FOCUSLISTVIEW_H
#define FOCUSLISTVIEW_H

#include <QWidget>
#include <QListView>
#include <QStringListModel>
#include <QMenu>

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

class FocusListView : public QWidget
{
    Q_OBJECT

public:
    explicit FocusListView(QWidget *parent = nullptr);
    ~FocusListView();
protected:
    void contextMenuEvent(QContextMenuEvent *evt) override;
    void resizeEvent(QResizeEvent *evt) override;
    void leaveEvent(QEvent *evt) override;

public slots:
    void addFocus(const QString &id);
    void showFocus();
    void showFocusWithoutSync(const QString &id);
    void focusHovering(const QModelIndex& index);

signals:
    void focusShown(const QString &str);
    void focusShownOnHover(const QString &str);

private:
    Ui::FocusListView *ui;
    ExtendedListView *view;
    QStringListModel *lst;
    QMenu *menu;
    QModelIndex selectedIdx;
};

#endif // FOCUSLISTVIEW_H
