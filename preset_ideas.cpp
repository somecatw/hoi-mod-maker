#include "preset_ideas.h"
#include "ui_preset_ideas.h"
#include <map>
#include <vector>
preset_ideas::preset_ideas(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::preset_ideas)
{
    ui->setupUi(this);
}
// std::map<QString,QString>mp,translation;
// void setmap()
// {
//     mp["稳定度修正"]="stability_factor";
//     mp["每周稳定度增长"]="stability_weekly";
//     mp["侵略战争稳定度修正"]="offensive_war_stability_factor";
//     mp["防御战争稳定度修正"]="defensive_war_stability_factor";
//     mp["战争支持度修正"]="war_support_factor";
//     mp["每周战争支持度增长"]="war_support_weekly";
//     mp["政治点数获取"]="political_power_factor";
//     mp["每日获取X点政治点数"]="political_power_gain";
//     mp["每日陆军经验增长"]="experience_gain_army";
//     mp["每日海军经验增长"]="experience_gain_navy";
//     mp["每日空军经验增长"]="experience_gain_air";
//     mp["陆军经验增长加成"]="experience_gain_army_factor";
//     mp["海军经验增长加成"]="experience_gain_navy_factor";
//     mp["空军经验增长加成"]="experience_gain_air_factor";
//     mp["指挥点上限增加"]="max_command_power";
//     mp["每日指挥点增长"]="command_power_gain";
//     mp["指挥点增长加成"]="command_power_gain_mult";
//     mp["每周人力"]="weekly_manpower";
//     mp["每月人口增长修正"]="monthly_population";
//     mp["适役人口"]="conscription";
//     mp["陆军攻击修正"]="army_attack_factor";
//     mp["陆军防御修正"]="army_defence_factor";
//     mp["核心领土陆军攻击修正"]="army_core_attack_factor";
//     mp["核心领土陆军防御修正"]="army_core_defence_factor";
//     mp["科研速度加成"]="research_speed_factor";
//     mp["生活消费品"]="consumer_goods_factor";
//     mp["建造速度加成"]="production_speed_buildings_factor";
//     mp["民工建造速度加成"]="production_speed_industrial_complex_factor";
//     mp["基建建造速度加成"]="production_speed_infrastructure_factor";
//     mp["军工建造速度加成"]="production_speed_arms_factory_factor";
//     mp["海军船坞建造速度加成"]="production_speed_dockyard_factor";
//     mp["铁路建造速度加成"]="production_speed_rail_way_factor";
//     mp["补给中心建造速度加成"]="production_speed_supply_node_factor";
//     mp["合成炼油厂建造速度加成"]="production_speed_fuel_silo_factor";
//     mp["海岸要塞建造速度加成"]="production_speed_coastal_bunker_factor";
//     mp["陆军要塞建造速度加成"]="production_speed_bunker_factor";
//     mp["地区最大工业值加成"]="global_building_slots_factor";
//     mp["工厂修复速度加成"]="industry_repair_factor";
//     mp["正当化战争目标时长修正"]="justify_war_goal_time";
//     mp["敌国正当化战争目标时长修正"]="enemy_justify_war_goal_time";
// }
preset_ideas::~preset_ideas()
{
    delete ui;
}
void preset_ideas::on_pushButton_clicked()
{
    //if(mp.empty())setmap();
    QString atype = this->ui->comboBox->currentText();
    double val = this->ui->doubleSpinBox->value();
}
