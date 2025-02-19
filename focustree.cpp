#include "focustree.h"
#include "ui_focustree.h"
#include "focuseditor.h"
#include "parser.h"
#include "lineitems.h"

focustree::focustree(QWidget *parent)
    : QMainWindow(parent)
    , focustreeui(new Ui::focustree)
{
    focustreeui->setupUi(this);
    treeScene = new QGraphicsScene(this);
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

QGraphicsProxyWidget* focustree::getProxy(const QString& id) const{
    if(!proxies.count(id))return nullptr;
    return proxies.value(id);
}

void focustree::showFocus(const QString &id){
    toItem(getProxy(id))->show();
    uManager->addAction(newAction<ShowFocusAction>(toItem(getProxy(id))));
}

void focustree::addFocusItem(const Focus& f){
    if(proxies.count(f.id))return;

    FocusItem *item = new FocusItem();
    item->setup(f.id,this);
    int rx=f.x,ry=f.y;

    connect(this->treeView,&FocusTreeView::cleared,item,&FocusItem::deSelect);
    connect(item,&FocusItem::hidden_with_id,this,&focustree::focusHidden);
    connect(item,&FocusItem::shown_with_id,this,&focustree::focusShown);
    connect(this->treeView,&FocusTreeView::frameResetNeeded,item,&FocusItem::hideFrame);
    connect(item,&FocusItem::implicitlySelected,this->treeView,&FocusTreeView::select);

    QGraphicsProxyWidget *proxy = this->treeScene->addWidget(item);
    proxy->setPos({wgap*f.x, hgap*f.y});
    proxies.insert(f.id,proxy);

    if(f.relativeId.size()){
        addFocusItem(this->focusModel->data(f.relativeId));
        QGraphicsProxyWidget *rel = getProxy(f.relativeId);
        proxy->setPos({wgap*f.x+rel->x(),hgap*f.y+rel->y()});
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
    foreach(const QVector<QString> &v,f.preReq){
        FocusItem *curr=toItem(getProxy(f.id));
        foreach(const QString &str,v){
            BrokenLine *l;
            if(v.size()==1)
                l = new SolidLine();
            else l = new DotLine();

            FocusItem *preq=toItem(getProxy(str));
            preq->postItems.push_back(curr);
            curr->preqItems.push_back(preq);

            if(yQuery(preq->displayPos.y(),curr->displayPos.y(),curr->displayPos.x(),[](FocusItem* x){
                    return x->isVisible();
                }))l->setType(1);

            QPointF p1 = toCenter(getProxy(str)->pos());
            QPointF p2 = toCenter(getProxy(f.id)->pos());
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
        }
        curr->visiblePreqCount+=v.size();
    }
}
void focustree::addFocusExLine(const Focus &f){
    qDebug()<<f.id;
    if(!toItem(getProxy(f.id))->isVisible())return;
    foreach(const QString &str,f.excl){
        if(!toItem(getProxy(str))->isVisible())continue;

        const Focus &t=focusModel->data(str);
        FocusItem *curr=toItem(getProxy(f.id)),*excl=toItem(getProxy(str));

        curr->exclItems.push_back(excl);
        excl->exclItems.push_back(curr);

        if(xQuery(curr->displayPos.x(),excl->displayPos.x(),curr->displayPos.y(),[&](FocusItem *x){
            if(!x->isVisible())return false;
            return std::find(f.excl.begin(),f.excl.end(),x->focusid)!=f.excl.end()
                   &&std::find(t.excl.begin(),t.excl.end(),x->focusid)!=t.excl.end();
        }))continue;

        if(getExclLine(curr,excl))continue;

        LineItem *l = new ExclusiveLine();
        QPointF p1 = toCenter(getProxy(f.id)->pos());
        QPointF p2 = toCenter(getProxy(str)->pos());
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
/*
void focustree::handleSelection(FocusItem *item,bool shiftPressed){
    if(!shiftPressed)emit resetSelection();
    setPreqFrames(item->focusid);
    treeView->handleSelection(item,shiftPressed);
}
*/
void focustree::setPreqFrames(const QString &str){
    const Focus &f=focusModel->data(str);
    const QVector<QVector<QString>> &v=f.preReq;
    for(unsigned i=0;i<v.size();i++)
        foreach(const QString &str,v[i]){
            toItem(getProxy(str))->setFrame(focustree::colorList[i]);
        }

    const QVector<QString> &m=f.excl;
    foreach(const QString &str,m){
        toItem(getProxy(str))->setFrame(0xff0000); // 红色代表互斥国策
    }
}

// 导入
void focustree::on_actionopen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "所有文件 (*.*);;文本文件 (*.txt);;图像文件 (*.png *.jpg)");
    if (!fileName.isEmpty()) {
        qDebug() << "选择的文件:" << fileName;
    }else return;
    AstNode node=Parser::parse(fileName);
    //node.prt();
    bool status=this->focusModel->init(node);
    if(!status){
        qDebug()<<"国策树读取失败，文件中存在错误";
    }

    this->treeScene->clear();
    this->proxies.clear();
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

void focustree::removeFocusExLine(const Focus &f){
    foreach(const QString &str,f.excl){
        LineItem *w;
        FocusItem *a=toItem(getProxy(f.id)),
            *b=toItem(getProxy(str));
        if((w=getExclLine(a,b))){
            this->treeScene->removeItem(w);
            delete w;
            if(exclLines.contains({a,b}))
                exclLines.remove({a,b});
            else exclLines.remove({b,a});
        }
    }
}

void focustree::updateExclusiveFocus(const QString &name){
    //qDebug()<<name<<"is hidden or shown";
    const Focus &f=this->focusModel->data(name);
    foreach(const QString &str,f.excl){
        removeFocusExLine(focusModel->data(str));
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

bool focustree::yQuery(int y1,int y2,int x,std::function<bool(FocusItem*)> f)const{
    if(y1>y2)std::swap(y1,y2);
    for(int i=y1+1;i<y2;i++)
        if(focusGrid.count(std::pair<int,int>(x,i))&&focusGrid[std::pair<int,int>(x,i)].size()){
            foreach(FocusItem *p,(focusGrid[std::pair<int,int>(x,i)]))
                if(f(p))return true;
        }
    return false;
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
    FocusItem *item=toItem(getProxy(id));
    if(item!=prevItem && prevItem!=nullptr)prevItem->unreveal();
    if(!item)return;
    item->reveal();
    prevItem=item;
}

bool focustree::noPreqHidden(const QString &id){
    foreach(const QVector<QString> &v,focusModel->data(id).preReq){
        foreach(const QString &str,v){
            if(!toItem(getProxy(str))->isVisible())return false;
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

void focustree::handleFocusMove(const QString &id,int dx,int dy,bool isManual){
    auto *proxy=getProxy(id);
    FocusItem *item=toItem(proxy);
    QPointF npos=proxy->pos()+QPointF(dx*wgap,dy*hgap);

    proxy->setPos(npos);

    QVector<FocusItem*> &v=focusGrid[{item->displayPos.x(),item->displayPos.y()}];
    v.erase(std::find(v.cbegin(),v.cend(),item));
    focusGrid[{item->displayPos.x()+dx,item->displayPos.y()+dy}].push_back(item);
    item->displayPos+=QPoint(dx,dy);
    emit item->moved(dx*wgap,dy*hgap);

    this->updateExclusiveFocus(id);
    //qDebug()<<toItem(proxy)->displayPos<<" "<<dx<<dy;
}
