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

void FocusModel::init(const AstNode& node){
    const AstNode &tree = request(node,"focus_tree");

    if(tree.children.size()<=1||tree.children[1].children[1].type!="block"){
        qDebug()<<"Error: 'focus_tree' is not found or not a block";
        return;
    }

    const AstNode& lst = tree.children[1].children[1];

    QVector<AstNode> v1=Parser::getAll(lst,"focus");
    QVector<AstNode> v2=Parser::getAll(lst,"shared_focus");
    v1.append(v2);

    qDebug()<<tree.children.size()<<" "<<v1.size()<<" "<<v2.size();

    focuses.clear();
    foreach(const AstNode& node,v1)
        focuses.emplace_back(node);
}

AstNode FocusModel::data(const QString& index){
    return AstNode();
}



Focus::Focus(){}

Focus::Focus(const AstNode& node){
    if(node.children.size()<=1||node.children[1].children[1].type!="block"){
        qDebug()<<"Error: 'focus' is not a block";
        return;
    }
    const AstNode &lst = Parser::getValue(node);
    const AstNode &idNode   = Parser::getValue(request(lst,"id"));
    const AstNode &iconNode = Parser::getValue(request(lst,"icon"));
    const AstNode &xNode    = Parser::getValue(request(lst,"x"));
    const AstNode &yNode    = Parser::getValue(request(lst,"y"));
    const AstNode &rIdNode  = Parser::getFirst(lst,"relative_position_id");

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
}
