#include "focusmodel.h"

AstNode request(const AstNode& node, QString key){
    const AstNode &ret = Parser::getFirst(node,key);
    if(ret.type.empty()){
        qDebug()<<"Error:"<<"key '"<<key<<"' missing in focus";
        return AstNode();
    }
    return ret;
}

FocusModel::FocusModel(QObject *parent)
    : QObject{parent}
{}

void FocusModel::moveFocus(const QString &index,int dx,int dy,bool isManual){
    size_t id=focusIndex.value(index);
    focuses[id].x+=dx;
    focuses[id].y+=dy;
    emit focusMoved(index,dx,dy,isManual);
}

bool FocusModel::init(const AstNode& node){
    const AstNode &tree = request(node,"focus_tree");

    if(tree.children.size()<=1||tree.children[1].children[1].type!="block"){
        qDebug()<<"Error: 'focus_tree' is not found or not a block";
        return false;
    }

    const AstNode& lst = tree.children[1].children[1];

    QVector<AstNode> v1=Parser::getAll(lst,"focus");
    QVector<AstNode> v2=Parser::getAll(lst,"shared_focus");
    v1.append(v2);

    qDebug()<<tree.children.size()<<" "<<v1.size()<<" "<<v2.size();

    focuses.clear();
    foreach(const AstNode& node,v1){
        Focus f=Focus(node);
        focusIndex.insert(f.id,focuses.size());
        focuses.push_back(f);
    }

    foreach(const Focus& f,focuses)
        if(!f.id.size())return false;
    return true;
}

Focus FocusModel::data(const QString& index) const {
    if(focusIndex.contains(index))
        return focuses[focusIndex.value(index)];
    return Focus();
}

const QVector<Focus>& FocusModel::allData() const{
    return focuses;
}
QVector<QString> getFocusPreqs(const AstNode &term){
    QVector<QString> ret;

    const QVector<AstNode> &focusNodes = Parser::getAll(Parser::getValue(term),"focus");
    foreach(const AstNode &node,focusNodes)
        ret.push_back(QString::fromStdString(Parser::getValue(node).content));

    return ret;
}

Focus::Focus(){}

Focus::Focus(const AstNode& node){
    if(node.children.size()<=1||node.children[1].children[1].type!="block"){
        if(node.children[0].content=="shared_focus")
            qDebug()<<"Unknown shared focus"<<Parser::getValue(node).content<<"found";
        else qDebug()<<"Error: 'focus' is not a block";
        return;
    }
    const AstNode &lst = Parser::getValue(node);
    const AstNode &idNode   = Parser::getValue(request(lst,"id"));
    const AstNode &iconNode = Parser::getValue(request(lst,"icon"));
    const AstNode &xNode    = Parser::getValue(request(lst,"x"));
    const AstNode &yNode    = Parser::getValue(request(lst,"y"));
    const AstNode &rIdNode  = Parser::getFirst(lst,"relative_position_id");
    const QVector<AstNode> &preqNodes = Parser::getAll(lst,"prerequisite");
    const AstNode &exNode   = Parser::getFirst(lst,"mutually_exclusive");

    if(idNode.type.empty()||iconNode.type.empty()||xNode.type.empty()||yNode.type.empty())return;

    this->id   = QString::fromStdString(idNode.content);
    this->icon = QString::fromStdString(iconNode.content);
    this->x    = QString::fromStdString(xNode.content).toInt();
    this->y    = QString::fromStdString(yNode.content).toInt();

    qDebug()<<"Found focus";
    qDebug()<<"Id  :"<<id;
    qDebug()<<"Icon:"<<icon;
    qDebug()<<"x   :"<<x;
    qDebug()<<"y   :"<<y;

    if(!rIdNode.type.empty())
        this->relativeId = QString::fromStdString(Parser::getValue(rIdNode).content);

    if(!exNode.type.empty())
        this->excl=getFocusPreqs(exNode);

    foreach(const AstNode& node,preqNodes){
        preReq.push_back(getFocusPreqs(node));
        /*qDebug()<<"preqs:";
        auto dbg=qDebug();
        foreach(const QString &str,preReq.back())
            dbg<<str;*/
    }

    /*auto dbg=qDebug();
    dbg<<"mutually exclusives:";
    foreach(const QString &str,this->excl)
        dbg<<str;
    */
    otherInfo=Parser::getAllExcept(lst,{"id","icon","x","y","relative_position_id","prerequisite","mutually_exclusive"});
}

