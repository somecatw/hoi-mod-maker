#include "preset_parser.h"
#include <map>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDir>
#include <QComboBox>
#include <QStandardItemModel>
preset_parser::preset_parser()
{

}
bool check(QChar c)
{
    if(c==' '||c=='\t'||c=='\r'||c=='\n'||c=='{'||c=='}'||c==','||c=='['||c==']'||c=='('||c==')') return 0;
    return 1;
}
std::map<QString,preset_type>mp;
std::vector<preset> focus_effect;
void preset_init()
{
    mp["float"].tpName="float";
    mp["int"].tpName="int";
    QFile file0(":/preset/alltype.txt");
    if (file0.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString fileData0 = file0.readAll();
        file0.close();
        std::vector<QString> path;
        QString str="";
        for(auto c:fileData0)
        {
            if(c=='\n')
            {
                path.push_back(str);
                mp[str].tpName=str;
                str="";
            }
            if(check(c)) str+=c;
        }
        for(auto p:path)
        {
            QFile file(":/preset/"+p+".type");
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QString fileData = file.readAll();
                file.close();
                QString chi,eng;int r=1;
                for(auto c:fileData)
                {
                    if(check(c))
                    {
                        if(r) chi+=c;
                        else eng+=c;
                    }
                    if(c=='\n')
                    {
                        if(r) r=0;
                        else
                        {
                            mp[p].translation[chi]=eng;
                            chi=eng="";
                            r=1;
                        }
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "打开文件失败!\n";
        return ;
    }
}
QString get1(const QString &str,int &st)
{
    QString s="";int cnt=0;
    for(;st<str.length();++st)
    {
        if(str[st]=='#')
        {
            if(++cnt==2)
            {
                ++st;
                return s;
            }
        }
        if(cnt==1) s+=str[st];
    }
    return s;
}
QString get_line(const QString &str,int &st)
{
    QString s="";
    while(st<str.length()&&!check(str[st])) ++st;
    for(;st<str.length();++st)
    {
        if(str[st]=='\n')
        {
            ++st;
            return s;
        }
        s+=str[st];
    }
    return s;
}
QString read(const QString &str,int &st)
{
    QString a="";
    while(st<str.length()&&!check(str[st])) ++st;
    if(st>=str.length()) return "";
    while(st<str.length()&&check(str[st])) a+=str[st],++st;
    return a;
}
void getX(preset &x,const QString &str)
{
    int cnt=0;
    for(int i=0;i<str.length();++i)
    {
        QString tpName=read(str,i);
        //qDebug()<<tpName<<'\n';
        QString tps=read(str,i);
        x.id[tps]=cnt++;
        x.type.push_back(tpName);
    }
    return ;
}
void preset_focus_effect_init()
{
    QFile file(":/preset/focus_effect_preset.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString str=file.readAll();
        file.close();
        //qDebug().noquote()<<str;
        int i=0;
        while(i<str.length())
        {
            preset pre;
            pre.translation=get_line(str,i);
            if(pre.translation=="") break;
            qDebug()<<pre.translation;
            getX(pre,get_line(str,i));
            pre.str=get1(str,i);
            focus_effect.push_back(pre);
            //++i;
        }
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    for(auto i:focus_effect)
    {
        qDebug().noquote()<<i.str;
        for(auto j:i.type)
        {
            qDebug()<<j;
        }
    }
}
struct input_box{
    int type;
    //0->QComboBox
    //1->QSpinBox
    //2->QDoubleSpinBox
    QString id;
    QComboBox *p0;
    QSpinBox *p1;
    QDoubleSpinBox *p2;
    input_box(int _type=0,QString _id="",QWidget *parent=NULL)
    {
        type=_type;
        id=_id;
        if(type==0)
        {
            p0=new QComboBox(parent);
        }
        else if(type==1)
        {
            p1=new QSpinBox(parent);
        }
        else if(type==2)
        {
            p2=new QDoubleSpinBox(parent);
        }
    }
    QString getvalue()
    {
        if(type==0)
        {
            return p0->currentText();
        }
        else if(type==1)
        {
            return p1->text();
        }
        else if(type==2)
        {
            return p2->text();
        }
        return "";
    }
};

QString out_str(preset x)
{
    QWidget *wid=new QWidget();
    wid->setWindowTitle(x.translation);
    std::vector<input_box*> v;
    int w=0,h=100;
    for(auto i:x.type)
    {
        if(mp[i].tpName=="int")
        {
            input_box *a=new input_box(1,i,wid);
            a->p1->setGeometry(w,0,100,h);
            w+=100;
            v.push_back(a);
        }
        else if(mp[i].tpName=="float")
        {
            input_box *a=new input_box(2,i,wid);
            a->p2->setGeometry(w,0,100,h);
            w+=100;
            v.push_back(a);
        }
        else
        {
            input_box *a=new input_box(0,i,wid);
            a->p0->setInsertPolicy(QComboBox::InsertAtTop);
            QStandardItemModel *model = qobject_cast<QStandardItemModel*>(a->p0->model());
            model->setSortRole(-1);
            for(auto j:mp[i].translation)
            {
                a->p0->addItem(j.first);
            }
            a->p0->setGeometry(w,0,200,h);
            w+=200;
            v.push_back(a);
        }
    }
    wid->resize(w,h);
    wid->show();
    return "";
}
