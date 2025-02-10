#include "parser.h"



Parser::identifier::identifier(std::string strr,bool it){str=strr;mode=it?3:1;}
Parser::identifier::identifier(class Parser* parss){pars=parss;mode=2;}


void astNode::prt(int width)const{
    if(!hide){
        for(int i=0;i<width;i++)std::cout<<"---";
        std::cout<<"\""<<content<<"\""<<" (type="<<type<<")"<<std::endl;
    }
    if(end)return;
    for(const fobject& wtf:children)wtf.prt(width+(!hide));
}

void astNode::_m_correct(std::vector<fobject>& ttfa)const{
    fobject self;
    self.type=type,self.content=content;
    if(end){if(!hide)ttfa.push_back(self);return;}
    for(const fobject& wtf:children)
        if(hide)wtf._m_correct(ttfa);
        else wtf._m_correct(self.children);
    if(!hide)ttfa.push_back(self);
}
void astNode::pp(int width)const{
    for(int i=0;i<width;i++)std::cout<<"---";
    std::cout<<"\""<<content<<"\""<<" (type="<<type<<")"<<std::endl;
    for(const fobject& wtf:children)wtf.pp(width+1);
}

astNode astNode::correct()const{
    fobject root=*this;
    while(root.hide)root=(root.children.at(0));
    std::vector<fobject> temp;
    for(const fobject& wtf:root.children)
        wtf._m_correct(temp);
    root.children=temp;
    return root;
}

bool astNode::hasChild(std::string type)const{
    for(const fobject& child:this->children)
        if(child.type==type)return true;
    return false;
}
bool astNode::hasContent(std::string cont)const{
    for(const fobject& child:this->children)
        if(child.content==cont)return true;
    return false;
}
astNode astNode::firstMatch(std::string type)const{
    for(const fobject& child:this->children)
        if(child.type==type)return child;
    return fobject();
}
std::vector<astNode> astNode::allMatch(std::string type)const{
    std::vector<fobject> ret;
    for(const fobject& child:this->children)
        if(child.type==type)ret.push_back(child);
    return ret;
}

Parser::Parser(){};
Parser::Parser(std::string ftype,bool ban_skip,bool _hide,bool _chide,bool _end){
    mytype=ftype;
    fend=_end;
    if(!ban_skip)skip=" \n\r\t";
    hide=_hide;
    chide=_chide;
}
bool Parser::tobeskiped(citer x){
    return std::find(skip.begin(),skip.end(),*x)!=skip.end();
}
bool Parser::endflag(citer x){
    return std::find(stop.begin(),stop.end(),*x)!=stop.end();
}
void Parser::_m_regnecor(){
    ttfa.rbegin()->mode=matchnode::necessary;
}
void Parser::_m_regsecor(){
    ttfa.rbegin()->mode=matchnode::optional;
}
astNode Parser::tryMatch
    (citerm begin,citer end,const matchnode& cnode){
    std::vector<fobject> considered_retval;
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
            fobject curr=cid.pars->parse(t,end);
            if(curr.type=="fail")goto fail;
            considered_retval.push_back(curr);
            position_of_retval.push_back(t);
        }else{
            //cout<<cid.str<<" "<<*t<<endl;
            if(cid.str.find(*t)==std::string::npos&&t!=end)++t;
            else goto fail;
            fobject temp={"token",std::string()+(*(t-1)),{},chide};
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
astNode Parser::parse(citerm begin,citer end){
    //cout<<"begin to match:"<<mytype<<endl;
    fobject ret;
    ret.type=mytype;
    ret.hide=hide;
    ret.end=fend;
    citer prevbegin=begin;
    while(tobeskiped(begin)&&begin!=end)begin++;
    for(const matchnode& cnode:ttfa){
        bool flag=false;
        while(!flag||cnode.mode==matchnode::repeatable){
            fobject curr=tryMatch(begin,end,cnode);
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
astNode Parser::parse(std::string x){
    citer a=x.begin(),b=x.end();
    return parse(a,b);
}

Parser strchar("character",1),str_no_quot("string",1),str_has_quot("string"),str("string",1,0,1,1);
Parser block("block"),term("term"),argument("argument"),file("file");

void init(){
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
void replaceAll(std::string &str, const std::string &from, const std::string &to) {
    size_t start_pos=0;
    while((start_pos=str.find(from, start_pos))!=std::string::npos) {
        str.replace(start_pos,from.length(),to);
        start_pos += to.length();
    }
}
void clearComment(std::string &str){
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
void parse(std::string str){
    replaceAll(str,"\\\"","[");
    replaceAll(str,"\\\\","]");

    clearComment(str);

    if(str.size()>3u && (unsigned char)str[0]==0xef && (unsigned char)str[1]==0xbb && (unsigned char)str[2]==0xbf)
        str.erase(str.begin(),str.begin()+3);

    astNode node=file.parse(str);
    if(node.content.size()==str.size())std::cout<<"ok"<<std::endl;
    else std::cout<<"fail"<<std::endl;
}
