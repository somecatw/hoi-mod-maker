#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <QDebug>

// 抽象语法树上的节点，用于保存键值对
struct AstNode{
    std::string type,content;
    std::vector<AstNode> children;
    bool hide,end;

    // 调试监视用的输出函数
    void prt(int width=0)const;
private:
    void _m_correct(std::vector<AstNode>& ttfa)const;
    void pp(int width=0)const;
public:
    // parse 字符串后去掉隐藏节点用
    AstNode correct()const;

    // 查询子节点中有没有类型为 type 的节点
    bool hasChild(std::string type)const;

    // 查询子节点中有没有内容为 cont 的节点
    bool hasContent(std::string cont)const;

    // 返回第一个匹配 type 类型的节点
    AstNode firstMatch(std::string type)const;

    // 以此类推
    std::vector<AstNode> allMatch(std::string type)const;
};

class Parser{
private:
    static void init();
    static void replaceAll(std::string &str, const std::string &from, const std::string &to);
    static void clearComment(std::string &str);
public:
    // 处理文件并返回文件对应的节点（如果匹配不上，会返回 type=fail）
    static AstNode parse(std::string filename);
};

/*
class TermArray{
private:
    QVector<AstNode> terms;
public:
    TermArray();
    TermArray(QString str);
    AstNode& operator[](unsigned id);
    AstNode& operator[](unsigned id)const;
    QVector<AstNode>::iterator begin();
    QVector<AstNode>::iterator end();
    QString toString();
};
*/

#endif // PARSER_H
