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
    HideFocusAction(FocusItem *_item);
private:
    FocusItem *item;
};

class ShowFocusAction : public BaseAction{
public:
    void execute() override;
    ActionPtr getReversedAction()const override;
    QString name() const override;
    ShowFocusAction(FocusItem *_item);
private:
    FocusItem *item;
};

class MoveFocusAction : public BaseAction{
public:
    void execute() override;
    ActionPtr getReversedAction()const override;
    QString name() const override;
    MoveFocusAction(const QSet<FocusItem*> &set,int dx,int dy);
    bool canMergeWith(ActionPtr act) const override;
    void mergeWith(ActionPtr act) override;
    int dx,dy;
    QSet<FocusItem*> items;
private:
    static constexpr int mergeTimeThresholdMS=1000;
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
