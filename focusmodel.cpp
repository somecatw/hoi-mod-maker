#include "focusmodel.h"

AttrPointer request(ObjPointer obj, QString key){
    AttrPointer ret = obj->getFirst(key);
    if(!ret){
        qDebug()<<"Error:"<<"key '"<<key<<"' missing in focus";
        return ret;
    }
    return ret;
}

FocusModel::FocusModel(QObject *parent)
    : QObject{parent}
{}

void FocusModel::batchMoveFocus(const QVector<QString> &ids,int dx,int dy){
    foreach(const QString &index,ids){
        size_t id=focusIndex.value(index);
        focuses[id].x+=dx;
        focuses[id].y+=dy;
    }
    emit focusMoved(ids,dx,dy);
}

void FocusModel::addFocusPreq(const QString &baseId,const QString &targetId,int group){
    size_t bindex=focusIndex[baseId];
    if((unsigned)group==focuses[bindex].preReq.size())
        focuses[bindex].preReq.push_back({targetId});
    else
        focuses[bindex].preReq[group].push_back(targetId);
    emit focusPreqChanged(baseId);
}

void FocusModel::removeFocusPreq(const QString &baseId,const QString &targetId){
    size_t bindex=focusIndex[baseId];
    for(QVector<QString> &v:focuses[bindex].preReq){
        if(v.contains(targetId))
            v.remove(v.indexOf(targetId));
    }
    focuses[bindex].preReq.removeAll({});
    emit focusPreqChanged(baseId);
}

bool FocusModel::init(ObjPointer obj){
    AttrPointer tree = request(obj,"focus_tree");

    if(!tree){
        qDebug()<<"Error: 'focus_tree' is not found or not a block";
        return false;
    }

    ObjPointer lst = tree->value;

    QVector<AttrPointer> v1=lst->getAll("focus");
    QVector<AttrPointer> v2=lst->getAll("shared_focus");
    v1.append(v2);

    focuses.clear();
    foreach(AttrPointer attr,v1){
        Focus f=Focus(attr);
        focusIndex.insert(f.id,focuses.size());
        focuses.push_back(f);
    }

    foreach(const Focus& f,focuses)
        if(!f.id.size())return false;
    return true;
}
int FocusModel::getFocusPreqGroup(const QString &base, const QString &target) const{
    const Focus &f=focuses[focusIndex[base]];
    for(unsigned i=0;i<f.preReq.size();i++){
        if(f.preReq[i].contains(target))return i;
    }
    return -1;
}
Focus FocusModel::data(const QString& index) const {
    if(focusIndex.contains(index))
        return focuses[focusIndex.value(index)];
    return Focus();
}

const QVector<Focus>& FocusModel::allData() const{
    return focuses;
}

QVector<QString> getFocusPreqs(AttrPointer attr){
    QVector<QString> ret;

    QVector<AttrPointer> focusNodes = attr->value->getAll("focus");
    foreach(AttrPointer atr,focusNodes)
        ret.push_back(atr->value->content);

    return ret;
}

Focus::Focus(){}

Focus::Focus(AttrPointer attr){
    if(!attr->hasValue()||attr->value->isString()){
        if(attr->key=="shared_focus")
            qDebug()<<"Unknown shared focus"<<attr->value->content<<"found";
        else qDebug()<<"Error: value of attribute 'focus' is not a object";
        return;
    }
    ObjPointer obj = attr->value;
    AttrPointer idNode   = request(obj,"id");
    AttrPointer iconNode = request(obj,"icon");
    AttrPointer xNode    = request(obj,"x");
    AttrPointer yNode    = request(obj,"y");
    AttrPointer rIdNode  = obj->getFirst("relative_position_id");
    QVector<AttrPointer> preqNodes = obj->getAll("prerequisite");
    AttrPointer exNode   = obj->getFirst("mutually_exclusive");

    if(!idNode||!iconNode||!xNode||!yNode)return;

    this->id   = idNode->value->content;
    this->icon = iconNode->value->content;
    this->x    = xNode->value->content.toInt();
    this->y    = yNode->value->content.toInt();

    qDebug()<<"Found focus";
    qDebug()<<"Id  :"<<id;
    qDebug()<<"Icon:"<<icon;
    qDebug()<<"x   :"<<x;
    qDebug()<<"y   :"<<y;

    if(rIdNode)
        this->relativeId = rIdNode->value->content;

    if(exNode)
        this->excl=getFocusPreqs(exNode);

    foreach(AttrPointer node,preqNodes){
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
    otherInfo=obj->getAllExcept({"id","icon","x","y","relative_position_id","prerequisite","mutually_exclusive"});
}

