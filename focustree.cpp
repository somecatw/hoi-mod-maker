#include "focustree.h"
#include "ui_focustree.h"
#include "focuseditor.h"
#include "parser.h"
#include "lineitems.h"
#include "prereqgroupselector.h"

focustree::focustree(QWidget *parent)
    : QMainWindow(parent)
    , focustreeui(new Ui::focustree)
{
    focustreeui->setupUi(this);
    treeScene = new QGraphicsScene(this);

    // 不加这个的话删东西的时候会 crash，不知道对性能有多少影响
    treeScene->setItemIndexMethod(QGraphicsScene::NoIndex);

    treeView = new FocusTreeView(this,treeScene);
    this->setCentralWidget(treeView);
    this->focusModel = new FocusModel(this);
    this->splitter = new QSplitter(this);
    this->listView = new FocusListView(this);

    connect(this,&focustree::focusHidden,this->listView,&FocusListView::addFocus);
    connect(this->listView,&FocusListView::focusRemoved,this,&focustree::showFocus);
    connect(this,&focustree::focusShown,this->listView,&FocusListView::removeFocusWithoutSync);
    connect(this->listView,&FocusListView::focusShownOnHover,this,&focustree::revealFocus);

    connect(this->focusModel,&FocusModel::focusMoved,this,&focustree::handleFocusMove);
    connect(this->focusModel,&FocusModel::focusPreqChanged,this,&focustree::handleFocusPreqUpdated);
    connect(this->focusModel,&FocusModel::focusExclChanged,this,&focustree::handleFocusExclUpdated);

    splitter->addWidget(treeView);
    splitter->addWidget(listView);
    splitter->setCollapsible(0,false);
    splitter->setCollapsible(1,false);
    splitter->setMinimumSize({800,600});

    uManager = new UndoManager(this);

    this->setCentralWidget(splitter);
    this->treeView->resize(int(size().width()*0.75),size().height());
}

focustree::~focustree()
{
    delete focustreeui;
}

FocusItem *toItem(QGraphicsProxyWidget *w){
    FocusItem *item = ((FocusItem*)(w->widget()));
    return item;
}

void focustree::on_focusa_clicked()
{
    FocusEditor *a = new FocusEditor();
    a->show();
}

void focustree::showFocus(const QString &id){
    items[id]->show();
    uManager->addAction(newAction<ShowFocusAction>(QSet<FocusItem*>({items[id]})));
}

