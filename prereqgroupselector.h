#ifndef PREREQGROUPSELECTOR_H
#define PREREQGROUPSELECTOR_H

#include <QDialog>
#include <QListWidgetItem>
#include "focusmodel.h"

class focustree;

namespace Ui {
class PrereqGroupSelector;
}

class PrereqGroupSelector : public QDialog
{
    Q_OBJECT

public:
    explicit PrereqGroupSelector(const Focus &base,const Focus &target,focustree *tree,QWidget *parent = nullptr);
    ~PrereqGroupSelector();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PrereqGroupSelector *ui;
    focustree *tree;
    QString baseId,targetId;
    QVector<QListWidgetItem*> items;
};

#endif // PREREQGROUPSELECTOR_H
