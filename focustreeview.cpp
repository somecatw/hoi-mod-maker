
#include "focustreeview.h"
#include <QContextMenuEvent>
#include "focustree.h"
#include "focuseditor.h"
#include "prereqgroupselector.h"

void FocusTreeView::contextMenuEvent(QContextMenuEvent *evt){
    FocusItem *t=getFocusAtGlobalPos(evt->globalPos());
    if(menuTargetItem == t && t){
        menuTargetItem=t;
        bool addPreqFlag=false,removePreqFlag=false;
        bool addExclFlag=false,removeExclFlag=false;

        if(selection->size()==1){
            FocusItem *base=(*selection->itemSet().begin());
            if(selection->size()==1 && t->y()<base->y()){
                if(!base->preqItems.contains(t))
                    addPreqFlag=true;
                else removePreqFlag=true;
            }
            if(selection->size()==1 && t->y()==base->y() && t!=base){
                if(!base->exclItems.contains(t))
                    addExclFlag=true;
                else removeExclFlag=true;
            }
        }

        if(addPreqFlag)
            menu->addAction(act1);
        if(removePreqFlag)
            menu->addAction(act2);
        if(addExclFlag)
            menu->addAction(act3);
        if(removeExclFlag)
            menu->addAction(act4);

        menu->exec(evt->globalPos());

        if(addPreqFlag)
            menu->removeAction(act1);
        if(removePreqFlag)
            menu->removeAction(act2);
        if(addExclFlag)
            menu->removeAction(act3);
        if(removeExclFlag)
            menu->removeAction(act4);
    }
}

void FocusTreeView::mousePressEvent(QMouseEvent *evt){
    FocusItem *t=getFocusAtGlobalPos(evt->globalPosition().toPoint());
    bool shiftPressed = evt->modifiers() & Qt::ShiftModifier;
    bool ctrlPressed = evt->modifiers() & Qt::ControlModifier;
    if(evt->button() == Qt::LeftButton){
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
    }else if(evt->button() == Qt::RightButton){
        setDragMode(QGraphicsView::ScrollHandDrag);
        // 右键拖动按不出来，曲线救国
        QMouseEvent leftPressEvent(QEvent::MouseButtonPress, evt->pos(), evt->globalPosition().toPoint(), Qt::LeftButton, Qt::NoButton, evt->modifiers());
        QGraphicsView::mousePressEvent(&leftPressEvent);
        menuTargetItem=t;
    }
    QGraphicsView::mousePressEvent(evt);
}

void FocusTreeView::mouseMoveEvent(QMouseEvent *evt){
    FocusItem *t=getFocusAtGlobalPos(evt->globalPosition().toPoint());
    if(hoveringItem)hoveringItem->hovering=false,hoveringItem->update();
    if(t)t->hovering=true,hoveringItem=t,t->update();
    if(dragging){
        QPointF localpos=mapToScene(mapFromGlobal(evt->globalPosition()).toPoint());
        int nx=localpos.x()/focustree::wgap;
        int ny=localpos.y()/focustree::hgap;
        nx-=moveReferenceItem->displayPos.x();
        ny-=moveReferenceItem->displayPos.y();

        ny=selection->limitY(ny);

        if(nx||ny){
            selection->move(tree,nx,ny);
            tree->uManager->addAction(newAction<MoveFocusAction>(selection->itemSet(),tree,nx,ny));
        }
    }else{
        menuTargetItem = nullptr;
        QGraphicsView::mouseMoveEvent(evt);
    }
}
void FocusTreeView::clearBuffer(){
    moveReferenceItem = nullptr;
    hoveringItem = nullptr;
    menuTargetItem = nullptr;
    selection->clear();
}
void FocusTreeView::mouseReleaseEvent(QMouseEvent *evt){
    FocusItem *t=getFocusAtGlobalPos(evt->globalPosition().toPoint());
    if(evt->button() == Qt::LeftButton){
        if(!dragging){
            if(!(evt->modifiers() & Qt::ShiftModifier)){
                clearSelection();
                if(t)select(t);
            }
        }
        if(dragging) dragging=false;
    }
    if(evt->button() == Qt::RightButton){
        QMouseEvent leftReleaseEvent(QEvent::MouseButtonRelease, evt->pos(), evt->globalPosition().toPoint(), Qt::LeftButton, Qt::NoButton, evt->modifiers());
        QGraphicsView::mouseReleaseEvent(&leftReleaseEvent);
        setDragMode(QGraphicsView::NoDrag);
    }
    QGraphicsView::mouseReleaseEvent(evt);
}

