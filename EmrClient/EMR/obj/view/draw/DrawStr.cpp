#include "DrawStr.h"

#include "ViewObj.h"



#include "OperateMgr.h"
#include "FileData.h"

#include "TxtViewParam.h"
#include "RowViewParam.h"


#include "BaseTemplate.h"

#include "ParagraphData.h"
#include "StrContentData.h"

#include "ConstantsID.h"
#include "UIConstants.h"
#include "AuthorityConstants.h"
#include "BaseConstants.h"


#include "BaseTools.h"
#include "BaseSystem.h"
#include "BaseMath.h"


DrawStr::DrawStr(ViewObj * viewOwner) : BaseDraw(viewOwner, ConstantsID::ClassId::DRAW_TXT)
, mTxtData(NULL)
, mTxtViewParam(NULL)
{
}

DrawStr::~DrawStr()
{
	// 参数内存由ViewObj自身回收
	if (mTxtData)
	{
		mTxtData = NULL;
	}
}
bool DrawStr::drawStrList(QPainter &pPainter, int x, int y, QList<StrContentData *> * list)
{
	if (mViewOwner->isObj(ConstantsID::ContainerId::UNFOLD_CHOICE_BOX_CONTAINER_VIEW))
	{
		return false;
	}
	if (!mTxtViewParam)
	{
		mTxtViewParam = (TxtViewParam *)mViewOwner->getViewParam();
		mTxtData = (StrContentData *)mViewOwner->getContentData();
	}
	TxtViewParam * viewData = (TxtViewParam *)mViewOwner->getViewParam();
	x += viewData->getX();
	y += viewData->getY();

	//RowViewParam * rowParam = getRowParam();
	//int rowInsideTop = rowParam->getInsideTop();
	//int rowInsideH = rowParam->getInsideH();

	//char underlineType = mTxtData->getUnderLineType();
	//short underlineThick = mTxtData->getUnderLineThick();
	// 设置font参数
	BaseSystem::getInstance()->refreshPainterFont(pPainter, mTxtData);


	QString str = BaseTools::judgeStrWithPageNumber(mTxtData, mViewOwner->getOperateMgr()->getEffectLoopPage());

	for (int i = 0; i < list->size(); i++)
	{
		QString tempStr = BaseTools::judgeStrWithPageNumber(list->at(i), mViewOwner->getOperateMgr()->getEffectLoopPage());
		str.append(tempStr);
	}


	//int w = viewData->getTotalW();//宽度
	//ViewObj * lastView = (ViewObj*)list->at(list->size() - 1)->getOwner();
	//BaseViewParam * lastParam = lastView->getViewParam();
	//w = lastParam->getX() + lastParam->getTotalW() - viewData->getX();

	//int strH = mTxtViewParam->getSelfH();
	//int strAscent = mTxtViewParam->getAscent();
	//int strDescent = mTxtViewParam->getDescent();
	//int rowDescent = rowParam->getDescent();

	//OperateMgr * operateMgr = mViewOwner->getOperateMgr();
	//bool canWrite = true;
	//if (operateMgr->getFileData()->getFileAuthority() == AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY)
	//	canWrite = false;
	//else
	//	canWrite = mViewOwner->getTempParent()->canWrite();
	//if (!canWrite)
	//{
	//	pPainter.setPen(*UIConstants::MyColor::ONLY_READ_COLOR_OBJ); //画笔颜色
	//}
	//else if (mTxtData->getNeedGray())
	//{
	//	pPainter.setPen(*UIConstants::MyColor::ONLY_READ_COLOR_OBJ);
	//}
	//else
	//{
	//	pPainter.setPen(*mTxtData->getFontColorObj());
	//}

	//y += rowInsideTop;
	//int normalStrY = y + rowInsideH - rowDescent;
	//int curStrH = strAscent;

	//switch (flowType)
	//{
	//	// 正常绘制
	//case BaseConstants::NONE:
	//	pPainter.drawText(x, normalStrY, str);
	//	break;
	//	// 上浮标
	//case BaseConstants::Direction::UP:
	//	pPainter.drawText(x, y + strAscent, str);
	//	break;
	//	// 下浮标
	//case BaseConstants::Direction::DOWN:
	//	pPainter.drawText(x, y + rowInsideH - strDescent, str);
	//	break;
	//}
	int idx = str.indexOf("中医鉴别诊断");
	if (str.indexOf("尿常规未见明显异常") >= 0 || idx >= 0)
	//if (str.indexOf("结合病史") >= 0)
	{
		int a = 0;
	}
	idx = str.indexOf("结合病史");
	if (idx >= 0)
	{
		int a = 0;
	}
	idx = str.indexOf("PH");
	if (idx >= 0)
	{
		int a = 0;
	}
	drawStr(pPainter, str, x, y);

	return false;
}

