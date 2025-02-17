#include "focuslistview.h"
#include "ui_focuslistview.h"
#include "focustree.h"
#include <QContextMenuEvent>

FocusListView::FocusListView(focustree *tree,QWidget *parent)
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

    delegate=new ExtendedItemDelegate(tree,this);
    view->setItemDelegate(delegate);

    menu = new QMenu(this);
    QAction *act=new QAction("显示此国策");
    menu->addAction(act);

    connect(act,&QAction::triggered,this,&FocusListView::removeFocus);
    connect(this->view,&ExtendedListView::mouseEntered,this,&FocusListView::focusHovering);
    connect(this->view,&ExtendedListView::mouseLeaved,this,&FocusListView::focusHovering);

    connect(this,&FocusListView::focusAdded,delegate,&ExtendedItemDelegate::preqChanged);
    connect(this,&FocusListView::focusRemoved,delegate,&ExtendedItemDelegate::preqChanged);
}

FocusListView::~FocusListView()
{
    delete ui;
}

void FocusListView::addFocus(const QString &str){
    int id=lst->rowCount();
    lst->insertRow(lst->rowCount());
    lst->setData(lst->index(id),str);
    emit focusAdded();
}

void FocusListView::contextMenuEvent(QContextMenuEvent *evt){
    QModelIndex index=view->indexAt(evt->pos()-view->pos());
    if(index.isValid()){
        selectedIdx=index;
        menu->exec(evt->globalPos());
    }
}

void FocusListView::removeFocus(){
    QString str=lst->data(selectedIdx).toString();
    lst->removeRow(selectedIdx.row());
    emit focusRemoved(str);
}

void FocusListView::resizeEvent(QResizeEvent *evt){
    view->resize(evt->size().width(),evt->size().height()-ui->label->height());
    QWidget::resizeEvent(evt);
}

void FocusListView::removeFocusWithoutSync(const QString &id){
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

ExtendedItemDelegate::ExtendedItemDelegate(focustree *_tree,QObject *parent)
    :QStyledItemDelegate(parent){
    tree=_tree;
}

void ExtendedItemDelegate::preqChanged(){
    updatedCount=1;
    qDebug()<<"preq changed";
}

void ExtendedItemDelegate::paint(QPainter *painter,const QStyleOptionViewItem &opt,const QModelIndex &index)const{
    if(updatedCount)checkoutPreq(index);

    if(noPreqHidden.contains(index.data().toString()))
        painter->fillRect(opt.rect,0xdddddd);
    QStyledItemDelegate::paint(painter,opt,index);
}
void ExtendedItemDelegate::checkoutPreq(const QModelIndex &index) const{
    auto *model=index.model();
    for(int i=0;i<model->rowCount();i++){
        if(tree->noPreqHidden(model->index(i,0).data().toString())){
            noPreqHidden.insert(model->index(i,0).data().toString(),true);
        }else noPreqHidden.remove(model->index(i,0).data().toString());
    }
    updatedCount=0;
    tree->update();
}
