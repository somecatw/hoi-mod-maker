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
    bool useLines;

    /* 一个国策可能会有多组前置国策，每组只要完成一个即可，即与-或的关系
     * 在编辑国策时，每组前置国策用一种颜色显示
     * 设置新的前置国策时，出现一个菜单，让用户选择把新的前置放到哪一组中
     * 这样就无需区分“加入前驱”和“加入可选前驱”了
     */
    QVector<QVector<QString>> preReq;
    QVector<QString> excl;
    QString relativeId;

    QVector<AttrPointer> otherInfo;

    AttrPointer src;

    Focus();
    Focus(AttrPointer attr);
    AttrPointer toLangObj()const;
};

class FocusModel : public QObject
{
    Q_OBJECT
public:
    explicit FocusModel(QObject *parent = nullptr);
    bool init(ObjPointer obj);
    Focus data(const QString& index) const;
    const QVector<Focus>& allData() const;
    void batchMoveFocus(const QVector<QString> &ids,int dx,int dy);
    void addFocusPreq(const QString &baseId,const QString &targetId,int group);
    void removeFocusPreq(const QString &baseId,const QString &targetId);
    void addFocusExcl(const QString &baseId,const QString &targetId);
    void removeFocusExcl(const QString &baseId,const QString &targetId);
    int getFocusPreqGroup(const QString &baseId,const QString &targetId) const;
private:
    QMap<QString,size_t> focusIndex;
    QVector<Focus> focuses;

    // 源文件中用到但没有定义的国策 id 被保护，不允许新建同名国策
    // 如果某个国策的前置国策、互斥国策中出现这些 id，则不在 treeView 中绘制其前置、互斥国策线，将编辑功能移到 FocusEditor 中手动处理
    QSet<QString> protectedFocusId;

    void checkFocusDefinition();
signals:
    void focusMoved(const QVector<QString> &id,int dx,int dy);
    void focusPreqChanged(const QString &id);
    void focusExclChanged(const QString &id);
};

#endif // FOCUSMODEL_H
