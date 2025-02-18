#include "undomanager.h"

UndoManager::UndoManager(QObject *parent)
    : QObject{parent}
{}

void UndoManager::undo(){
    if(!undoStack.size())return;
    redoStack.push(undoStack.top()->getReversedAction());
    qDebug()<<undoStack.top()->name();
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
