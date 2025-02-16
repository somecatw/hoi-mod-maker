#include "focuslistview.h"
#include "ui_focuslistview.h"
#include <QContextMenuEvent>

FocusListView::FocusListView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FocusListView)
{
    ui->setupUi(this);
    lst=new QStringListModel(this);
    setMouseTracking(true);

    view=ui->listView;
    view->setModel(lst);
    view->setViewMode(QListView::ListMode);
    view->setEditTriggers(QListView::NoEditTriggers);
    view->setMouseTracking(true);

    menu = new QMenu(this);
    QAction *act=new QAction("显示此国策");
    menu->addAction(act);

    connect(act,&QAction::triggered,this,&FocusListView::showFocus);
    connect(this->view,&ExtendedListView::mouseEntered,this,&FocusListView::focusHovering);
    connect(this->view,&ExtendedListView::mouseLeaved,this,&FocusListView::focusHovering);
}

FocusListView::~FocusListView()
{
    delete ui;
}

void FocusListView::addFocus(const QString &str){
    int id=lst->rowCount();
    lst->insertRow(lst->rowCount());
    lst->setData(lst->index(id),str);
}

void FocusListView::contextMenuEvent(QContextMenuEvent *evt){
    QModelIndex index=view->indexAt(evt->pos()-view->pos());
    if(index.isValid()){
        selectedIdx=index;
        menu->exec(evt->globalPos());
    }
}

void FocusListView::showFocus(){
    QString str=lst->data(selectedIdx).toString();
    lst->removeRow(selectedIdx.row());
    emit focusShown(str);
}

void FocusListView::resizeEvent(QResizeEvent *evt){
    view->resize(evt->size().width(),evt->size().height()-ui->label->height());
    QWidget::resizeEvent(evt);
}

void FocusListView::showFocusWithoutSync(const QString &id){
    for(int row=0;row<lst->rowCount();row++){
        if(lst->data(lst->index(row)).toString()==id){
            lst->removeRow(row);
            return;
        }
    }
}

void FocusListView::focusHovering(const QModelIndex &index){
    if(!index.isValid())emit focusShownOnHover("");
    emit focusShownOnHover(lst->data(index).toString());
}
void FocusListView::leaveEvent(QEvent *evt){
    emit focusShownOnHover("");
}
void ExtendedListView::mouseMoveEvent(QMouseEvent *evt){
    QModelIndex index=indexAt(evt->pos());
    if(index.isValid())emit mouseEntered(index);
    else emit mouseLeaved(index);
}
ExtendedListView::ExtendedListView(QWidget *parent):QListView(parent){}
