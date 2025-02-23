#include "hoilangview.h"
#include "ui_hoilangview.h"
#include "parser.h"
#include <QResizeEvent>
#include <QMessageBox>

class EditPropertyAction: public BaseAction{
public:
    void execute() override{
        view->save(uniquePath,curr);
    }
    ActionPtr getReversedAction()const override{
        return newAction<EditPropertyAction>(view,uniquePath,curr,prev);
    }
    EditPropertyAction(HoiLangView *v,const QVector<int> &path,AttrPointer _prev,AttrPointer _curr)
        :uniquePath(path){
        view=v;prev=_prev;curr=_curr;
    }
    QString name()const override{return "edit";}

private:
    HoiLangView *view;
    QVector<int> uniquePath;
    AttrPointer prev,curr;
};

EditProtector::EditProtector(std::initializer_list<QString> l):protectedTerms(l){}

bool EditProtector::check(const QString &x){
    return protectedTerms.contains(x);
}

EditProtector::EditProtector(){}

EditProtector defaultFocusProtector({
    "focus.x",
    "focus.y",
    "focus.prerequisite",
    "focus.mutually_exclusive",
    "focus.relative_position_id",
    "focus.id",
    "focus.icon"
});


HoiLangItem::HoiLangItem(HoiLangItem *parent,AttrPointer _ptr,int rowId):QStandardItem(_ptr->key){
    ptr=_ptr;
    if(parent!=nullptr){
        path=parent->path+"."+ptr->key;
        uniquePath=parent->uniquePath;
    }else {
        path=ptr->key;
    }
    uniquePath.push_back(rowId);
}

HoiLangView::HoiLangView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HoiLangView)
{
    ui->setupUi(this);

    uManager=new UndoManager(this);

    ui->splitter_2->setCollapsible(0,false);
    ui->splitter_2->setCollapsible(1,false);
    //ui->splitter->setCollapsible(0,false);
    //ui->splitter->setCollapsible(0,false);

    ui->langEdit->resize(int(size().width()*0.6),size().height());
    ui->langEdit->setMinimumSize({800,600});
    ui->langEdit->view=this;

    ui->buttonBox->hide();

    this->itemModel=new QStandardItemModel(this);
    ui->treeView->setModel(itemModel);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);
    ui->treeView->setFont(QFont("consolas",12));

    connect(ui->treeView,&QAbstractItemView::clicked,this,&HoiLangView::handleClick);
    connect(ui->langEdit,&HoiLangEdit::textChanged,this,&HoiLangView::handleChange);
    connect(ui->buttonBox,&QDialogButtonBox::clicked,this,&HoiLangView::handleButtonClick);

    ui->treeView->setStyleSheet(R"(
        QTreeView:disabled {
            background-color: lightgray;
            color: darkgray;
        }
    )");
}

HoiLangView::~HoiLangView()
{
    delete ui;
}

void HoiLangView::init(AttrPointer attr){
    containedObj=attr;
    HoiLangItem *item=new HoiLangItem(nullptr,attr,0);
    itemModel->invisibleRootItem()->removeRows(0,itemModel->invisibleRootItem()->rowCount());
    itemModel->invisibleRootItem()->appendRow(item);
    recursivelyBuild(attr->value,item);
    handleClick(itemModel->indexFromItem(item));
}

void HoiLangView::handleChange(){
    ui->buttonBox->show();
    ui->treeView->setDisabled(true);
}

void HoiLangView::recursivelyBuild(ObjPointer obj,HoiLangItem *parent){
    if(obj->isString()){
        return;
    }else{
        for(AttrPointer &attr:obj->attributes){
            HoiLangItem *item=new HoiLangItem(parent,attr,parent->rowCount());
            if(attr->hasValue())
                recursivelyBuild(attr->value,item);
            parent->appendRow(item);
        }
    }
}

void HoiLangView::handleClick(const QModelIndex &curr){
    if(curr.isValid()){
        HoiLangItem *item=dynamic_cast<HoiLangItem*>(itemModel->itemFromIndex(curr));
        assert(item);
        ui->langEdit->init(item->ptr,item->path,protector.checkPrefix(item->path));
        editingItem=item;
    }
    ui->buttonBox->hide();
    ui->treeView->setDisabled(false);
}

void HoiLangView::resizeEvent(QResizeEvent *evt){
    ui->splitter_2->resize(evt->size());
}
void HoiLangView::save(const QVector<int> &path,AttrPointer repl){
    if(!path.size())return;
    QStandardItem *prev=itemModel->invisibleRootItem(),*sp=nullptr;
    foreach(int s,path){
        HoiLangItem *x=dynamic_cast<HoiLangItem*>(prev->child(s));
        sp=prev;
        prev=x;
    }
    HoiLangItem *ex=dynamic_cast<HoiLangItem*>(prev);
    HoiLangItem *item=new HoiLangItem(dynamic_cast<HoiLangItem*>(sp),editingItem->ptr,editingItem->uniquePath.back());

    if(repl->hasValue())
        recursivelyBuild(repl->value,item);

    ex->ptr->key = repl->key;
    if(repl->hasValue()){
        ex->ptr->op  = repl->op;
        ex->ptr->value = repl->value;
    }
    for(int i=0;i<sp->rowCount();i++){
        if(sp->child(i)==ex){
            sp->setChild(i,item);
            break;
        }
    }
    if(editingItem!=nullptr){
        editingItem=item;
        ui->langEdit->init(editingItem->ptr,editingItem->path,protector.checkPrefix(editingItem->path));
    }
}
void HoiLangView::handleButtonClick(QAbstractButton *btn){
    if(btn==(QAbstractButton*)ui->buttonBox->button(QDialogButtonBox::Save)){
        ObjPointer ptr=Parser::parse(ui->langEdit->plainText());
        if(ptr == nullptr){
            QMessageBox::warning(this, "语法错误", "文本格式错误，请检查并重试");
            return;
        }else if(ptr->attributes.size()>1||ptr->attributes.size()==0){
            QMessageBox::warning(this, "语法错误", "应只存在一个顶层对象，请检查并重试");
            return;
        }
        if(ptr!=nullptr && ptr->attributes.size()){
            uManager->addAction(newAction<EditPropertyAction>(this,editingItem->uniquePath,editingItem->ptr->duplicate(),ptr->attributes[0]));
            save(editingItem->uniquePath,ptr->attributes[0]);
        }
    }

    ui->langEdit->init(editingItem->ptr,editingItem->path,protector.checkPrefix(editingItem->path));
    ui->buttonBox->hide();
    ui->treeView->setDisabled(false);

}

bool EditProtector::checkPrefix(const QString &x){
    int dotIndex = x.lastIndexOf('.');
    while (dotIndex != -1) {
        QString parent = x.left(dotIndex);
        if (protectedTerms.contains(parent)) {
            return true;
        }
        dotIndex = x.lastIndexOf('.', dotIndex - 1);
    }
    return false;
}

void HoiLangView::undo(){
    uManager->undo();
}
void HoiLangView::redo(){
    uManager->redo();
}
