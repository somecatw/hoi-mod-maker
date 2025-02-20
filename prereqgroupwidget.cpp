#include "prereqgroupwidget.h"
#include "qlabel.h"
#include "ui_prereqgroupwidget.h"

PrereqGroupWidget::PrereqGroupWidget(const QVector<QString> &v,int groupId,QColor color,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::prereqGroupWidget)
{
    ui->setupUi(this);

    QLabel *l=new QLabel();

    if(groupId!=-1)
        l->setText("第 "+QString::number(groupId)+" 组");
    else l->setText("新建一组");

    QFont f=l->font();
    f.setFamily("Microsoft YaHei UI");
    f.setPixelSize(15);
    f.setBold(true);
    l->setStyleSheet(QString("QLabel { color : %1; }").arg(color.name()));
    l->setFont(f);
    ui->verticalLayout->addWidget(l);

    foreach(const QString &str,v){
        QLabel *label=new QLabel();
        label->setText(str);
        label->adjustSize();
        label->resize(label->minimumSize());
        ui->verticalLayout->addWidget(label);
    }

    ui->verticalLayout->addWidget(new QLabel());

    ui->verticalLayout->setContentsMargins(0,0,0,0);
    this->setLayout(ui->verticalLayout);
    this->adjustSize();
}

PrereqGroupWidget::~PrereqGroupWidget()
{
    delete ui;
}
