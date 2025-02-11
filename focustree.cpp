#include "focustree.h"
#include "ui_focustree.h"
#include "focuseditor.h"

focustree::focustree(QWidget *parent)
    : QMainWindow(parent)
    , focustreeui(new Ui::focustree)
{
    focustreeui->setupUi(this);
    treeScene = new QGraphicsScene(this);
    treeView = new FocusTreeView(treeScene, this);

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

// 导入
void focustree::on_actionopen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "所有文件 (*.*);;文本文件 (*.txt);;图像文件 (*.png *.jpg)");
    if (!fileName.isEmpty()) {
        qDebug() << "选择的文件:" << fileName;
    }else return;
    AstNode node=Parser::parse(fileName.toStdString());
    node.prt();
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
