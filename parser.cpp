#include "parser.h"
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

struct AstNode{
    std::string type,content;
    std::vector<AstNode> children;
    bool hide,end;

    void prt(int width=0)const;
private:
    void _m_correct(std::vector<AstNode>& ttfa)const;
    void pp(int width=0)const;
public:

    AstNode correct()const;
    bool hasChild(std::string type)const;
    bool hasContent(std::string cont)const;
    AstNode firstMatch(std::string type)const;
    std::vector<AstNode> allMatch(std::string type)const;


};
class __parser{
private:
    using citer=std::string::const_iterator;
    using citerm=std::string::const_iterator&;
    class identifier{
    public:
        std::string str;
        __parser* pars;
        int mode;
        identifier(std::string strr,bool it=0);
        identifier(__parser* parss);
        static constexpr int String = 1;
        static constexpr int Parser = 2;
        static constexpr int Exclude = 3;
    };
    struct matchnode{
        std::list<identifier> content;
        int mode;
        static constexpr int necessary  = 1;
        static constexpr int optional = 2;
        static constexpr int repeatable = 3;
    };
    std::string mytype;
    std::vector<matchnode> ttfa;
    std::string skip,stop;
    bool hide,chide,fend;
public:
    __parser();
    __parser(std::string ftype,bool ban_skip=false,bool _hide=false,bool _chide=false,bool _end=false);
private:
    bool tobeskiped(citer x);
    bool endflag(citer x);
public:
    void exclude(std::string x,int type=matchnode::repeatable);

    template<typename STR_OR_PTR>
    void necessary(STR_OR_PTR x){
        ttfa.push_back({{{x}},matchnode::necessary});
    }
    template<typename _T,typename ...Args>
    void necessary(_T x,Args ...args){
        ttfa.push_back({{{x}},matchnode::necessary});
        necessary(args...);
    }
    template<typename STR_OR_PTR>
    void optional(STR_OR_PTR x){
        ttfa.push_back({{{x}},matchnode::optional});
    }
    template<typename STR_OR_PTR>
    void repeatable(STR_OR_PTR x){
        ttfa.push_back({{{x}},matchnode::repeatable});
    }
    template<typename ...Args>
    void anyNecessary(Args ...args){
        ttfa.push_back({{},matchnode::necessary});
        _m_regnecor(args...);
    }
    template<typename ...Args>
    void anyOptional(Args ...args){
        ttfa.push_back({{},matchnode::optional});
        _m_regsecor(args...);
    }
private:
    template<typename _T,typename ...Args>
    void _m_regnecor(_T x,Args ...args){
        ttfa.rbegin()->content.push_back(identifier(x));
        _m_regnecor(args...);
    }
    void _m_regnecor();
    template<typename _T,typename ...Args>
    void _m_regsecor(_T x,Args ...args){
        ttfa.rbegin()->content.push_back(identifier(x));
        _m_regsecor(args...);
    }
    void _m_regsecor();
    AstNode tryMatch
        (citerm begin,citer end,const matchnode& cnode);
public:
    AstNode parse(citerm begin,citer end);
    AstNode parse(std::string x);
};

extern __parser file;


__parser::identifier::identifier(std::string strr,bool it){str=strr;mode=it?3:1;}
__parser::identifier::identifier(class __parser* parss){pars=parss;mode=2;}


void AstNode::prt(int width)const{
    if(!hide){
        for(int i=0;i<width;i++)qDebug().noquote()<<"---";
        qDebug().noquote()<<"\""<<content<<"\""<<" (type="<<type<<")"<<Qt::endl;
    }
    if(end)return;
    for(const AstNode& wtf:children)wtf.prt(width+(!hide));
}

void AstNode::_m_correct(std::vector<AstNode>& ttfa)const{
    AstNode self;
    self.type=type,self.content=content;
    if(end){if(!hide)ttfa.push_back(self);return;}
    for(const AstNode& wtf:children)
        if(hide)wtf._m_correct(ttfa);
        else wtf._m_correct(self.children);
    if(!hide)ttfa.push_back(self);
}
void AstNode::pp(int width)const{
    for(int i=0;i<width;i++)std::cout<<"---";
    std::cout<<"\""<<content<<"\""<<" (type="<<type<<")"<<std::endl;
    for(const AstNode& wtf:children)wtf.pp(width+1);
}

AstNode AstNode::correct()const{
    AstNode root=*this;
    while(root.hide)root=(root.children.at(0));
    std::vector<AstNode> temp;
    for(const AstNode& wtf:root.children)
        wtf._m_correct(temp);
    root.children=temp;
    return root;
}

