#include "customDelegate/CustomLineEditNumbersDelegate.h"
#include <QLineEdit>
 
CustomLineEditNumbersDelegate::CustomLineEditNumbersDelegate(QObject *parent)
: QItemDelegate(parent)
{
}
CustomLineEditNumbersDelegate::~CustomLineEditNumbersDelegate()
{
	if (m_editor != NULL)
	{
		delete m_editor;
		m_editor = NULL;
	}
}
void CustomLineEditNumbersDelegate::setQRegExpValue(QRegExp tempRegExp)
{
		CustomLineEditNumbersDelegate::regExp = tempRegExp;
		if (!tempRegExp.isEmpty())
			m_editor->setValidator(new QRegExpValidator(regExp));
}

QRegExp* CustomLineEditNumbersDelegate::getQRegExpValue()
{
	return &regExp;
}

QWidget *CustomLineEditNumbersDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
	const QModelIndex &index) const
{
	m_editor = new QLineEdit(parent);
	if (m_editor != NULL)return m_editor;
	return NULL;
}

void CustomLineEditNumbersDelegate::setEditorDataForTableView(const QModelIndex &index)
{
	QString text = index.model()->data(index, Qt::EditRole).toString();
	m_editor->setText(text);
}
void CustomLineEditNumbersDelegate::setModelDataForTableView(QAbstractItemModel *model,
	const QModelIndex &index)
{
	QString text = m_editor->text();
	model->setData(index, text, Qt::EditRole);
}
void CustomLineEditNumbersDelegate::updateEditorGeometryForTableView(const QStyleOptionViewItem &option,
	const QModelIndex &index)
{
	m_editor->setGeometry(option.rect);
}