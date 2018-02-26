#include "CustomComboBoxDelegate.h"
#include <QComboBox>
#include <QVariantList>
#include <QDebug>
#include <QComboBox>
 
CustomComboBoxDelegate::CustomComboBoxDelegate(QObject *parent)
: QItemDelegate(parent)
{
	baseDictList = NULL;
}
CustomComboBoxDelegate::~CustomComboBoxDelegate()
{
	if (m_editor != NULL)
	{
		delete m_editor;
		m_editor = NULL;
	}
}
QWidget *CustomComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
	const QModelIndex &index)const
{
	if (m_editor != NULL) return m_editor;
	m_editor = new QComboBox(parent);
	if (m_editor != NULL)
	{
		return m_editor;
	}
	else
	{
		return NULL;
	}
	return m_editor;
}
/*设置基础字典的值，并绑定数据*/
void CustomComboBoxDelegate::setBaseDictList(QList<BaseDictDataModel*> *tempList)
{
	baseDictList = tempList;
	if (tempList != NULL&&m_editor != NULL)
	{
		m_editor->clear();
		for (int i = 0; i < tempList->size(); i++)
		{
			//m_editor->addItem(tempList->value(i)->getValueName());
			m_editor->addItem(tempList->value(i)->m_data.valueName);
		}
	}
}
/*设置默认项*/
void CustomComboBoxDelegate::setEditorData(const QModelIndex &index) const
{
	QString text = index.model()->data(index, Qt::EditRole).toString();
	int tindex = m_editor->findText(text);
	m_editor->setCurrentIndex(tindex);
}
void CustomComboBoxDelegate::setModelData(QAbstractItemModel *model, const QModelIndex &index) const
{
	QString text = m_editor->currentText();
	model->setData(index, text, Qt::EditRole);
}
void CustomComboBoxDelegate::setVisible(bool flag)
{
	m_editor->setVisible(flag);
}
void CustomComboBoxDelegate::setEnabled(bool flag)
{
	m_editor->setEnabled(flag);
}
void CustomComboBoxDelegate::setEditable(bool flag)
{
	m_editor->setEditable(flag);
}
void CustomComboBoxDelegate::updateEditorGeometry(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	m_editor->setGeometry(option.rect);
}
/* 清空绑定的数据*/
void CustomComboBoxDelegate::clearBaseDictList()
{
	if (m_editor != NULL)
	{
		m_editor->clear();
	}
}
QString CustomComboBoxDelegate::getDictValueId(int index) const
{
	return getDictValueOrNameOrIdOrComment(index, 2);
}

QString CustomComboBoxDelegate::getDictValueName(int index) const
{
	return getDictValueOrNameOrIdOrComment(index, 1);
}

QString CustomComboBoxDelegate::getDictDictComment(int index) const
{
	return getDictValueOrNameOrIdOrComment(index, 3);
}
/*返回数据字典*/
QList<BaseDictDataModel*> *CustomComboBoxDelegate::getBaseDictDataModel()const
{
	return baseDictList;
}
/*!
index 索引;type 类型*/
QString CustomComboBoxDelegate::getDictValueOrNameOrIdOrComment(int index, int type)const
{
	if (baseDictList != NULL&&index >= 0)
	{
		switch (type)
		{
		case 1: return baseDictList->value(index)->m_data.valueName; break;
		case 2: return baseDictList->value(index)->m_data.valueId; break;
		case 3: return baseDictList->value(index)->m_data.dictComment; break;
		default:break;
		}
	}
	return NULL;
}