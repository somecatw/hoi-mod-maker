#include "preset_add_point.h"
#include "ui_preset_add_point.h"

preset_add_point::preset_add_point(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::preset_add_point)
{
    ui->setupUi(this);
}

preset_add_point::~preset_add_point()
{
    delete ui;
}

void preset_add_point::on_pushButton_2_clicked()
{
    QString atype = this->ui->comboBox->currentText();
    int val = this->ui->spinBox->value();
    //qDebug()<<atype;
    QString str1="";
    if(atype=="稳定度%")
    {
        str1 = "add_stability = ";
        double val2 = (double)val/(double)100.0;
        str1 += QString::number(val2, 'f', 2);
        this->out_str=str1;
    }
    else if(atype=="战争支持度%")
    {
        str1 = "add_war_support = ";
        double val2 = (double)val/(double)100.0;
        str1 += QString::number(val2, 'f', 2);
        this->out_str=str1;
    }
    else if(atype=="政治点数")
    {
        str1 = "add_political_power = ";
        str1 += QString::number(val);
        this->out_str=str1;
    }
    else if(atype=="指挥点数")
    {
        str1 = "add_command_power = ";
        str1 += QString::number(val);
        this->out_str=str1;
    }
    else if(atype=="陆军经验")
    {
        str1 = "army_experience = ";
        str1 += QString::number(val);
        this->out_str=str1;
    }
    else if(atype=="海军经验")
    {
        str1 = "air_experience = ";
        str1 += QString::number(val);
        this->out_str=str1;
    }
    else if(atype=="空军经验")
    {
        str1 = "navy_experience = ";
        str1 += QString::number(val);
        this->out_str=str1;
    }
    else if(atype=="法西斯主义支持率")
    {
        str1 = "add_popularity = {\n\tideology = ";
        str1 += "fascism";
        str1 += "\n\tpopularity = ";
        double val2 = (double)val/(double)100.0;
        str1 += QString::number(val2, 'f', 2);
        str1 +=  "\n}";
        this->out_str=str1;
    }
    else if(atype=="民主主义支持率")
    {
        str1 = "add_popularity = {\n\tideology = ";
        str1 += "democratic";
        str1 += "\n\tpopularity = ";
        double val2 = (double)val/(double)100.0;
        str1 += QString::number(val2, 'f', 2);
        str1 +=  "\n}";
        this->out_str=str1;
    }
    else if("中立主义支持率")
    {
        str1 = "add_popularity = {\n\tideology = ";
        str1 += "neutrality";
        str1 += "\n\tpopularity = ";
        double val2 = (double)val/(double)100.0;
        str1 += QString::number(val2, 'f', 2);
        str1 +=  "\n}";
        this->out_str=str1;
    }
    else if(atype=="共产主义支持率")
    {
        str1 = "add_popularity = {\n\tideology = ";
        str1 += "communism";
        str1 += "\n\tpopularity = ";
        double val2 = (double)val/(double)100.0;
        str1 += QString::number(val2, 'f', 2);
        str1 +=  "\n}";
        this->out_str=str1;
    }
    else if(atype=="执政党支持率")
    {
        str1 = "add_popularity = {\n\tideology = ";
        str1 += "ROOT";
        str1 += "\n\tpopularity = ";
        double val2 = (double)val/(double)100.0;
        str1 += QString::number(val2, 'f', 2);
        str1 +=  "\n}";
        this->out_str=str1;
    }
    qDebug().noquote()<<this->out_str;
    return ;
}

