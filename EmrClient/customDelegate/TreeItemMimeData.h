#pragma once
#include "qmimedata.h"
#ifndef TREEITEMMIMEDATA_H
#define TREEITEMMIMEDATA_H
#include<QMimeData>
#include<QTreeWidgetItem>
#include<QVariant>
#include<QStringList>
#include<QString> 
#include"qmap.h"
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class TreeItemMimeData :public QMimeData
{
	Q_OBJECT
public:
	TreeItemMimeData();
	~TreeItemMimeData();
	void SetDragData(QString mimeType, QTreeWidgetItem *pItem);
	QStringList formats() const;
public:
	QMap<QString,QString> DragItemData()const; //撤销的函数,已废弃
protected:
	QVariant retrieveData(const QString &mimetype, QVariant::Type preferredType) const;
private:
	const QTreeWidgetItem   *m_pDragItem;
	QStringList              m_format;
public:
	static QMap<QString, QString>   *m_map;
};

#endif // TREEITEMMIMEDATA_H


