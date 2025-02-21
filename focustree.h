#ifndef FOCUSTREE_H
#define FOCUSTREE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsProxyWidget>
#include <QSplitter>
#include "focusmodel.h"
#include "focusitem.h"
#include "focuslistview.h"
#include "undomanager.h"
#include "focustreeview.h"

namespace Ui {
class focustree;
}

class LineItem;

class focustree : public QMainWindow
{
    Q_OBJECT

public:
    explicit focustree(QWidget *parent = nullptr);
    ~focustree();
    static constexpr double wgap = 80;
    static constexpr double hgap = 100;

    static inline const double itemW = 80;
    static inline const double itemH = 80;

    /* 同种颜色的前置国策，只需完成任意一个，详见 focusmodel.h
     * 不过好像换个跟红色（互斥国策）差别大一点的颜色比较好？有没有色彩老师指导一下
     */
    static inline QColor colorList[10]={0x4B0002,0xFF8C00,0x8B008B,0x001f3f};
    UndoManager *uManager;

    FocusModel *model();
    //void handleSelection(FocusItem *item,bool shiftPressed);
    void batchMoveFocus(const QSet<FocusItem*> items,int dx,int dy);
    void addFocusPrereq(const QString &baseId,const QString &targetId,int group);
    void removeFocusPrereq(const QString &baseId,const QString &targetId);
    void setPreqFrames(const QString &id);
    bool noPreqHidden(const QString &id);

protected:
    void resizeEvent(QResizeEvent *evt) override;

signals:
    //void resetSelection();
    void focusHidden(const QString &id);
    void focusShown(const QString &id);

public slots:
    void showFocus(const QString &id);
    void handleFocusMove(const QVector<QString> &ids,int dx,int dy);
    void handleFocusPreqUpdated(const QString &id);

private slots:
    void on_focusa_clicked();
    void updateExclusiveFocus(const QString &name);
    void on_actionopen_triggered();
    void revealFocus(const QString &id);

    void on_action_redo_triggered();
    void on_action_undo_triggered();

    void on_actiondaochu_triggered();

private:
    Ui::focustree *focustreeui;
    QSplitter *splitter;
    FocusListView *listView;
    QGraphicsScene *treeScene;
    FocusTreeView *treeView;
    FocusModel *focusModel;
    QMap<QString,FocusItem*> items;
    QMap<std::pair<int,int>,QVector<FocusItem*>> focusGrid;
    QMap<QPair<FocusItem*,FocusItem*>,LineItem*> exclLines;

    // 如果 id 已经存在，就直接 return
    void addFocusItem(const Focus& f);

    void addFocusPreqLine(const Focus& f);
    void addFocusExLine(const Focus& f);
    void removeFocusExLine(const Focus &f);
    void removeFocusPreqLine(FocusItem *item);

    bool xQuery(int x1,int x2,int y,std::function<bool(FocusItem*)> f)const;
    int yQuery(int y1,int y2,int x,std::function<bool(FocusItem*)> f)const;
    LineItem *getExclLine(FocusItem *a,FocusItem *b)const;
};

#endif // FOCUSTREE_H
