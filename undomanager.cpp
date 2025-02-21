#include "undomanager.h"
#include "focustree.h"
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
    foreach(FocusItem *item,items)
        item->hide();
}
ActionPtr HideFocusAction::getReversedAction()const{
    return newAction<ShowFocusAction>(items);
}
HideFocusAction::HideFocusAction(const QSet<FocusItem*> &_items){
    items=_items;
}
QString HideFocusAction::name()const{
    return "HideFocus";
}

void ShowFocusAction::execute(){
    foreach(FocusItem *item,items)
        item->show();
}
ActionPtr ShowFocusAction::getReversedAction()const{
    return newAction<HideFocusAction>(items);
}
ShowFocusAction::ShowFocusAction(const QSet<FocusItem*> &_items){
    items=_items;
}
QString ShowFocusAction::name()const{
    return "ShowFocus";
}

void MoveFocusAction::execute(){
    tree->batchMoveFocus(items,dx,dy);
}
ActionPtr MoveFocusAction::getReversedAction()const{
    return newAction<MoveFocusAction>(items,tree,-dx,-dy);
}
MoveFocusAction::MoveFocusAction(const QSet<FocusItem*> &_items,focustree *_tree,int _dx,int _dy){
    items=_items;
    dx=_dx;
    dy=_dy;
    tree=_tree;
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
    timeStamp=mfa->timeStamp;
}

void AddPrereqAction::execute(){
    tree->addFocusPrereq(itemId,prereqId,group);
}
ActionPtr AddPrereqAction::getReversedAction()const{
    return newAction<RemovePrereqAction>(tree,itemId,prereqId,group);
}
AddPrereqAction::AddPrereqAction(focustree *_tree,QString _itemId,QString _prereqId,int _group){
    tree=_tree;
    itemId=_itemId;
    prereqId=_prereqId;
    group=_group;
}
QString AddPrereqAction::name()const{
    return "AddPrerequisite";
}

void RemovePrereqAction::execute(){
    tree->removeFocusPrereq(itemId,prereqId);
}
ActionPtr RemovePrereqAction::getReversedAction()const{
    return newAction<AddPrereqAction>(tree,itemId,prereqId,group);
}
RemovePrereqAction::RemovePrereqAction(focustree *_tree,QString _itemId,QString _prereqId,int _group){
    tree=_tree;
    itemId=_itemId;
    prereqId=_prereqId;
    group=_group;
}
QString RemovePrereqAction::name()const{
    return "RemovePrerequisite";
}

void AddExclAction::execute(){
    tree->addFocusExcl(itemId,exclId);
}
ActionPtr AddExclAction::getReversedAction()const{
    return newAction<RemoveExclAction>(tree,itemId,exclId);
}
AddExclAction::AddExclAction(focustree *_tree,QString _itemId,QString _exclId){
    tree=_tree;
    itemId=_itemId;
    exclId=_exclId;
}
QString AddExclAction::name()const{
    return "AddExclusion";
}

void RemoveExclAction::execute(){
    tree->removeFocusExcl(itemId,exclId);
}
ActionPtr RemoveExclAction::getReversedAction()const{
    return newAction<AddExclAction>(tree,itemId,exclId);
}
RemoveExclAction::RemoveExclAction(focustree *_tree,QString _itemId,QString _exclId){
    tree=_tree;
    itemId=_itemId;
    exclId=_exclId;
}
QString RemoveExclAction::name()const{
    return "RemoveExclusion";
}
