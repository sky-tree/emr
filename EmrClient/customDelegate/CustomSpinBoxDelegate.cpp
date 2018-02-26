#include "customDelegate/CustomSpinBoxDelegate.h"
CustomSpinBoxDelegate::CustomSpinBoxDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{

}

QWidget *CustomSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
	const QModelIndex &index)const
{
	m_editor = new QSpinBox(parent);
	if (m_editor != NULL)
	{
		m_editor->setFrame(false);
		return m_editor;
	}
	return NULL;
}

void CustomSpinBoxDelegate::setEditorData(const QModelIndex &index)
{
	int value = index.model()->data(index, Qt::EditRole).toInt();
	m_editor->setValue(value);
}


void CustomSpinBoxDelegate::setModelData(QAbstractItemModel *model, const QModelIndex &index)
{
	m_editor->interpretText();
	int value = m_editor->value();
	model->setData(index, value, Qt::EditRole);
}

void CustomSpinBoxDelegate::updateEditorGeometry( const QStyleOptionViewItem &option,
	const QModelIndex &index)
{
	m_editor->setGeometry(option.rect);
}
void CustomSpinBoxDelegate::setRange(int pMin, int pMax)
{
	m_editor->setMinimum(pMin);
	m_editor->setMaximum(pMax);
}
// void CustomSpinBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
// {	
// 	//QStyledItemDelegate::paint(painter, option, index);
// }