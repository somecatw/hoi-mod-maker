#include "undomanager.h"
#include <QDateTime>
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
    if(undoStack.size()){
        if(undoStack.top()->canMergeWith(act)){
            undoStack.top()->mergeWith(act);
            return;
        }
    }
    undoStack.push(act);
}
BaseAction::BaseAction(){
    timeStamp=QDateTime::currentMSecsSinceEpoch();
}
bool BaseAction::canMergeWith(ActionPtr act) const{
    return false;
}
void BaseAction::mergeWith(ActionPtr act){

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
bool MoveFocusAction::canMergeWith(ActionPtr act) const{
    MoveFocusAction *mfa=dynamic_cast<MoveFocusAction*>(act.data());
    if(!mfa)return false;
    if(items!=mfa->items)return false;
    if(mfa->timeStamp-timeStamp>MoveFocusAction::mergeTimeThresholdMS)return false;
    return true;
}
void MoveFocusAction::mergeWith(ActionPtr act){
    MoveFocusAction *mfa=dynamic_cast<MoveFocusAction*>(act.data());
    dx+=mfa->dx;
    dy+=mfa->dy;
}
