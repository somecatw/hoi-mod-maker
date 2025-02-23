#include "hoilangview.h"
#include "ui_hoilangview.h"
#include "parser.h"
#include <QResizeEvent>

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


HoiLangItem::HoiLangItem(HoiLangItem *parent,AttrPointer _ptr):QStandardItem(_ptr->key){
    ptr=_ptr;
    if(parent!=nullptr)
        path=parent->path+"."+ptr->key;
    else path=ptr->key;
}

HoiLangView::HoiLangView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HoiLangView)
{
    ui->setupUi(this);

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
    HoiLangItem *item=new HoiLangItem(nullptr,attr);
    itemModel->invisibleRootItem()->removeRows(0,itemModel->invisibleRootItem()->rowCount());
    itemModel->invisibleRootItem()->appendRow(item);
    recursivelyBuild(attr->value,item);
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
            HoiLangItem *item=new HoiLangItem(parent,attr);
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
        ui->langEdit->init(item->ptr,item->path);
        editingItem=item;
    }
    ui->buttonBox->hide();
    ui->treeView->setDisabled(false);
}

void HoiLangView::resizeEvent(QResizeEvent *evt){
    ui->splitter_2->resize(evt->size());
}

void HoiLangView::handleButtonClick(QAbstractButton *btn){
    if(btn==(QAbstractButton*)ui->buttonBox->button(QDialogButtonBox::Save)){
        ObjPointer ptr=Parser::parse(ui->langEdit->plainText());
        if(ptr!=nullptr && ptr->attributes.size()){
            AttrPointer now=ptr->attributes[0];

            editingItem->ptr->key = now->key;
            editingItem->ptr->op  = now->op;
            editingItem->ptr->value = now->value;

            HoiLangItem *p=dynamic_cast<HoiLangItem*>(editingItem->parent());
            QStandardItem *sp=editingItem->parent();
            if(!sp)sp=itemModel->invisibleRootItem();
            HoiLangItem *item=new HoiLangItem(p,editingItem->ptr);
            recursivelyBuild(now->value,item);

            for(int i=0;i<sp->rowCount();i++){
                if(sp->child(i)==editingItem){
                    sp->setChild(i,item);
                    break;
                }
            }

            editingItem=item;
        }
    }

    ui->langEdit->init(editingItem->ptr,editingItem->path);
    ui->buttonBox->hide();
    ui->treeView->setDisabled(false);

}
