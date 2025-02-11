#include "focusmodel.h"

FocusModel::FocusModel(QObject *parent)
    : QObject{parent}
{}

void FocusModel::init(const AstNode& node){

}

AstNode FocusModel::data(const QString& index){
    return AstNode();
}

