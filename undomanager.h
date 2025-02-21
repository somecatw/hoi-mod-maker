#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <QObject>
#include <QStack>
#include "focusitem.h"

class BaseAction;
using ActionPtr = QSharedPointer<BaseAction>;

template<typename Tp,typename Arg,typename ...Args>
ActionPtr newAction(Arg && x, Args && ...xs){
    return QSharedPointer<Tp>::create(std::forward<Arg>(x),std::forward<Args>(xs)...);
}

class BaseAction{
public:
    BaseAction();
    virtual void execute() = 0;
    virtual ActionPtr getReversedAction()const = 0;
    virtual QString name() const = 0;
    virtual bool canMergeWith(ActionPtr act) const;
    virtual void mergeWith(ActionPtr act);
protected:
    time_t timeStamp;
};

class HideFocusAction : public BaseAction{
public:
    void execute() override;
    ActionPtr getReversedAction()const override;
    QString name() const override;
    HideFocusAction(const QSet<FocusItem*> &_items);
private:
    QSet<FocusItem*> items;
};

class ShowFocusAction : public BaseAction{
public:
    void execute() override;
    ActionPtr getReversedAction()const override;
    QString name() const override;
    ShowFocusAction(const QSet<FocusItem*> &_items);
private:
    QSet<FocusItem*> items;
};

class MoveFocusAction : public BaseAction{
public:
    void execute() override;
    ActionPtr getReversedAction()const override;
    QString name() const override;
    MoveFocusAction(const QSet<FocusItem*> &set,focustree *tree,int dx,int dy);
    bool canMergeWith(ActionPtr act) const override;
    void mergeWith(ActionPtr act) override;
    int dx,dy;
    QSet<FocusItem*> items;
    focustree *tree;
private:
    static constexpr int mergeTimeThresholdMS=1000;
};

class AddPrereqAction : public BaseAction{
public:
    void execute() override;
    ActionPtr getReversedAction()const override;
    QString name() const override;
    AddPrereqAction(focustree *tree,QString itemId,QString prereqId,int group);
private:
    focustree *tree;
    QString itemId,prereqId;
    int group;
};

class RemovePrereqAction : public BaseAction{
public:
    void execute() override;
    ActionPtr getReversedAction()const override;
    QString name() const override;
    RemovePrereqAction(focustree *tree,QString itemId,QString prereqId,int group);
private:
    focustree *tree;
    QString itemId,prereqId;
    int group;
};


class UndoManager : public QObject
{
    Q_OBJECT
public:
    explicit UndoManager(QObject *parent = nullptr);

signals:

public slots:
    void undo();
    void redo();
    void addAction(ActionPtr act);
private:
    QStack<ActionPtr> undoStack;
    QStack<ActionPtr> redoStack;
};

#endif // UNDOMANAGER_H