bool DrawStr::draw(QPainter &pPainter, int x, int y, int w, int h)
{
	if (!mTxtViewParam)
	{
		mTxtViewParam = (TxtViewParam *)mViewOwner->getViewParam();
		mTxtData = (StrContentData *)mViewOwner->getContentData();
	}
	int preX = x;
	int preY = y;


	//FocusTxtData * focusTxtData = mTxtParam->getFocusTxtData();
	//获取字符串的宽度和高度
	//QColor *fillColor = mTxtData->getBackgroundColorObj();
	//int tempSize = mTxtData->getPointSize();

	char underlineType = mTxtData->getUnderLineType();
	short underlineThick = mTxtData->getUnderLineThick();
	// 设置font参数
	BaseSystem::getInstance()->refreshPainterFont(pPainter, mTxtData);


	QString str = BaseTools::judgeStrWithPageNumber(mTxtData, mViewOwner->getOperateMgr()->getEffectLoopPage());
	//for (int i = 0; i < list->size(); i++)
	//{
	//	QString tempStr = BaseTools::judgeStrWithPageNumber(list->at(i));
	//	str.append(tempStr);
	//}
	//mTxtData->refreshPainterFont(pPainter);
	w = mTxtViewParam->getTotalW();//宽度
	h = mTxtViewParam->getTotalH();//高度


	//RowViewParam * rowParam = getRowParam();
	//int strH = mTxtViewParam->getSelfH();
	//int strAscent = mTxtViewParam->getAscent();
	//int strDescent = mTxtViewParam->getDescent();
	//
	//int rowDescent = rowParam->getDescent();
	//int rowInsideH = rowParam->getInsideH();
	//int rowInsideTop = rowParam->getInsideTop();


	//OperateMgr * operateMgr = mViewOwner->getOperateMgr();
	//bool canWrite = true;
	//if (operateMgr->getFileData()->getFileAuthority() == AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY)
	//	canWrite = false;
	//else
	//	canWrite = mViewOwner->getTempParent()->canWrite();
	//if (!canWrite)
	//{
	//	pPainter.setPen(*UIConstants::MyColor::ONLY_READ_COLOR_OBJ);
	//}
	//else if (mTxtData->getNeedGray())
	//{
	//	pPainter.setPen(*UIConstants::MyColor::ONLY_READ_COLOR_OBJ);
	//}
	//else
	//{
	//	pPainter.setPen(*mTxtData->getFontColorObj());
	//}

	//y += rowInsideTop;
	//int normalStrY = y + rowInsideH - rowDescent;
	//int curStrH = strAscent;


	//switch (flowType)
	//{
	//	// 正常绘制
	//case BaseConstants::NONE:
	//	pPainter.drawText(x, normalStrY, str);
	//	break;
	//	// 上浮标
	//case BaseConstants::Direction::UP:
	//	pPainter.drawText(x, y + strAscent, str);
	//	break;
	//	// 下浮标
	//case BaseConstants::Direction::DOWN:
	//	pPainter.drawText(x, y + rowInsideH - strDescent, str);
	//	break;
	//}
	drawStr(pPainter, str, x, y);
	return false;
}
void DrawStr::drawStr(QPainter &pPainter, QString str, int x, int y)
{
	RowViewParam * rowParam = getRowParam();
	int strH = mTxtViewParam->getSelfH();
	int strAscent = mTxtViewParam->getAscent();
	int strDescent = mTxtViewParam->getDescent();

	int rowDescent = rowParam->getDescent();
	int rowInsideH = rowParam->getInsideH();
	int rowInsideTop = rowParam->getInsideTop();

	OperateMgr * operateMgr = mViewOwner->getOperateMgr();
	bool canWrite = true;
	if (operateMgr->getFileData()->getFileAuthority() == AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY)
		canWrite = false;
	else
		canWrite = mViewOwner->getTempParent()->canWrite();
	if (!canWrite)
	{
		pPainter.setPen(*UIConstants::MyColor::ONLY_READ_COLOR_OBJ);
	}
	else if (mTxtData->getNeedGray())
	{
		pPainter.setPen(*UIConstants::MyColor::ONLY_READ_COLOR_OBJ);
	}
	else
	{
		pPainter.setPen(*mTxtData->getFontColorObj());
	}

	y += rowInsideTop;
	int normalStrY = y + rowInsideH - rowDescent;
	int curStrH = strAscent;


	char flowType = mTxtData->getFlowType();

	switch (flowType)
	{
		// 正常绘制
	case BaseConstants::NONE:
		pPainter.drawText(x, normalStrY, str);
		break;
		// 上浮标
	case BaseConstants::Direction::UP:
		pPainter.drawText(x, y + strAscent, str);
		break;
		// 下浮标
	case BaseConstants::Direction::DOWN:
		pPainter.drawText(x, y + rowInsideH - strDescent, str);
		break;
	}
}