#include "hoilangedit.h"
#include <QTextBlock>



HoiTextEdit::HoiTextEdit(QWidget *parent):QTextEdit(parent){
    QFont font("Consolas", 15);
    setFont(font);

    QFontMetrics metrics(font);
    int tabStop = 4 * metrics.horizontalAdvance(' ');
    setTabStopDistance(tabStop);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    isProtected=false;
}
void HoiTextEdit::updateSize(){
    setFixedHeight(document()->size().rheight());
    updateGeometry();
}
void HoiTextEdit::setProtected(bool b){
    isProtected=b;
    setReadOnly(b);
    if(b){
        setStyleSheet("background-color: lightgray;");
    }else{
        setStyleSheet("background-color: white;");
    }
}
HoiLangEdit::HoiLangEdit(QWidget *parent):QScrollArea(parent) {
    w=new QWidget();
    layout=new QVBoxLayout(w);
    w->setLayout(layout);
}
void safeAppend(QTextEdit *edit,const QString &text){
    edit->textCursor().insertText(text);
}
HoiTextEdit *HoiLangEdit::appendLangText(AttrPointer attr,HoiTextEdit *edit,bool inheritedP,QString path,int w){
    bool prot;
    if(inheritedP)prot = true;
    else prot = view->protector.check(path);
    if(prot && ((edit==nullptr) || (edit!=nullptr && !edit->isProtected))){
        HoiTextEdit *currEdit=new HoiTextEdit();
        currEdit->setProtected(true);
        layout->addWidget(currEdit);
        ttfa.push_back(currEdit);
        edit=currEdit;
    }
    if(edit==nullptr || (edit!=nullptr && !prot && edit->isProtected)){
        edit=new HoiTextEdit();
        layout->addWidget(edit);
        ttfa.push_back(edit);
    }
    for(int i=0;i<w;i++)safeAppend(edit,"\t");
    safeAppend(edit,attr->key);
    if(attr->op!=nullptr){
        safeAppend(edit," "+attr->op+" ");
        if(attr->value->isObject())safeAppend(edit,"{\n");
        edit = appendLangText(attr->value,edit,prot,path,w+1);
        if(attr->value->isObject()){
            for(int i=0;i<w;i++)safeAppend(edit,"\t");
            safeAppend(edit,"}\n");
        }
    }
    return edit;
}
HoiTextEdit *HoiLangEdit::appendLangText(ObjPointer obj,HoiTextEdit *edit,bool inheritedP,QString path,int w){
    if(obj->isString()){
        safeAppend(edit,obj->content);
    }else{
        foreach(const AttrPointer &ptr,obj->attributes){
            edit=appendLangText(ptr,edit,inheritedP,path+"."+ptr->key,w);
            safeAppend(edit,"\n");
        }
    }
    return edit;
}
void HoiLangEdit::init(AttrPointer attr,const QString &header){
    foreach(HoiTextEdit *ptr,ttfa){
        layout->removeWidget(ptr);
        ptr->deleteLater();
    }
    ttfa.clear();
    appendLangText(attr,nullptr,false,header);
    this->setWidget(w);
    foreach(HoiTextEdit *edit,ttfa){
        edit->document()->adjustSize();
        edit->updateSize();
        connect(edit,&QTextEdit::textChanged,edit,&HoiTextEdit::updateSize);
        connect(edit,&QTextEdit::textChanged,this,&HoiLangEdit::textChanged);
    }
}
QString HoiLangEdit::plainText(){
    QString ret;
    foreach(HoiTextEdit *edit,ttfa){
        ret.append(edit->toPlainText());
    }
    return ret;
}
