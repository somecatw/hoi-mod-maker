#ifndef PRESET_IDEAS_H
#define PRESET_IDEAS_H

#include <QMainWindow>

class idea{
public:
    QString t3="\t\t\t";
    QString t5="\t\t\t\t\t";
    QString ts(int num)
    {
        QString a="";
        while(num--)
        {
            a+='\t';
        }
        return a;
    }
    std::vector<QString> effect,val;
    QString id,name,picture;
    QString out_str;
    idea(QString _str="")
    {
        effect.clear();
        out_str=_str;
    }
    QString set_str()
    {
        out_str=t3+id+" = {\n"+t3+"\tname = "+name+"\n"+t3+"\tremoval_cost = -1\n"+t3+"\tpicture = "+picture+"\n"+t3+"\tallowed = {\n"+t3+"\t\talways = yes\n"+t3+"\t}\n"+t3+"\tmodifier = {\n";
        for(int i=0;i<(int)effect.size();++i)
        {
            out_str+=t5+effect[i]+" = "+val[i]+"\n";
        }
        return out_str;
    }
};

namespace Ui {
class preset_ideas;
}

class preset_ideas : public QMainWindow
{
    Q_OBJECT

public:
    Ui::preset_ideas *ui;
    explicit preset_ideas(QWidget *parent = nullptr);
    ~preset_ideas();
    std::vector<idea*> v;
    QString out_str;
    QString set_str()
    {
        out_str="ideas={\t\ncountry = {\n;";
        for(auto i:v)
        {
            out_str+=i->set_str();
        }
        out_str+="\t}\n}";
        return out_str;
    }

private slots:
    void on_pushButton_clicked();

private:

};

#endif // PRESET_IDEAS_H
