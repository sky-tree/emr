//#pragma once
/*********************************************************************************
*Copyright(C),Shine
*FileName:  CustomSpinBoxDelegate
*Author:  liuyi
*Modifier:caokewen
*Version:  1.0
*Date:  //2016/3/8
*Description:  //此类为一个公共的sPinBox类，为其他模块添加sPinBox提供方便。
**********************************************************************************/
#ifndef CUSTOMSPINBOXDELEGATE_H
#define CUSTOMSPINBOXDELEGATE_H
 
#include <QStyledItemDelegate>
#include <QSpinBox>
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
/************************************************************************/
/*         自定义SpinBox委托,可添加到TableView中显示                    */
/************************************************************************/
class CustomSpinBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	CustomSpinBoxDelegate(QObject *parent = 0);

	/*创建自定义widget并返回*/
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, 
		const QModelIndex &index)const Q_DECL_OVERRIDE;
	/*将model中当前有的数据设置到代理上,自己从model取出数据，自己setValue到editor上*/
	void setEditorData(const QModelIndex &index);
	/*将editor上的数据保存到Model中*/
	void setModelData(QAbstractItemModel *model, const QModelIndex &index);
	/*将editor设置到一定位置，并且有一定大小，使这个editor看起来像是正好嵌入到格子里面一样。*/
	void updateEditorGeometry(const QStyleOptionViewItem &option,
		const QModelIndex &index);
	/*默认继承了该方法,如果需要特殊风格绘制单元项中内容,还需重载这个函数.*/
	//void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
	/*设置范围*/
	void setRange(int pMin, int pMax);
private:
	mutable QSpinBox *m_editor;
};

#endif // CUSTOMSPINBOXDELEGATE_H
