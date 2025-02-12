#ifndef PRESET_PARSER_H
#define PRESET_PARSER_H
#include "parser.h"
#include <vector>
struct eazy_statement{
    //一些简单语句 A token B
    QString A,token,B;
};

struct idea_node{
    QString name,id,allowed,allowed_civil_war;
    std::vector<eazy_statement> modifier;
    QString out_str()
    {
        QString str=id+"={\n";
        str+="name="+name+"\n";
        str+="allowed={\n"+allowed+"\n}\n";
        str+="allowed_civil_war={\n"+allowed_civil_war+"\n}\n";
        str+="removal_cost = -1\n";//似乎所有的都是-1，不用再解析了
        str+="modifier={\n";
        for(auto i:modifier)
        {
            str+=i.A+i.token+i.B+"\n";
        }
        str+="}\n";
        return str;
    }
};

class preset_parser
{
public:
    preset_parser();
};

#endif // PRESET_PARSER_H
