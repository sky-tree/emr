#pragma once
#ifndef BASEDICTDATAMODEL_H
#define BASEDICTDATAMODEL_H
#include <QString>
#include"qobject.h"
/********qt版本高于等于qt5，VS版本高于或等于VS2010 ******/
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
/************************************************************************/
/*通用基础字典数据处理模型*/ 
/************************************************************************/
class  metaData:public QObject
{
public:
	/*字典序列号，自增不可重复*/
	QString id;
	/*编码字典表名称*/
	QString dictName;
	/*字典分类，主要用于数据归类*/
	QString dictType;
	/*编码字典表中文名称*/
	QString dictComment;
	/*字典对应的ID编码*/
	QString valueId;
	/*字典对应的名称值*/
	QString valueName;
	/*字典名称对应的拼音简码*/
	QString inputCode;
	/*是否被停用 1启用 0停用*/
	QString isStop;
	/*国家标准编码*/
	QString gbCode;
	/*顺序号，用于排序*/
	QString sortNo;
};
class BaseDictDataModel
{
public:
	BaseDictDataModel();
	~BaseDictDataModel();
	//QString getId();
	//QString getDictName();
	//QString getDictType();
	//QString getDictComment();
	//QString getValueId();
	//QString getValueName();
	//QString getInputCode();
	//QString getIsStop();
	//QString getGbCode();
	//QString getSortNo();

	//void setId(QString temp);
	//void setDictName(QString temp);
	//void setDictType(QString temp);
	//void setDictComment(QString temp);
	//void setValueId(QString temp);
	//void setValueName(QString temp);
	//void setInputCode(QString temp);
	//void setIsStop(QString temp);
	//void setGbCode(QString temp);
	//void setSortNo(QString temp);

public:
	metaData m_data;

};
#endif // BASEDICTDATAMODEL_H

