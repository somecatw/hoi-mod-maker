#include "focustree.h"
#include "ui_focustree.h"
#include "focuseditor.h"
#include "parser.h"
#include "linewidgets.h"

focustree::focustree(QWidget *parent)
    : QMainWindow(parent)
    , focustreeui(new Ui::focustree)
{
    focustreeui->setupUi(this);
    treeScene = new QGraphicsScene(this);
    treeView = new FocusTreeView(treeScene, this);
    this->setCentralWidget(treeView);
    this->focusModel = new FocusModel(this);
}

focustree::~focustree()
{
    delete focustreeui;
}

void focustree::on_focusa_clicked()
{
    FocusEditor *a = new FocusEditor();
    a->show();
}

void FocusTreeView::contextMenuEvent(QContextMenuEvent *evt){
    QGraphicsItem *item=this->itemAt(mapFromGlobal(evt->globalPos()));
    if(!item)return;
    QGraphicsProxyWidget *w=dynamic_cast<QGraphicsProxyWidget*>(item);
    if(!w)return;
    FocusItem *t=dynamic_cast<FocusItem*>(w->widget());
    if(!t)return;
    selectedItem=t;
    menu->exec(evt->globalPos());
}

QGraphicsProxyWidget* focustree::getProxy(const QString& id) const{
    if(!proxies.count(id))return nullptr;
    return proxies.value(id);
}

void focustree::addFocusItem(const Focus& f){
    if(proxies.count(f.id))return;

    FocusItem *item = new FocusItem();
    item->setup(f.id,this);
    int rx=f.x,ry=f.y;

    connect(this,&focustree::resetSelection,item,&FocusItem::deSelect);

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
    displayPos.insert(f.id,{rx,ry});
}
QPointF toCenter(const QPointF &p){
    return p+QPointF(focustree::itemW/2,focustree::itemH/2);
}
FocusItem *toItem(QGraphicsProxyWidget *w){
    FocusItem *item = ((FocusItem*)(w->widget()));
    return item;
}
void focustree::addFocusPreqLine(const Focus &f){
    foreach(const QVector<QString> &v,f.preReq){
        foreach(const QString &str,v){
            BrokenLine *l;
            if(v.size()==1)
                l = new SolidLine();
            else l = new DotLine();

            if(yQuery(displayPos[str].y(),displayPos[f.id].y(),displayPos[f.id].x(),[](FocusItem* x){
                    return x->isVisible();
                }))l->setType(1);

            QPointF p1 = toCenter(getProxy(str)->pos());
            QPointF p2 = toCenter(getProxy(f.id)->pos());
            l->setEnd(p2-p1);

            connect(toItem(getProxy(f.id)),&FocusItem::hidden,l,&BrokenLine::hide);
            connect(toItem(getProxy(str)),&FocusItem::hidden,l,&BrokenLine::hide);

            auto proxy=treeScene->addWidget(l);

            if(p2.x()>=p1.x())
                proxy->setPos(QPointF(p1.x()-1,p1.y()));
            else
                proxy->setPos(QPointF(p2.x()-1,p1.y()));

            proxy->setZValue(-100);

            connect(toItem(getProxy(str)),&FocusItem::hidden,toItem(getProxy(f.id)),&FocusItem::preqHidden);
        }
        toItem(getProxy(f.id))->visiblePreqCount+=v.size();
    }
}
void focustree::addFocusExLine(const Focus &f){
    qDebug()<<f.id;
    foreach(const QString &str,f.excl){
        if(!toItem(getProxy(str))->isVisible())continue;
        if(!toItem(getProxy(f.id))->isVisible())continue;

        const Focus &t=focusModel->data(str);

        if(xQuery(displayPos[f.id].x(),displayPos[str].x(),displayPos[f.id].y(),[&](FocusItem *x){
            if(!x->isVisible())return false;
            return std::find(f.excl.begin(),f.excl.end(),x->focusid)!=f.excl.end()
                   &&std::find(t.excl.begin(),t.excl.end(),x->focusid)!=t.excl.end();
        }))continue;

        if(getExclLine(toItem(getProxy(f.id)),toItem(getProxy(str))))continue;

        LineWidget *l = new ExclusiveLine();
        QPointF p1 = toCenter(getProxy(f.id)->pos());
        QPointF p2 = toCenter(getProxy(str)->pos());
        if(p1.x()>p2.x())std::swap(p1,p2);

        l->setEnd(p2-p1);
        connect(toItem(getProxy(f.id)),&FocusItem::hidden_with_id,this,&focustree::updateExclusiveFocus);
        connect(toItem(getProxy(str)),&FocusItem::hidden_with_id,this,&focustree::updateExclusiveFocus);

        auto proxy=treeScene->addWidget(l);
        if(p2.x()>=p1.x())
            proxy->setPos(QPointF(p1.x(),p1.y()-ExclusiveLine::h/2));
        else
            proxy->setPos(QPointF(p2.x(),p1.y()-ExclusiveLine::h/2));
        proxy->setZValue(-100);
        exclLines.insert({toItem(getProxy(f.id)),toItem(getProxy(str))},proxy);
        qDebug()<<"added excl line:"<<f.id<<str;
    }
}

const FocusModel &focustree::model(){
    return *focusModel;
}
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
    this->displayPos.clear();

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

void FocusTreeView::wheelEvent(QWheelEvent *evt){
    const double fac = 1.1;
    if(evt->angleDelta().y()>0){
        scale(fac,fac);
    }else scale(1.0/fac,1.0/fac);
}

FocusTreeView::FocusTreeView(QGraphicsScene *scene, QWidget *parent)
    :QGraphicsView(scene,parent)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
    menu=new QMenu(this);
    QAction *act=new QAction("hide",this);
    menu->addAction(act);
    connect(act,&QAction::triggered,this,&FocusTreeView::hideFocus);
}

void FocusTreeView::hideFocus(){
    if(selectedItem){
        selectedItem->hide();
    }
}

void focustree::updateExclusiveFocus(const QString &name){
    const Focus &f=this->focusModel->data(name);
    foreach(const QString &str,f.excl){
        QGraphicsProxyWidget *w;
        FocusItem *a=toItem(getProxy(name)),
            *b=toItem(getProxy(str));
        if((w=getExclLine(a,b))){
            this->treeScene->removeItem(w);
            delete w;
            if(exclLines.contains({a,b}))
                exclLines.remove({a,b});
            else exclLines.remove({b,a});
        }
        addFocusExLine(focusModel->data(str));
    }
    //exLineDeployed[name]=false;
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

QGraphicsProxyWidget* focustree::getExclLine(FocusItem *a,FocusItem *b)const{
    if(exclLines.contains({a,b}))return exclLines.value({a,b});
    else return exclLines.value({b,a});
}
