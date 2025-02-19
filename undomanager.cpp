#include "undomanager.h"

UndoManager::UndoManager(QObject *parent)
    : QObject{parent}
{}

void UndoManager::undo(){
    if(!undoStack.size())return;
    redoStack.push(undoStack.top()->getReversedAction());
    redoStack.top()->execute();
    undoStack.pop();
}

void UndoManager::redo(){
    if(!redoStack.size())return;
    undoStack.push(redoStack.top()->getReversedAction());
    undoStack.top()->execute();
    redoStack.pop();
}

void UndoManager::addAction(ActionPtr act){
    redoStack.clear();
    if (!undoStack.empty() && act->name()=="MoveFocus" && undoStack.top()->name()=="MoveFocus") {
        auto lastMove = dynamic_cast<MoveFocusAction*>(undoStack.top().get());
        auto newMove = dynamic_cast<MoveFocusAction*>(act.get());
        if (lastMove && newMove && lastMove->items == newMove->items) {
            lastMove->dx += newMove->dx;
            lastMove->dy += newMove->dy;
            return;
        }
    }
    undoStack.push(act);
}

void HideFocusAction::execute(){
    item->hide();
}
ActionPtr HideFocusAction::getReversedAction()const{
    return newAction<ShowFocusAction>(item);
}
HideFocusAction::HideFocusAction(FocusItem *_item){
    item=_item;
}
QString HideFocusAction::name()const{
    return "HideFocus";
}

void ShowFocusAction::execute(){
    item->show();
}
ActionPtr ShowFocusAction::getReversedAction()const{
    return newAction<HideFocusAction>(item);
}
ShowFocusAction::ShowFocusAction(FocusItem *_item){
    item=_item;
}
QString ShowFocusAction::name()const{
    return "ShowFocus";
}

void MoveFocusAction::execute(){
    foreach(FocusItem *item,items)
        item->move(dx,dy);
}
ActionPtr MoveFocusAction::getReversedAction()const{
    return newAction<MoveFocusAction>(items,-dx,-dy);
}
MoveFocusAction::MoveFocusAction(const QSet<FocusItem*> &_items,int _dx,int _dy){
    items=_items;
    dx=_dx;
    dy=_dy;
}
QString MoveFocusAction::name()const{
    return "MoveFocus";
}
