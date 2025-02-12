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

    // 第一维是与，第二维是或
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