bool AstNode::hasChild(std::string type)const{
    for(const AstNode& child:this->children)
        if(child.type==type)return true;
    return false;
}
bool AstNode::hasContent(std::string cont)const{
    for(const AstNode& child:this->children)
        if(child.content==cont)return true;
    return false;
}
AstNode AstNode::firstMatch(std::string type)const{
    for(const AstNode& child:this->children)
        if(child.type==type)return child;
    return AstNode();
}
std::vector<AstNode> AstNode::allMatch(std::string type)const{
    std::vector<AstNode> ret;
    for(const AstNode& child:this->children)
        if(child.type==type)ret.push_back(child);
    return ret;
}

__parser::__parser(){};
__parser::__parser(std::string ftype,bool ban_skip,bool _hide,bool _chide,bool _end){
    mytype=ftype;
    fend=_end;
    if(!ban_skip)skip=" \n\r\t";
    hide=_hide;
    chide=_chide;
}
void __parser::exclude(string x,int type){
    ttfa.push_back({{{x,1}},type});
}
bool __parser::tobeskiped(citer x){
    return std::find(skip.begin(),skip.end(),*x)!=skip.end();
}
bool __parser::endflag(citer x){
    return std::find(stop.begin(),stop.end(),*x)!=stop.end();
}
void __parser::_m_regnecor(){
    ttfa.rbegin()->mode=matchnode::necessary;
}
void __parser::_m_regsecor(){
    ttfa.rbegin()->mode=matchnode::optional;
}
AstNode __parser::tryMatch
    (citerm begin,citer end,const matchnode& cnode){
    std::vector<AstNode> considered_retval;
    std::vector<citer> position_of_retval;
    for(const identifier& cid:cnode.content){
        citer t=begin;
        if(cid.mode!=identifier::Exclude && end-begin<cid.str.size())goto fail;
        if(cid.mode==identifier::String){
            for(citer it=cid.str.begin();it!=cid.str.end();++it,++t)
                if(*it!=*t){goto fail;}
            considered_retval.push_back({"token",cid.str,{},chide});
            position_of_retval.push_back(t);
        }else if(cid.mode==identifier::Parser){
            AstNode curr=cid.pars->parse(t,end);
            if(curr.type=="fail")goto fail;
            considered_retval.push_back(curr);
            position_of_retval.push_back(t);
        }else{
            //cout<<cid.str<<" "<<*t<<endl;
            if(cid.str.find(*t)==std::string::npos&&t!=end)++t;
            else goto fail;
            AstNode temp={"token",std::string()+(*(t-1)),{},chide};
            considered_retval.push_back(temp);
            position_of_retval.push_back(t);
        }
    fail:continue;
    }
    //temporary solution
    if(!considered_retval.size())return {"fail","",{},false};
    int id=0;
    for(unsigned i=0;i<considered_retval.size();i++)
        if(position_of_retval[i]>position_of_retval[id])id=i;
    begin=position_of_retval[id];
    return considered_retval[id];
}
AstNode __parser::parse(citerm begin,citer end){
    //cout<<"begin to match:"<<mytype<<endl;
    AstNode ret;
    ret.type=mytype;
    ret.hide=hide;
    ret.end=fend;
    citer prevbegin=begin;
    while(tobeskiped(begin)&&begin!=end)begin++;
    for(const matchnode& cnode:ttfa){
        bool flag=false;
        while(!flag||cnode.mode==matchnode::repeatable){
            AstNode curr=tryMatch(begin,end,cnode);
            if(curr.type=="fail"){
                if(cnode.mode==matchnode::necessary){return {"fail","",{},false};}
                else break;
            }
            ret.children.push_back(curr);
            flag=true;
            while(tobeskiped(begin)&&begin!=end)begin++;
            if(endflag(begin))goto endwork;
        }
    }
    ret.content=std::string(prevbegin,begin);
endwork:return ret;
}
AstNode __parser::parse(std::string x){
    citer a=x.begin(),b=x.end();
    return parse(a,b);
}

__parser strchar("character",1),str_no_quot("string",1),str_has_quot("string"),str("string",1,0,1,1);
__parser block("block"),term("term"),argument("argument"),file("file");

