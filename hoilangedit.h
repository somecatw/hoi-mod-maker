#ifndef HOILANGEDIT_H
#define HOILANGEDIT_H

#include <QTextEdit>
#include <QScrollArea>
#include "parser.h"
#include "hoilangview.h"
#include <QVBoxLayout>

class HoiTextEdit: public QTextEdit{
    Q_OBJECT
public:
    explicit HoiTextEdit(QWidget *parent = nullptr);
    bool isProtected;
    void setProtected(bool p);
public slots:
    void updateSize();
};

class HoiLangEdit: public QScrollArea
{
    Q_OBJECT
public:
    explicit HoiLangEdit(QWidget *parent = nullptr);
    QString plainText();
    HoiLangView *view;

public slots:
    void init(AttrPointer attr,const QString &header);

protected:
    // bool eventFilter(QObject *obj,QEvent *evt) override;
signals:
    void textChanged();
private:
    QWidget *w;
    QVBoxLayout *layout;
    QVector<HoiTextEdit*> ttfa;
    HoiTextEdit *appendLangText(AttrPointer attr,HoiTextEdit *edit,bool inheritedP,QString path="",int w=0);
    HoiTextEdit *appendLangText(ObjPointer obj,HoiTextEdit *edit,bool inheritedP,QString path="",int w=0);
};

#endif // HOILANGEDIT_H
