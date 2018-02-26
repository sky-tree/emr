#ifndef _BASE_EDIT_PARAM_H_
#define _BASE_EDIT_PARAM_H_

#pragma once
#include <QPainterPath>
#include "BaseContainer.h"
#include "BaseElement.h"

class ViewObj;

struct StrItemData // ��������µ����ݣ���ImgContentData�޸Ķ�д
{
	int				strType;		//item����
	QPainterPath	strPath;		//�����߶�
	QRect			strRect;		//Բ
	QLineF			strLine;		//�߶�
	QPointF			strPoint;		//�ı���ʼλ��
	QString			strQstr;		//�ı�����
	QPolygonF		strPolygon;		//��ͷ
	QPointF			strOffsetPoint;	//�϶�ƫ������
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