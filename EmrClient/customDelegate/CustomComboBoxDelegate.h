//#pragma once
/*********************************************************************************
*Copyright(C),Shine
*FileName:  customComboBoxDelegate
*Author:  liuyi
*Modifier:caokewen
*Version:  1.0
*Date:  //2016/2/24
*Description:  //此类为一个公共的combox类，为其他模块添加Combox提供方便。
**********************************************************************************/
#ifndef CUSTOMCOMBOBOXDELEGATE_H
#define CUSTOMCOMBOBOXDELEGATE_H
#include  "dataModel/hisDataModel/BaseDictDataModel.h"
#include<QItemDelegate>
 
class QComboBox;

#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class CustomComboBoxDelegate :public QItemDelegate
{
	 Q_OBJECT
public:
	CustomComboBoxDelegate(QObject *parent = 0);
	~CustomComboBoxDelegate();
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)const;
	void setBaseDictList(QList<BaseDictDataModel*> *tempList);
	void setEditorData(const QModelIndex &index) const;
	void setModelData(QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void clearBaseDictList();
	void setEnabled(bool);
	void setVisible(bool);
	void setEditable(bool);
	QString getDictValueName(int index)const;
	QString getDictValueId(int index) const;
	QString getDictDictComment(int index)const;
	QList<BaseDictDataModel*>* getBaseDictDataModel()const;
private:
	QString getDictValueOrNameOrIdOrComment(int index, int type)const;
private:
	QList<BaseDictDataModel*> *baseDictList;
	mutable QComboBox *m_editor;

};
#endif