void FocusTreeView::mouseDoubleClickEvent(QMouseEvent *evt){
    FocusItem *t=getFocusAtGlobalPos(evt->globalPosition().toPoint());
    if(t){
        FocusEditor *e=new FocusEditor(tree);
        e->show();
    }
}

FocusItem *FocusTreeView::getFocusAtGlobalPos(const QPoint &p)const{
    QGraphicsItem *item=this->itemAt(mapFromGlobal(p));
    if(!item)return nullptr;
    FocusItem *t=dynamic_cast<FocusItem*>(item);
    if(!t)return nullptr;
    return t;
}

void FocusTreeView::wheelEvent(QWheelEvent *evt){
    const double fac = 1.1;
    if(evt->angleDelta().y()>0){
        scale(fac,fac);
    }else scale(1.0/fac,1.0/fac);
}

void FocusTreeView::addPrereq(){
    PrereqGroupSelector selector=PrereqGroupSelector(
        tree->model()->data((*selection->itemSet().begin())->focusid),
        tree->model()->data(menuTargetItem->focusid),
        tree,this);
    selector.exec();
}

void FocusTreeView::removePrereq(){
    int group=tree->model()->getFocusPreqGroup((*selection->itemSet().begin())->focusid,menuTargetItem->focusid);
    tree->removeFocusPrereq((*selection->itemSet().begin())->focusid,menuTargetItem->focusid);
    tree->uManager->addAction(newAction<RemovePrereqAction>(tree,(*selection->itemSet().begin())->focusid,menuTargetItem->focusid,group));
}

void FocusTreeView::addExcl(){
    tree->addFocusExcl((*selection->itemSet().begin())->focusid,menuTargetItem->focusid);
    tree->uManager->addAction(newAction<AddExclAction>(tree,(*selection->itemSet().begin())->focusid,menuTargetItem->focusid));
}

void FocusTreeView::removeExcl(){
    tree->removeFocusExcl((*selection->itemSet().begin())->focusid,menuTargetItem->focusid);
    tree->uManager->addAction(newAction<RemoveExclAction>(tree,(*selection->itemSet().begin())->focusid,menuTargetItem->focusid));
}

FocusTreeView::FocusTreeView(focustree *_tree,QGraphicsScene *scene, QWidget *parent)
    :QGraphicsView(scene,parent)
{
    tree=_tree;
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    menu=new QMenu(this);
    QAction *act_hide=new QAction("隐藏",this);
    QAction *act_select=new QAction("选中子树",this);
    menu->addAction(act_hide);
    menu->addAction(act_select);
    hoveringItem = nullptr;
    menuTargetItem = nullptr;
    moveReferenceItem = nullptr;
    dragging = false;

    act1=new QAction("设置为前置",this);
    connect(act1,&QAction::triggered,this,&FocusTreeView::addPrereq);

    act2=new QAction("取消前置",this);
    connect(act2,&QAction::triggered,this,&FocusTreeView::removePrereq);

    act3=new QAction("设置为互斥",this);
    connect(act3,&QAction::triggered,this,&FocusTreeView::addExcl);

    act4=new QAction("取消互斥",this);
    connect(act4,&QAction::triggered,this,&FocusTreeView::removeExcl);


    this->selection=new MultipleFocusSelection(this);
    connect(act_hide,&QAction::triggered,this,&FocusTreeView::hideFocus);
    connect(act_select,&QAction::triggered,this,&FocusTreeView::selectSubtree);
}

void FocusTreeView::hideFocus(){
    if(menuTargetItem){
        QSet<FocusItem*> tempSet=selection->itemSet();
        clearSelection();
        if(!tempSet.contains(menuTargetItem)){
            menuTargetItem->hide();
            tree->uManager->addAction(newAction<HideFocusAction>(QSet<FocusItem*>({menuTargetItem})));
        }else{
            foreach(FocusItem *item,tempSet){
                item->hide();
                tree->uManager->addAction(newAction<HideFocusAction>(tempSet));
            }
        }
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
void FocusTreeView::selectSubtree(){
    if(menuTargetItem){
        menuTargetItem->selectSubtree();
    }
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

void MultipleFocusSelection::move(focustree *tree,int dx,int dy){
    tree->batchMoveFocus(items,dx,dy);
}

const QSet<FocusItem*> &MultipleFocusSelection::itemSet()const{
    return items;
}
