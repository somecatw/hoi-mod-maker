#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

class Parser{
private:
    using citer=std::string::const_iterator;
    using citerm=std::string::const_iterator&;
    class identifier{
    public:
        std::string str;
        Parser* pars;
        int mode;
        identifier(std::string strr,bool it=0);
        identifier(Parser* parss);
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
    struct fobject{
        std::string type,content;
        std::vector<fobject> children;
        bool hide,end;
        void prt(int width=0)const;
    private:
        void _m_correct(std::vector<fobject>& ttfa)const;
        void pp(int width=0)const;
    public:
        fobject correct()const;
        bool hasChild(std::string type)const;
        bool hasContent(std::string cont)const;
        fobject firstMatch(std::string type)const;
        std::vector<fobject> allMatch(std::string type)const;
    };
    Parser();
    Parser(std::string ftype,bool ban_skip=false,bool _hide=false,bool _chide=false,bool _end=false);
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
    fobject tryMatch
        (citerm begin,citer end,const matchnode& cnode);
public:
    fobject parse(citerm begin,citer end);
    fobject parse(std::string x);
};
using astNode=Parser::fobject;

extern Parser strchar,str_no_quot,str_has_quot,str;
extern Parser block,term,argument,file;

void init();
void replaceAll(std::string &str, const std::string &from, const std::string &to);
void clearComment(std::string &str);
void parse(std::string str);
#endif // PARSER_H
