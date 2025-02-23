#ifndef HOILANGEDIT_H
#define HOILANGEDIT_H

#include <QTextEdit>
#include <QScrollArea>
#include "parser.h"
#include "hoilangview.h"
#include <QSyntaxHighlighter>
#include <QVBoxLayout>
#include <QRegularExpression>

class HoiLangHighlighter : public QSyntaxHighlighter{
    Q_OBJECT
public:
    HoiLangHighlighter(QTextDocument *parent = nullptr);
protected:
    void highlightBlock(const QString &text) override ;
private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
};

class HoiTextEdit: public QTextEdit{
    Q_OBJECT
public:
    explicit HoiTextEdit(QWidget *parent = nullptr);
    bool isProtected;
    void setProtected(bool p);
    void clearSelection();
public slots:
    void updateSize();
protected:
    void keyPressEvent(QKeyEvent *evt) override;
private:
    QString getIndentation(const QString &str) const;
};

class HoiLangEdit: public QScrollArea
{
    Q_OBJECT
public:
    explicit HoiLangEdit(QWidget *parent = nullptr);
    QString plainText();
    HoiLangView *view;

public slots:
    void init(AttrPointer attr,const QString &header,bool headProtected);
    void ensureCursorVisible();
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
    HoiTextEdit *addEdit(bool p);
};

#endif // HOILANGEDIT_H