void focustree::addFocusItem(const Focus& f){
    if(items.count(f.id))return;

    FocusItem *item = new FocusItem();
    item->setup(f.id,this);
    int rx=f.x,ry=f.y;

    connect(this->treeView,&FocusTreeView::cleared,item,&FocusItem::deSelect);
    connect(item,&FocusItem::hidden_with_id,this,&focustree::focusHidden);
    connect(item,&FocusItem::shown_with_id,this,&focustree::focusShown);
    connect(this->treeView,&FocusTreeView::frameResetNeeded,item,&FocusItem::hideFrame);
    connect(item,&FocusItem::implicitlySelected,this->treeView,&FocusTreeView::select);

    this->treeScene->addItem(item);
    item->setPos({wgap*f.x, hgap*f.y});
    items.insert(f.id,item);

    if(f.relativeId.size()){
        addFocusItem(this->focusModel->data(f.relativeId));
        FocusItem *rel = items[f.relativeId];
        item->setPos({wgap*f.x+rel->x(),hgap*f.y+rel->y()});
        rx+=round(rel->x()/wgap);
        ry+=round(rel->y()/hgap);
    }
    focusGrid[std::pair<int,int>(rx,ry)].push_back(item);
    // displayPos.insert(f.id,{rx,ry});
    item->displayPos=QPoint(rx,ry);
}
QPointF toCenter(const QPointF &p){
    return p+QPointF(focustree::itemW/2,focustree::itemH/2);
}
void focustree::addFocusPreqLine(const Focus &f){
    FocusItem *curr=items[f.id];
    curr->visiblePreqCount=0;
    foreach(const QVector<QString> &v,f.preReq){
        foreach(const QString &str,v){
            BrokenLine *l;
            if(v.size()==1)
                l = new SolidLine();
            else l = new DotLine();

            curr->preqLines.push_back(l);

            FocusItem *preq=items[str];
            preq->postItems.push_back(curr);
            curr->preqItems.push_back(preq);

            int dy=yQuery(preq->displayPos.y(),curr->displayPos.y(),curr->displayPos.x(),[](FocusItem* x){
                    return x->isVisible();
            });
            l->setTurnPoint(dy-1);

            QPointF p1 = toCenter(preq->pos());
            QPointF p2 = toCenter(curr->pos());
            l->setEnd(p2-p1);

            connect(curr,&FocusItem::hidden,l,&BrokenLine::hide);
            connect(preq,&FocusItem::hidden,l,&BrokenLine::hide);
            connect(curr,&FocusItem::shown,l,&BrokenLine::show);
            connect(preq,&FocusItem::shown,l,&BrokenLine::show);
            connect(curr,&FocusItem::moved,l,&BrokenLine::moveEnd);
            connect(preq,&FocusItem::moved,l,&BrokenLine::moveStart);
            connect(preq,&FocusItem::neededSelectSubtree,curr,&FocusItem::selectSubtree);

            treeScene->addItem(l);
            // l->proxy=proxy;

            if(p2.x()>=p1.x())
                l->setPos(QPointF(p1.x()-1,p1.y()));
            else
                l->setPos(QPointF(p2.x()-1,p1.y()));

            l->setZValue(-100);

            connect(preq,&FocusItem::hidden,curr,&FocusItem::preqHidden);
            connect(preq,&FocusItem::shown,curr,&FocusItem::preqShown);

            if(preq->isVisible()){
                curr->visiblePreqCount++;
            }else{
                l->hide();
            }
        }
    }
}
void focustree::addFocusExLine(const Focus &f){
    qDebug()<<f.id;
    if(!items[f.id]->isVisible())return;
    foreach(const QString &str,f.excl){
        if(!items[str]->isVisible())continue;

        const Focus &t=focusModel->data(str);
        FocusItem *curr=items[f.id],*excl=items[str];

        curr->exclItems.push_back(excl);
        excl->exclItems.push_back(curr);

        if(xQuery(curr->displayPos.x(),excl->displayPos.x(),curr->displayPos.y(),[&](FocusItem *x){
            if(!x->isVisible())return false;
            return std::find(f.excl.begin(),f.excl.end(),x->focusid)!=f.excl.end()
                   &&std::find(t.excl.begin(),t.excl.end(),x->focusid)!=t.excl.end();
        }))continue;

        if(getExclLine(curr,excl))continue;

        LineItem *l = new ExclusiveLine();
        QPointF p1 = toCenter(curr->pos());
        QPointF p2 = toCenter(excl->pos());
        if(p1.x()>p2.x())std::swap(p1,p2);

        l->setEnd(p2-p1);
        connect(curr,&FocusItem::hidden_with_id,this,&focustree::updateExclusiveFocus,Qt::UniqueConnection);
        connect(excl,&FocusItem::hidden_with_id,this,&focustree::updateExclusiveFocus,Qt::UniqueConnection);
        connect(curr,&FocusItem::shown_with_id,this,&focustree::updateExclusiveFocus,Qt::UniqueConnection);
        connect(excl,&FocusItem::shown_with_id,this,&focustree::updateExclusiveFocus,Qt::UniqueConnection);

        treeScene->addItem(l);

        if(p2.x()>=p1.x())
            l->setPos(QPointF(p1.x(),p1.y()-ExclusiveLine::h/2));
        else
            l->setPos(QPointF(p2.x(),p1.y()-ExclusiveLine::h/2));
        l->setZValue(-100);
        exclLines.insert({curr,excl},l);
        qDebug()<<"added excl line:"<<f.id<<str;
    }
}

