#ifndef _BASE_EDIT_PARAM_H_
#define _BASE_EDIT_PARAM_H_

#pragma once
#include <QPainterPath>
#include "BaseContainer.h"
#include "BaseElement.h"

class ViewObj;

struct StrItemData // 如果增加新的数据，在ImgContentData修改读写
{
	int				strType;		//item类型
	QPainterPath	strPath;		//自由线段
	QRect			strRect;		//圆
	QLineF			strLine;		//线段
	QPointF			strPoint;		//文本起始位置
	QString			strQstr;		//文本内容
	QPolygonF		strPolygon;		//箭头
	QPointF			strOffsetPoint;	//拖动偏移坐标
public:
	StrItemData()
	{
		strType = 0;
	}
};

class BaseEditParam :
	public BaseElement
{
public:
	BaseEditParam(ViewObj * owner, int classID);
	BaseEditParam(ViewObj * owner, int classID, BaseEditParam * source);

	~BaseEditParam();
public:

	void setItem(QList<StrItemData> items);
	QList<StrItemData> &getItem();
	void setScaleW(int w);
	void setScaleH(int h);
	int getScaleW();
	int getScaleH();

private:
	ViewObj * mViewOwner;
	int			mScaleW;
	int			mScaleH;
	QList<StrItemData> mItemsData;
	QList<StrItemData*> mpItemsData;
};

#endif