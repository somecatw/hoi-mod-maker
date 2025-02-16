#ifndef PRESET_PARSER_H
#define PRESET_PARSER_H
#include "parser.h"
#include <QFile>
#include <vector>
#include <QWidget>
#include <map>
struct eazy_statement{
    //一些简单语句 A token B
    QString A,token,B;
};
struct eazy_node{
    //一些简单的节点name={A1=B1 A2=B2}
    QString name;
    std::vector<eazy_statement>effects;
};
struct idea{
    QString id;
    std::vector<eazy_statement> st;
    std::vector<eazy_node> nodes;
};
struct preset{
    QString translation;//中文表示
    QString str;//表达式原始字符串
    std::vector<QString> type;//依次可以选择的变量A，B，C的type
    std::map<QString,int> id;//A,B,C在type中对应的位置
};
extern std::vector<preset> focus_effect;
struct preset_type{
    QString tpName;
    std::map<QString,QString> translation;
};
extern std::map<QString,preset_type>mp;
void preset_focus_effect_init();
void preset_init();
QString out_str(preset);
class preset_parser
{
public:
    preset_parser();
};

#endif // PRESET_PARSER_H
