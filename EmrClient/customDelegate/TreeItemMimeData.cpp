#include "treeitemmimedata.h"
#include"qdebug.h"
#include "TempleteTypeEnumeration.h"
QMap<QString, QString> * TreeItemMimeData::m_map = NULL;
TreeItemMimeData::TreeItemMimeData()
{
	m_pDragItem = NULL;
}
TreeItemMimeData::~TreeItemMimeData()
{
// 	if (m_map)
// 	{
// 		delete m_map;
// 		m_map = NULL;
// 	}
}
void TreeItemMimeData::SetDragData(QString mimeType,QTreeWidgetItem *pItem)
{
	m_format << mimeType;
	m_pDragItem = pItem;
	if (m_map == NULL)
	{
		m_map = new QMap<QString, QString>();
	}

	m_map->clear();
	QString str = pItem->text(0);
	if (pItem->text(0)=="录入提示")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::INPUT_TEXT);
		m_map->insert("name", "录入提示");
	}
	if (pItem->text(0) == "关键词")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::KEYWORD);
		m_map->insert("name", "关键词");
	}
	if (pItem->text(0)== "单选")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::RADIO);
		m_map->insert("name", "单选");
	}
	else if (pItem->text(0) == "多项")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::MULTIPLE_CHOICE);
		m_map->insert("name", "多项");
	}
	else if(pItem->text(0) == "有无选择")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::HAVE_CHOICE);
		m_map->insert("name", "有无选择");
	}
	else if (pItem->text(0) == "单选（勾选型）")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::UNFOLD_RADIO);
		m_map->insert("name", "单选");
	}
	else if (pItem->text(0) == "多选（勾选型）")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::UNFOLD_MULTIPLE_CHOICE);
		m_map->insert("name", "多选");
	}
	else if (pItem->text(0) == "格式化录入")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::STRUCTURE);
		m_map->insert("name", "格式化录入");
	}
	else if (pItem->text(0) == "表格")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::EXCEL);
		m_map->insert("name", "表格");
	}
	else if (pItem->text(0) == "时间选项")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::TIME);
		m_map->insert("name", "时间选项");
	}
	else if (pItem->text(0) == "数值")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::NUMBER_FILTER);
		m_map->insert("name", "数值");
	}
	else if (pItem->text(0) == "分页符")
	{
		m_map->insert("dlgType", "12");
		m_map->insert("name", "分页符");
	}
	else if (pItem->text(0) == "固定文本")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::FIXTEXT);
		m_map->insert("name", "固定文本");
	}
	else if (pItem->text(0) == "横线")
	{
		m_map->insert("dlgType", "14");
		m_map->insert("name", "横线");
	}
	else if (pItem->text(0) == "公式")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::MENSTRUATION);
		m_map->insert("name", "公式");
	}
	else if (pItem->text(0) == "图像")
	{
		m_map->insert("dlgType", "16");
		m_map->insert("name", "图像");
	}
	else if (pItem->text(0) == "特殊符号")
	{
		m_map->insert("dlgType", "17");
		m_map->insert("name", "特殊符号");
	}
	else if (pItem->text(0) == "常用医学单位")
	{
		m_map->insert("dlgType", "18");
		m_map->insert("name", "常用医学单位");
	}
	else if (pItem->parent()->text(0) == "患者基本信息")
	{
		m_map->insert("dlgType", TempleteTypeEnumeration::MACRO);
		QString str = pItem->text(0);
		QVariant var = pItem->data(0, Qt::UserRole);
		QString  id = var.toString();
		QString name = str + ":";
		m_map->insert("name",name);
		m_map->insert("value", str);
		m_map->insert("id",id);
	}	
}
QStringList TreeItemMimeData::formats() const
{
	return m_format;
}
QMap<QString,QString> TreeItemMimeData::DragItemData() const
{
	return *m_map;
}
QVariant TreeItemMimeData::retrieveData(const QString &mimetype, QVariant::Type preferredType) const
{
	if (mimetype == "ItemMimeData")
	{
		return m_pDragItem;
	}
	else
	{
		return QMimeData::retrieveData(mimetype, preferredType);
	}
}

