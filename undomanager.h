#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <QObject>
#include <memory>
#include <QStack>
#include "focusitem.h"

class BaseAction;
using ActionPtr = std::shared_ptr<BaseAction>;

#define newAction std::make_shared

class BaseAction{
public:
    virtual void execute() = 0;
    virtual ActionPtr getReversedAction()const = 0;
    virtual QString name() const = 0;
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
    MoveFocusAction(FocusItem *_item,int dx,int dy);
    int dx,dy;
    FocusItem *item;
private:
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
