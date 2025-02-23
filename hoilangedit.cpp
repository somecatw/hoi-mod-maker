#include "hoilangedit.h"
#include <QTextBlock>


// gpt 写的
HoiLangHighlighter::HoiLangHighlighter(QTextDocument *parent):QSyntaxHighlighter(parent){
    QTextCharFormat keyFormat;
    keyFormat.setForeground(Qt::blue);
    keyFormat.setFontWeight(QFont::Bold);

    // Define the format for values
    QTextCharFormat valueFormat;
    valueFormat.setForeground(Qt::darkBlue);

    QTextCharFormat quotedValueFormat;
    quotedValueFormat.setForeground(Qt::darkGreen);

    // Define the rules for keys and values
    HighlightingRule keyRule;
    keyRule.pattern = QRegularExpression(R"([^<=>{}\t\r\n \"]+)");
    keyRule.format = keyFormat;
    highlightingRules.append(keyRule);

    HighlightingRule valueRule;
    valueRule.pattern = QRegularExpression(R"(([<=>]\s*)[^<=>{}\t\r\n "]+)");
    valueRule.format = valueFormat;
    highlightingRules.append(valueRule);

    HighlightingRule quotedValueRule;
    quotedValueRule.pattern = QRegularExpression(R"(([<=>]\s*)\"[^\"]*\")");
    quotedValueRule.format = quotedValueFormat;
    highlightingRules.append(quotedValueRule);
}

void HoiLangHighlighter::highlightBlock(const QString &text) {
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpression expression(rule.pattern);
        QRegularExpressionMatchIterator matchIterator = expression.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            int startIndex = match.capturedStart();
            int length = match.capturedLength();
            if (rule.pattern.pattern().startsWith(R"(([<=>]\s*))")) {
                startIndex += match.captured(1).length(); // Move start index past the equals sign
                length -= match.captured(1).length(); // Reduce length to exclude the equals sign
            }

            setFormat(startIndex, length, rule.format);
        }
    }
}

HoiTextEdit::HoiTextEdit(QWidget *parent):QTextEdit(parent){
    QFont font("Consolas", 15);
    setFont(font);

    QFontMetrics metrics(font);
    int tabStop = 4 * metrics.horizontalAdvance(' ');
    setTabStopDistance(tabStop);

    setLineWrapMode(QTextEdit::NoWrap);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    isProtected=false;

    new HoiLangHighlighter(document());
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
HoiTextEdit *HoiLangEdit::addEdit(bool p){
    HoiTextEdit *ret=new HoiTextEdit();
    if(p)ret->setProtected(p);
    layout->addWidget(ret);
    ttfa.push_back(ret);
    return ret;
}
HoiTextEdit *HoiLangEdit::appendLangText(AttrPointer attr,HoiTextEdit *edit,bool inheritedP,QString path,int w){
    bool prot;
    if(inheritedP)prot = true;
    else prot = view->protector.check(path);
    if(prot && ((edit==nullptr) || (edit!=nullptr && !edit->isProtected))){
        edit=addEdit(true);
    }
    if(edit==nullptr || (edit!=nullptr && !prot && edit->isProtected)){
        edit=addEdit(false);
    }
    for(int i=0;i<w;i++)safeAppend(edit,"\t");
    safeAppend(edit,attr->key);
    if(attr->op!=nullptr){
        safeAppend(edit," "+attr->op+" ");
        if(attr->value->isObject())safeAppend(edit,"{\n");
        edit = appendLangText(attr->value,edit,prot,path,w+1);
        if(edit->isProtected!=prot)
            edit=addEdit(prot);
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

void HoiTextEdit::keyPressEvent(QKeyEvent *evt){
    if(evt->key() == Qt::Key_Return || evt->key()==Qt::Key_Enter){
        auto cursor = textCursor();
        QString currentText = cursor.block().text();
        cursor.insertText("\n" + getIndentation(currentText));
        setTextCursor(cursor);
    }else QTextEdit::keyPressEvent(evt);
}
QString HoiTextEdit::getIndentation(const QString &str)const{
    QString ret;
    foreach(QChar c,str){
        if(c.isSpace()||c=='\t')ret+=c;
        else break;
    }
    return ret;
}

void HoiLangEdit::init(AttrPointer attr,const QString &header,bool headProtected){
    foreach(HoiTextEdit *ptr,ttfa){
        layout->removeWidget(ptr);
        ptr->deleteLater();
    }
    while(layout->count()){
        auto *item=layout->takeAt(0);
        delete item;
    }
    ttfa.clear();
    appendLangText(attr,nullptr,headProtected,header);
    this->setWidget(w);
    foreach(HoiTextEdit *edit,ttfa){
        edit->document()->adjustSize();
        edit->updateSize();
        edit->document()->clearUndoRedoStacks();
        connect(edit,&QTextEdit::textChanged,edit,&HoiTextEdit::updateSize);
        connect(edit,&QTextEdit::textChanged,this,&HoiLangEdit::textChanged);
        connect(edit,&QTextEdit::cursorPositionChanged,this,&HoiLangEdit::ensureCursorVisible);
    }
    layout->addStretch();
}
QString HoiLangEdit::plainText(){
    QString ret;
    foreach(HoiTextEdit *edit,ttfa){
        ret.append(edit->toPlainText());
    }
    return ret;
}
void HoiLangEdit::ensureCursorVisible(){
    foreach(HoiTextEdit *edit,ttfa){
        if(edit->hasFocus()){
            QRect rect=edit->cursorRect();
            rect.moveTopLeft(edit->mapTo(w,rect.topLeft()));
            ensureVisible(rect.left(),rect.top());
        }else edit->clearSelection();
    }
}

void HoiTextEdit::clearSelection(){
    auto cursor=this->textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
}