FocusModel *focustree::model(){
    return focusModel;
}

void focustree::setPreqFrames(const QString &str){
    const Focus &f=focusModel->data(str);
    const QVector<QVector<QString>> &v=f.preReq;
    for(unsigned i=0;i<v.size();i++)
        foreach(const QString &str,v[i]){
            items[str]->setFrame(focustree::colorList[i]);
            items[str]->reveal();
        }

    const QVector<QString> &m=f.excl;
    foreach(const QString &str,m){
        items[str]->setFrame(0xff0000); // 红色代表互斥国策
        items[str]->reveal();
    }
}

// 导入
void focustree::on_actionopen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "所有文件 (*.*);;文本文件 (*.txt);;图像文件 (*.png *.jpg)");
    if (!fileName.isEmpty()) {
        qDebug() << "选择的文件:" << fileName;
    }else return;
    ObjPointer node=Parser::parseFile(fileName);
    //node.prt();
    bool status=this->focusModel->init(node);
    if(!status){
        qDebug()<<"国策树读取失败，文件中存在错误";
    }

    this->treeScene->clear();
    this->items.clear();
    this->focusGrid.clear();
    this->exclLines.clear();

    foreach(const Focus& f,this->focusModel->allData()){
        this->addFocusItem(f);
    }

    foreach(const Focus &f,this->focusModel->allData()){
        this->addFocusPreqLine(f);
    }

    foreach(const Focus &f,this->focusModel->allData()){
        if(f.excl.size())
            this->addFocusExLine(f);
    }
}

void focustree::removeFocusExLine(FocusItem *item){
    QVector<QPair<FocusItem*,FocusItem*>> toRemove;
    for(auto it=exclLines.begin();it!=exclLines.end();++it){
        if(it.key().first==item || it.key().second==item)toRemove.push_back(it.key());
    }
    foreach(const auto &p,toRemove){
        treeScene->removeItem(exclLines[p]);
        delete exclLines[p];
        exclLines.remove(p);
    }
    item->exclItems.clear();
}

void focustree::updateExclusiveFocus(const QString &name){
    //qDebug()<<name<<"is hidden or shown";
    const Focus &f=this->focusModel->data(name);
    foreach(const QString &str,f.excl){
        removeFocusExLine(items[str]);
        addFocusExLine(focusModel->data(str));
    }
}

bool focustree::xQuery(int x1,int x2,int y,std::function<bool(FocusItem*)> f)const{
    if(x1>x2)std::swap(x1,x2);
    for(int i=x1+1;i<x2;i++)
        if(focusGrid.count(std::pair<int,int>(i,y))&&focusGrid[std::pair<int,int>(i,y)].size()){
            foreach(FocusItem *p,(focusGrid[std::pair<int,int>(i,y)]))
                if(f(p))return true;
        }
    return false;
}

int focustree::yQuery(int y1,int y2,int x,std::function<bool(FocusItem*)> f)const{
    if(y1>y2)std::swap(y1,y2);
    for(int i=y1+1;i<y2;i++)
        if(focusGrid.count(std::pair<int,int>(x,i))&&focusGrid[std::pair<int,int>(x,i)].size()){
            foreach(FocusItem *p,(focusGrid[std::pair<int,int>(x,i)]))
                if(!f(p))return i-y1;
        }else return i-y1;
    return y2-y1;
}

LineItem* focustree::getExclLine(FocusItem *a,FocusItem *b)const{
    if(exclLines.contains({a,b}))return exclLines.value({a,b});
    else return exclLines.value({b,a});
}

void focustree::resizeEvent(QResizeEvent *evt){
    this->treeView->setMinimumWidth(int(evt->size().width()*0.65));
    QMainWindow::resizeEvent(evt);
}

