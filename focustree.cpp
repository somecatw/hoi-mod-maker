#include "focustree.h"
#include "ui_focustree.h"
#include "focuseditor.h"
#include "parser.h"
#include "focusitem.h"
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

QGraphicsProxyWidget* focustree::getProxy(const QString& id) const{
    if(!proxies.count(id))return nullptr;
    return proxies.value(id);
}

void focustree::addFocusItem(const Focus& f){
    if(proxies.count(f.id))return;

    FocusItem *item = new FocusItem();
    item->setup(f.id,this);

    connect(this,&focustree::resetSelection,item,&FocusItem::deSelect);

    QGraphicsProxyWidget *proxy = this->treeScene->addWidget(item);
    proxy->setPos({wgap*f.x, hgap*f.y});
    proxies.insert(f.id,proxy);

    if(f.relativeId.size()){
        addFocusItem(this->focusModel->data(f.relativeId));
        QGraphicsProxyWidget *rel = getProxy(f.relativeId);
        proxy->setPos({wgap*f.x+rel->x(),hgap*f.y+rel->y()});
    }
}
QPointF toCenter(const QPointF &p){
    return p+QPointF(focustree::itemW/2,focustree::itemH/2);
}
void focustree::addFocusPreqLine(const Focus &f){
    foreach(const QVector<QString> &v,f.preReq){
        foreach(const QString &str,v){
            LineWidget *l;
            if(v.size()==1)
                l = new SolidLine();
            else l = new DotLine();
            QPointF p1 = toCenter(getProxy(str)->pos());
            QPointF p2 = toCenter(getProxy(f.id)->pos());
            l->setEnd(p2-p1);
            auto proxy=treeScene->addWidget(l);

            if(p2.x()>=p1.x())
                proxy->setPos(QPointF(p1.x()-1,p1.y()));
            else
                proxy->setPos(QPointF(p2.x()-1,p1.y()));

            proxy->setZValue(-100);
        }
    }
}
void focustree::addFocusExLine(const Focus &f){
    QVector<QGraphicsProxyWidget*> v;
    foreach(const QString &str,f.excl){
        if(this->exLineDeployed.count(str))return;
        v.push_back(this->getProxy(str));
        this->exLineDeployed.insert(str,true);
    }
    v.push_back(getProxy(f.id));
    this->exLineDeployed.insert(f.id,true);

    std::sort(v.begin(),v.end(),[](QGraphicsProxyWidget *a,QGraphicsProxyWidget *b){
        return a->x()<b->x();
    });

    qDebug()<<v.size();
    for(unsigned i=0;i+1<v.size();i++){
        LineWidget *l = new ExclusiveLine();
        QPointF p1 = toCenter(v[i]->pos());
        QPointF p2 = toCenter(v[i+1]->pos());
        l->setEnd(p2-p1);
        auto proxy=treeScene->addWidget(l);
        if(p2.x()>=p1.x())
            proxy->setPos(QPointF(p1.x(),p1.y()-ExclusiveLine::h/2));
        else
            proxy->setPos(QPointF(p2.x(),p1.y()-ExclusiveLine::h/2));
        proxy->setZValue(-100);
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
            QGraphicsProxyWidget *proxy = getProxy(str);
            FocusItem *item = ((FocusItem*)(proxy->widget()));
            item->setFrame(focustree::colorList[i]);
        }

    const QVector<QString> &m=f.excl;
    foreach(const QString &str,m){
        QGraphicsProxyWidget *proxy = getProxy(str);
        FocusItem *item = ((FocusItem*)(proxy->widget()));
        item->setFrame(0xff0000); // 红色代表互斥国策
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
    this->exLineDeployed.clear();

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
}
