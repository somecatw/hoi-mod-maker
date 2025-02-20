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
    QVector<QString> excl;
    QString relativeId;

    QVector<AttrPointer> otherInfo;

    Focus();
    Focus(AttrPointer attr);
};

class FocusModel : public QObject
{
    Q_OBJECT
public:
    explicit FocusModel(QObject *parent = nullptr);
    bool init(ObjPointer obj);
    Focus data(const QString& index) const;
    const QVector<Focus>& allData() const;
    void moveFocus(const QString &id,int dx,int dy,bool isManual);
private:
    QMap<QString,size_t> focusIndex;
    QVector<Focus> focuses;
signals:
    void focusMoved(const QString &id,int dx,int dy,bool isManual);
};

#endif // FOCUSMODEL_H
