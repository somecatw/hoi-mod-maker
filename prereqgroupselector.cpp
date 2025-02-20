#include "prereqgroupselector.h"
#include "prereqgroupwidget.h"
#include "ui_prereqgroupselector.h"
#include "focustree.h"

PrereqGroupSelector::PrereqGroupSelector(const Focus &base,const Focus &target,focustree *_tree,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PrereqGroupSelector)
{
    ui->setupUi(this);
    tree=_tree;
    int index=0;
    foreach(const QVector<QString> &v,base.preReq){
        index++;
        PrereqGroupWidget *w=new PrereqGroupWidget(v,index,tree->colorList[index-1],this);
        QListWidgetItem *item=new QListWidgetItem();
        items.push_back(item);
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item,w);
        item->setSizeHint(w->size());
    }
    PrereqGroupWidget *w=new PrereqGroupWidget(QVector<QString>(),-1,tree->colorList[index],this);
    QListWidgetItem *item=new QListWidgetItem();
    items.push_back(item);
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,w);
    item->setSizeHint(w->size());
    this->setLayout(ui->verticalLayout);
    baseId=base.id;
    targetId=target.id;
}

PrereqGroupSelector::~PrereqGroupSelector()
{
    delete ui;
}

void PrereqGroupSelector::on_buttonBox_accepted()
{
    for(unsigned i=0;i<items.size();i++){
        QListWidgetItem *item=items[i];
        if(item->isSelected()){
            tree->addFocusPrereq(baseId,targetId,i);
            tree->uManager->addAction(newAction<AddPrereqAction>(tree,baseId,targetId,i));
        }
    }
    this->close();
}


void PrereqGroupSelector::on_buttonBox_rejected()
{
    this->close();
}