void Parser::init(){
    strchar.exclude("\"{} \t\n\r=<>",1);

    str_no_quot.necessary(&strchar);
    str_no_quot.repeatable(&strchar);

    str_has_quot.necessary("\"");
    str_has_quot.exclude("\"");
    str_has_quot.necessary("\"");

    str.anyNecessary(&str_no_quot,&str_has_quot);

    term.necessary(&str);
    term.optional(&argument);

    argument.anyNecessary("=",">","<");
    argument.anyNecessary(&block,&str);

    block.necessary("{");
    block.repeatable(&term);
    block.necessary("}");

    file.repeatable(&term);

}
void Parser::replaceAll(std::string &str, const std::string &from, const std::string &to) {
    size_t start_pos=0;
    while((start_pos=str.find(from, start_pos))!=(size_t)-1) {
        str.replace(start_pos,from.length(),to);
        start_pos += to.length();
    }
}
void Parser::clearComment(std::string &str){
    std::string temp;
    bool flag=0;
    for(char c:str){
        if(c=='#')flag=1;
        if(c=='\n')flag=0;
        if(flag)continue;
        temp+=c;
    }
    str=temp;
}
ObjPointer toObject(const AstNode &node);
AttrPointer toAttr(const AstNode &node){
    if(node.type=="term"){
        AttrPointer ret=AttrPointer::create();
        ret->key = QString::fromStdString(node.children[0].content);
        if(node.children.size()>1){
            const AstNode &arg=node.children[1];
            ret->op = QString::fromStdString(arg.children[0].content);
            ret->value = toObject(arg.children[1]);
        }
        return ret;
    }else{
        qDebug()<<"Parser internal error:";
        qDebug()<<"toAttr :: input is not a attribute";
        return nullptr;
    }
}
ObjPointer toObject(const AstNode &node){
    if(node.type=="file"||node.type=="block"){
        ObjPointer ret = ObjPointer::create(HoiObject::Object);
        for(const AstNode &child:node.children){
            if(child.type=="term")
                ret->attributes.push_back(toAttr(child));
        }
        return ret;
    }else if(node.type=="string"){
        ObjPointer ret = ObjPointer::create(HoiObject::String);
        ret->content = QString::fromStdString(node.content);
        return ret;
    }else{
        qDebug()<<"Parser internal error:";
        qDebug()<<"toObject :: input is not a hoi object";
        return nullptr;
    }
}
ObjPointer Parser::parseFile(QString filename){
    static bool flag=false;

    ifstream ttfa(filename.toStdString());
    stringstream buf;
    buf<<ttfa.rdbuf();
    string str=buf.str();

    return hiddenParse(str);
}

ObjPointer Parser::parse(QString content){
    return Parser::hiddenParse(content.toStdString());
}

ObjPointer Parser::hiddenParse(std::string str){
    static bool flag=false;

    if(!flag){init();flag=true;}

    // 本来我以为原始文本里没有左右方括号，但是其实是有的，所以这个地方要改
    replaceAll(str,"\\\"","[");
    replaceAll(str,"\\\\","]");

    clearComment(str);

    if(str.size()>3u && (unsigned char)str[0]==0xef && (unsigned char)str[1]==0xbb && (unsigned char)str[2]==0xbf)
        str.erase(str.begin(),str.begin()+3);

    AstNode node=file.parse(str).correct();

    if(node.content.size()==str.size())qDebug().noquote()<<"Successfully parsed input string"<<Qt::endl;
    else qDebug()<<"Parser::parse : Syntax error found in input string";

    return toObject(node);
}

AttrPointer HoiObject::getFirst(QString key){
    if(isString()){
        qDebug()<<"HoiObject::getFirst: 此 obj 是字符串，不存在属性";
        return nullptr;
    }else{
        foreach(AttrPointer ptr,attributes){
            if(ptr->key==key)return ptr;
        }
        return nullptr;
    }
}

QVector<AttrPointer> HoiObject::getAll(QString key){
    QVector<AttrPointer> ret;
    if(isString()){
        qDebug()<<"Parser::getAll: 传入的 obj 是字符串，不存在属性";
        return ret;
    }
    foreach(AttrPointer ptr,attributes)
        if(ptr->key==key)ret.push_back(ptr);
    return ret;
}

QVector<AttrPointer> HoiObject::getAllExcept(QVector<QString> keys){
    QVector<AttrPointer> ret;
    if(isString()){
        qDebug()<<"Parser::getAllExcept: 传入的 obj 是字符串，不存在属性";
        return ret;
    }
    foreach(AttrPointer ptr,attributes)
        if(keys.indexOf(ptr->key)==-1)
            ret.push_back(ptr);
    return ret;
}

bool Attribute::hasValue(){
    return value!=nullptr;
}
bool HoiObject::isString(){
    return type==HoiObject::String;
}
bool HoiObject::isObject(){
    return type==HoiObject::Object;
}
HoiObject::HoiObject(int x){
    type=x;
}
HoiObject::HoiObject(){}
Attribute::Attribute(){
    value=nullptr;
}
