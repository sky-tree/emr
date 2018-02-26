//#pragma once
/*********************************************************************************
*Copyright(C),Shine
*FileName:  customComboBoxDelegate
*Author:  caokewen
*Version:  1.0
*Date:  //2016/3/1
*Description:  //此类为一个公共的LineEdit类，为TableView模块添加LineEdit提供方便。
**********************************************************************************/
#ifndef CUSTOMLINEEDITNUMBERSDELEGATE_H
#define CUSTOMLINEEDITNUMBERSDELEGATE_H
 
#include <QItemDelegate>
#include <QRegExp>
/********qt版本高于等于qt5，VS版本高于或等于VS2010 ******/
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
/************************************************************************/
/*自定义委托：基于QLineEdit控件只能输入数字委托(可通过正则表表达式设置输入范围)*/
/************************************************************************/
class CustomLineEditNumbersDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	CustomLineEditNumbersDelegate(QObject *parent = 0);
	~CustomLineEditNumbersDelegate();
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const Q_DECL_OVERRIDE;
	void setEditorDataForTableView(const QModelIndex &index);
	void setModelDataForTableView(QAbstractItemModel *model, const QModelIndex &index);
	void updateEditorGeometryForTableView( const QStyleOptionViewItem &option,const QModelIndex &index);
	/*设置正则表达式公共变量*/
	void setQRegExpValue(QRegExp tempRegExp);
	/*获得正则表达式公共变量*/
	QRegExp *getQRegExpValue();
protected:
	/*正则表达式公共变量*/
	QRegExp regExp;
private:
	mutable QLineEdit *m_editor;
};

#endif // INPUTNUMBERSDELEGATE_H
