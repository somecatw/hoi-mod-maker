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
#include <QString>

class HoiObject;
class Attribute;

using ObjPointer = QSharedPointer<HoiObject>;
using AttrPointer = QSharedPointer<Attribute>;

class HoiObject{
public:
    static constexpr int String=0,Object=1;

    HoiObject();
    HoiObject(int _type);

    // 同时只能存在一个，为方便起见，把两者并列
    QString content;
    QVector<AttrPointer> attributes;

    int type;
    bool isString();
    bool isObject();

    // 在属性中寻找满足条件的 key, 返回这一 Attribute
    AttrPointer getFirst(QString key);
    QVector<AttrPointer> getAll(QString key);
    QVector<AttrPointer> getAllExcept(QVector<QString> keys_excluded);
};

class Attribute{
public:
    QString key;
    QString op;
    ObjPointer value;
    Attribute();
    // hoi 语言的 attribute 可以只有 key 没有 value
    bool hasValue();
};

class Parser{
private:
    static void init();
    static void replaceAll(std::string &str, const std::string &from, const std::string &to);
    static void clearComment(std::string &str);
    static ObjPointer hiddenParse(std::string str);
public:
    // 处理文件并返回文件对应的节点（如果匹配不上，会返回 type=fail）
    static ObjPointer parseFile(QString filename);
    static ObjPointer parse(QString content);
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
