#include "views/NavigationView.h"
#include "flowlayout.h"

#include <QPushButton>
 
#define TEST(stringName) \
	flowLayout->addWidget(new QPushButton(tr(stringName)));

NavigationView::NavigationView(QWidget *parent) : QWidget(parent)
{
	//FlowLayout * flowLayout = new FlowLayout();
	//this->setLayout(flowLayout);
 //   this->setStyleSheet("QPushButton{background-image:url(:/Res/images/Menu1.jpg); border: 0px; width: 123px; height: 120px} QPushButton:hover{background-image:url(:/Res/images/menu_selected.png);}");
	//flowLayout->addWidget(new QPushButton(tr("药房退药申请")));
	//flowLayout->addWidget(new QPushButton(tr("药品调拨管理")));
	//flowLayout->addWidget(new QPushButton(tr("药房库存查询")));
	//flowLayout->addWidget(new QPushButton(tr("出入库记录查询")));
	//flowLayout->addWidget(new QPushButton(tr("药房领药申请管理")));
	//TEST("药房报损管理")TEST("药房盘点管理")TEST("药房药品管理")TEST("药品目录")TEST("员工管理")TEST("票据管理")
 //   TEST("账户管理")TEST("疾病编码管理")TEST("核算分类管理")TEST("收入项目管理")TEST("部门管理")TEST("支付方式管理")
	//TEST("诊疗项目管理")TEST("医嘱用法设置")TEST("库存药品单位配置")TEST("部门药房配置")TEST("用药途径管理")
	//TEST("收费项目管理")TEST("生产商管理")TEST("供应商管理")TEST("角色管理")TEST("门诊病历模板管理")TEST("系统参数配置")
	//TEST("门诊处方组套管理")TEST("退费授权管理")TEST("药品收费配置")TEST("药品调价")TEST("药库报损管理")
	//TEST("部门领药管理")TEST("药库盘点管理")TEST("药库退药管理")TEST("出入库记录查询")TEST("科室退药")
}
NavigationView::~NavigationView()
{

}
