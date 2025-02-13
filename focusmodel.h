#ifndef FOCUSMODEL_H
#define FOCUSMODEL_H

#include <QObject>
#include <QVector>
#include "parser.h"

// 存储单个国策信息的类
class Focus{
public:
    QString id;
    QString icon;
    int x,y;

    /* 一个国策可能会有多组前置国策，每组只要完成一个即可，即与-或的关系
     * 在编辑国策时，每组前置国策用一种颜色显示
     * 设置新的前置国策时，出现一个菜单，让用户选择把新的前置放到哪一组中
     * 这样就无需区分“加入前驱”和“加入可选前驱”了
     */
    QVector<QVector<QString>> preReq;
    QString relativeId;

    QVector<AstNode> otherInfo;

    Focus();
    Focus(const AstNode& node);
};

class FocusModel : public QObject
{
    Q_OBJECT
public:
    explicit FocusModel(QObject *parent = nullptr);
    bool init(const AstNode& node);
    Focus data(const QString& index) const;
    const QVector<Focus>& allData() const;
private:
    QVector<Focus> focuses;
signals:
    void dataChanged();
};

#endif // FOCUSMODEL_H