void focustree::revealFocus(const QString &id){
    static FocusItem *prevItem=nullptr;
    if(!id.size()){
        if(prevItem)prevItem->unreveal();
        prevItem=nullptr;
        return;
    }
    FocusItem *item=items[id];
    if(item!=prevItem && prevItem!=nullptr)prevItem->unreveal();
    if(!item)return;
    item->reveal();
    prevItem=item;
}

bool focustree::noPreqHidden(const QString &id){
    foreach(const QVector<QString> &v,focusModel->data(id).preReq){
        foreach(const QString &str,v){
            if(!items[str]->isVisible())return false;
        }
    }
    return true;
}

void focustree::on_action_redo_triggered()
{
    uManager->redo();
}


void focustree::on_action_undo_triggered()
{
    uManager->undo();
}

void focustree::handleFocusMove(const QVector<QString> &ids,int dx,int dy){
    foreach(const QString &id,ids){
        FocusItem *item=items[id];
        QPointF npos=item->pos()+QPointF(dx*wgap,dy*hgap);

        item->setPos(npos);

        QVector<FocusItem*> &v=focusGrid[{item->displayPos.x(),item->displayPos.y()}];
        v.erase(std::find(v.cbegin(),v.cend(),item));
        focusGrid[{item->displayPos.x()+dx,item->displayPos.y()+dy}].push_back(item);
        item->displayPos+=QPoint(dx,dy);
        emit item->moved(dx*wgap,dy*hgap);
    }
    foreach(const QString &id,ids){
        this->updateExclusiveFocus(id);
        FocusItem *item=items[id];
        // 暂时这么搞
        for(unsigned i=0;i<item->preqItems.size();i++){
            FocusItem *preq=item->preqItems[i];
            int dy=yQuery(preq->displayPos.y(),item->displayPos.y(),item->displayPos.x(),[](FocusItem* x){
                return x->isVisible();
            });
            item->preqLines[i]->setTurnPoint(dy-1);
        }
    }
}

void focustree::on_actiondaochu_triggered()
{

}

void focustree::batchMoveFocus(const QSet<FocusItem*> items,int dx,int dy){
    QVector<QString> idx;
    foreach(FocusItem *item,items)idx.push_back(item->focusid);
    focusModel->batchMoveFocus(idx,dx,dy);
}

void focustree::addFocusPrereq(const QString &baseId,const QString &targetId,int group){
    focusModel->addFocusPreq(baseId,targetId,group);
}
void focustree::removeFocusPrereq(const QString &baseId,const QString &targetId){
    focusModel->removeFocusPreq(baseId,targetId);
}
void focustree::addFocusExcl(const QString &baseId,const QString &targetId){
    focusModel->addFocusExcl(baseId,targetId);
}
void focustree::removeFocusExcl(const QString &baseId,const QString &targetId){
    focusModel->removeFocusExcl(baseId,targetId);
}

void focustree::removeFocusPreqLine(FocusItem *item){
    foreach(BrokenLine *l,item->preqLines){
        treeScene->removeItem(l);
        l->deleteLater();
    }
    foreach(FocusItem *preq,item->preqItems){
        disconnect(preq,&FocusItem::neededSelectSubtree,item,&FocusItem::selectSubtree);
        disconnect(preq,&FocusItem::hidden,item,&FocusItem::preqHidden);
        disconnect(preq,&FocusItem::shown,item,&FocusItem::preqShown);
        preq->postItems.remove(preq->postItems.indexOf(item));
    }
    item->preqItems.clear();
    item->preqLines.clear();
}

void focustree::handleFocusPreqUpdated(const QString &id){
    emit treeView->frameResetNeeded();
    removeFocusPreqLine(items[id]);
    addFocusPreqLine(focusModel->data(id));
    setPreqFrames(id);
}

void focustree::handleFocusExclUpdated(const QString &id){
    emit treeView->frameResetNeeded();
    removeFocusExLine(items[id]);
    addFocusExLine(focusModel->data(id));
}
