
#include "focustreeview.h"
#include <QContextMenuEvent>
#include "focustree.h"

void FocusTreeView::contextMenuEvent(QContextMenuEvent *evt){
    FocusItem *t=getFocusAtGlobalPos(evt->globalPos());
    if(t)menuTargetItem=t;
    menu->exec(evt->globalPos());
}

void FocusTreeView::mousePressEvent(QMouseEvent *evt){
    FocusItem *t=getFocusAtGlobalPos(evt->globalPosition().toPoint());
    bool shiftPressed = evt->modifiers() & Qt::ShiftModifier;
    bool ctrlPressed = evt->modifiers() & Qt::ControlModifier;
    if(t){
        if(!(shiftPressed) && !selection->contains(t))
            clearSelection();
        if(shiftPressed && selection->contains(t))deSelect(t);
        else select(t);
        if(ctrlPressed){
            dragging=true;
            moveReferenceItem=t;
        }
    }else clearSelection();
    QGraphicsView::mousePressEvent(evt);
}

void FocusTreeView::mouseMoveEvent(QMouseEvent *evt){
    if(dragging){
        QPointF localpos=mapToScene(mapFromGlobal(evt->globalPosition()).toPoint());
        int nx=localpos.x()/focustree::wgap;
        int ny=localpos.y()/focustree::hgap;
        nx-=moveReferenceItem->displayPos.x();
        ny-=moveReferenceItem->displayPos.y();

        ny=selection->limitY(ny);

        if(QPoint(nx,ny)!=moveReferenceItem->displayPos){
            selection->move(nx,ny);
            tree->uManager->addAction(newAction<MoveFocusAction>(selection->itemSet(),nx,ny));
        }
    }else QGraphicsView::mouseMoveEvent(evt);
}

void FocusTreeView::mouseReleaseEvent(QMouseEvent *evt){
    FocusItem *t=getFocusAtGlobalPos(evt->globalPosition().toPoint());
    if(!dragging){
        if(!(evt->modifiers() & Qt::ShiftModifier)){
            clearSelection();
            if(t)select(t);
        }
    }
    if(dragging) dragging=false;
    QGraphicsView::mouseReleaseEvent(evt);
}
FocusItem *FocusTreeView::getFocusAtGlobalPos(const QPoint &p)const{
    QGraphicsItem *item=this->itemAt(mapFromGlobal(p));
    if(!item)return nullptr;
    QGraphicsProxyWidget *w=dynamic_cast<QGraphicsProxyWidget*>(item);
    if(!w)return nullptr;
    FocusItem *t=dynamic_cast<FocusItem*>(w->widget());
    if(!t)return nullptr;
    return t;
}

void FocusTreeView::wheelEvent(QWheelEvent *evt){
    const double fac = 1.1;
    if(evt->angleDelta().y()>0){
        scale(fac,fac);
    }else scale(1.0/fac,1.0/fac);
}

FocusTreeView::FocusTreeView(focustree *_tree,QGraphicsScene *scene, QWidget *parent)
    :QGraphicsView(scene,parent)
{
    tree=_tree;
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
    menu=new QMenu(this);
    QAction *act=new QAction("hide",this);
    menu->addAction(act);
    menuTargetItem = nullptr;
    moveReferenceItem = nullptr;
    dragging = false;
    this->selection=new MultipleFocusSelection(this);
    connect(act,&QAction::triggered,this,&FocusTreeView::hideFocus);
}

void FocusTreeView::hideFocus(){
    if(menuTargetItem){
        menuTargetItem->hide();
        tree->uManager->addAction(newAction<HideFocusAction>(menuTargetItem));
    }
}

void FocusTreeView::clearSelection(){
    selection->clear();
    emit cleared();
}
void FocusTreeView::select(FocusItem *item){
    if(selection->contains(item))return;
    emit frameResetNeeded();
    selection->addItem(item);
    tree->setPreqFrames(item->focusid);
    item->setSelected();
}
void FocusTreeView::deSelect(FocusItem *item){
    if(!selection->contains(item))return;
    emit frameResetNeeded();
    selection->removeItem(item);
    item->deSelect();
}

MultipleFocusSelection::MultipleFocusSelection(QObject *parent):QObject(parent){}

void MultipleFocusSelection::addItem(FocusItem *item){
    items.insert(item);
    if(exclItems.contains(item))exclItems.remove(item);
    foreach(FocusItem *excl,item->exclItems)
        if(!items.contains(excl))exclItems.insert(excl);
    updateLimits();
}
void MultipleFocusSelection::removeItem(FocusItem *item){
    items.remove(item);
    foreach(FocusItem *excl,item->exclItems)
        if(items.contains(excl)) exclItems.insert(item);
    updateLimits();
}

void MultipleFocusSelection::clear(){
    items.clear();
    exclItems.clear();
}

size_t MultipleFocusSelection::size() const{
    return items.size();
}

bool MultipleFocusSelection::contains(FocusItem *item) const{
    return items.contains(item);
}

void MultipleFocusSelection::updateLimits(){
    uLimit=-1e9;dLimit=1e9;
    if(exclItems.size())uLimit=dLimit=0;
    else{
        foreach(FocusItem *item,items){
            foreach(FocusItem *preq,item->preqItems){
                if(items.contains(preq))continue;
                uLimit=std::max(uLimit,(preq->displayPos.y()+1)-item->displayPos.y());
            }
            foreach(FocusItem *post,item->postItems){
                if(items.contains(post))continue;
                dLimit=std::min(dLimit,post->displayPos.y()-1-item->displayPos.y());
            }
        }

    }
}

int MultipleFocusSelection::limitY(int targetY){
    updateLimits();
    return std::min(std::max(targetY,uLimit),dLimit);
}

void MultipleFocusSelection::move(int dx,int dy){
    foreach(FocusItem *item,items){
        item->move(dx,dy);
    }
}

const QSet<FocusItem*> &MultipleFocusSelection::itemSet()const{
    return items;
}
