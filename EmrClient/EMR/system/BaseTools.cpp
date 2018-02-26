#include "BaseTools.h"

#include "qDebug.h"



#include "ViewObj.h"

#include "RowViewParam.h"

#include "CodeTempData.h"
#include "ParagraphData.h"

#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseTemplate.h"

#include "TxtWinData.h"
#include "ExcelCellData.h"
#include "StrContentData.h"


#include "BaseLayout.h"
#include "SystemParams.h"
#include "OperateMgr.h"

#include "FlagParam.h"
#include "BaseViewParam.h"

#include "BaseMath.h"
#include "BaseSystem.h"

#include "UIConstants.h"
#include "StrConstants.h"
#include "GlobalTemplate.h"


long BaseTools::mStartTime = 0;

BaseTools::BaseTools()
{
}

BaseTools::~BaseTools()
{
}

void BaseTools::openAllRowReplace(BaseContainer * obj)
{
	ViewObj * firstView = obj->getFirstChildView(true);
	ViewObj * lastView = obj->getLastChildView(true);
	BaseLayout * firstLayout = firstView->getViewParent()->getLayout();
	BaseLayout * lastLayout = lastView->getViewParent()->getLayout();
	BaseLayout * curLayout = firstLayout;
	while (curLayout != lastLayout)
	{
		curLayout->openReplace();
		curLayout = curLayout->getNextLayout(false);
	}
	lastLayout->openReplace();
}
bool BaseTools::haveExcel(QList<BaseContainer *> * list)
{
	if (!list)
	{
		return false;
	}
	int size = list->size();
	if (size < 1)
	{
		return false;
	}
	BaseContainer * obj = NULL;
	for (int i = 0; i < size; i ++)
	{
		obj = list->at(i);
		if (obj->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
		{
			return true;
		}
	}
	return false;
}
void BaseTools::setLineType(QPainter &painter, QColor * roundLineColor, int roundLineThick, char type)
{
	if (type == UIConstants::LineType::SINGLE_LINE)
		painter.setPen(QPen(*roundLineColor, roundLineThick, Qt::SolidLine, Qt::RoundCap));
	else if (type == UIConstants::LineType::SINGLE_DOT_LINE)
		painter.setPen(QPen(*roundLineColor, roundLineThick, Qt::DotLine, Qt::RoundCap));
}
bool BaseTools::drawLines(QPainter &pPainter, QColor * color, int thick, char type, int x0, int y0, int x1, int y1)
{
	if (type > BaseConstants::NONE)
	{
		BaseTools::setLineType(pPainter, color, thick, type);
		pPainter.drawLine(x0, y0, x1, y1);
	}
	return true;
}
void BaseTools::getObjListFromExcel(BaseContainer * source, QList<BaseContainer *> * destList)
{
	if (source->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
	{
		BaseContainer * excelView = source->getChild(0);
		QList<BaseContainer *> * excelPages = excelView->getChildren();
		BaseContainer * excelPage = NULL;
		for (int i = 0; i < excelPages->size();i++)
		{
			excelPage = excelPages->at(i);
			ExcelCellData *	cellData = (ExcelCellData *)excelPage->getContentData();
			if (cellData)
			{
				BaseTools::exchangeObjToViewList(cellData->mRootTemplete, destList);
				//BaseTools::exchangeObjToViewList(cellData->mRootTemplete, destList);
				//for (int j = 0; j < destList->size(); j++)
				//{
				//	BaseContainer * tempObj = destList->at(j);
				//	preValue = mPreValueList->at(i)->at(j);
				//	if (tempObj->isTxt())
				//	{
				//		data = (StrContentData *)tempObj->getContentData();
				//		setParamByType((StrContentData *)data, preValue);
				//	}
				//	else
				//	{
				//	}
				//}
			}
		}
	}
}
void BaseTools::exchangeObjToViewList(BaseContainer * source, QList<BaseContainer *> * destList)
{
	if (source->isView())
	{
		destList->push_back((ViewObj *)source);
		return;
	}
	if (source->haveChildren() < 1)
	{
		return;
	}
	QList<BaseContainer *> * children = source->getChildren();
	int size = children->size();
	for (int i = 0; i < size; i++)
	{
		exchangeObjToViewList(children->at(i), destList);
	}
}
void BaseTools::exchangeListToViewList(QList<BaseContainer *> *sourceList, int start, int num, QList<BaseContainer *> * destList)
{
	BaseContainer * obj = NULL;
	int end = start + num;
	if (num == -1)
	{
		end = sourceList->size() - start;
	}
	for (int i = start; i < end; i++)
	{
		obj = sourceList->at(i);
		exchangeObjToViewList(obj, destList);
	}
}
void BaseTools::seperateListByEnter(QList<QList<BaseContainer *> *> * seperateAllViewChildren, QList<BaseContainer *> * source)
{
	int size = source->size();
	ViewObj * obj = NULL;
	int seperateIdx = 0;
	QList<BaseContainer *> * curList = NULL;
	//if (seperateAllViewChildren->size() > 0)
	//{
	//	curList = seperateAllViewChildren->at(seperateIdx);
	//}
	//else
	deleteVectorContent(seperateAllViewChildren);
	//seperateAllViewChildren->clear();
	curList = new QList<BaseContainer *>();
	seperateAllViewChildren->push_back(curList);
	for (int i = 0; i < size; i++)
	{
		obj = (ViewObj *)source->at(i);
		curList->push_back(obj);
		if (obj->isEnter() && i < size - 1)
		{
			curList = new QList<BaseContainer *>();
			seperateAllViewChildren->push_back(curList);
			seperateIdx++;
		}
	}
}
void BaseTools::removeAllChildrenFromList(QList<BaseContainer *> * list, BaseContainer * parent)
{
	BaseContainer * obj = NULL;
	for (int i = 0; i < list->size(); i++)
	{
		obj = list->at(i);
		if (parent->isTempleteParent(obj, true))
		{
			list->removeOne(obj);
			i--;
		}
	}
}
void BaseTools::addToListWithoutSame(QList<ParagraphData *> * list, ParagraphData * data)
{
	int idx = list->indexOf(data);
	if (idx < 0)
	{
		list->push_back(data);
	}
}
QVector<QRect> BaseTools::getCurClip(QPainter &pPainter)
{
	QRect rect;

	QRegion regions = pPainter.clipRegion();
	const QVector<QRect> vecRect = regions.rects();
	QRect var;
	foreach(var, vecRect)
	{
		QRect var1 = var;
		QPoint x1 = (var1.topLeft());
		QPoint x2 = (var.topRight());
		QPoint y1 = (var.bottomLeft());
		QPoint y2 = (var.bottomRight());
	}
	return vecRect;
}
void BaseTools::setChooseColor(BaseContainer * obj, int color)
{
	if (obj->isView())
		obj->setMaskColor(color);
	else
	{
		QList<BaseContainer *> * viewChildren = BaseSystem::getInstance()->getClearTempContainerList();
		CodeTempData * data = (CodeTempData *)obj->getContentData();
		//data->refreshAllViewChildren();
		BaseTools::exchangeObjToViewList(obj, viewChildren);
		//allChildren = data->getAllViewChildren();
		for (int i = 0; i < viewChildren->size(); i++)
		{
			obj = viewChildren->at(i);
			obj->setMaskColor(color);
		}
	}
}
int BaseTools::getMaxPageNumberWidth(TxtWinData * winData, QFontMetrics  graphics, char type)
{
	int pageNum = winData->getPageListNum();
	int w = 0;
	QString str;
	// 先获取当前最大页数的宽度
	if (type == BaseConstants::SystemParams::ONLY_VIEW_PAGE_NUMBER)
	{
		if (pageNum < 10)
		{
			str = StrConstants::NUMBER_9;
		}
		else if (pageNum < 100)
		{
			str = StrConstants::NUMBER_99;
		}
		else if (pageNum < 1000)
		{
			str = StrConstants::NUMBER_999;
		}
		else if (pageNum < 10000)
		{
			str = StrConstants::NUMBER_9999;
		}
		else
		{
			str = StrConstants::NUMBER_99999;
		}
	}
	else
	{
		if (pageNum < 10)
		{
			str = StrConstants::NUMBER_9 + "/" + StrConstants::NUMBER_9;
		}
		else if (pageNum < 100)
		{
			str = StrConstants::NUMBER_99 + "/" + StrConstants::NUMBER_99;
		}
		else if (pageNum < 1000)
		{
			str = StrConstants::NUMBER_999 + "/" + StrConstants::NUMBER_999;
		}
		else if (pageNum < 10000)
		{
			str = StrConstants::NUMBER_9999 + "/" + StrConstants::NUMBER_9999;
		}
		else
		{
			str = StrConstants::NUMBER_99999 + "/" + StrConstants::NUMBER_99999;
		}
	}
	str = StrConstants::PAGE_NUMBER_FIRST_WORD + str + StrConstants::PAGE_NUMBER_LAST_WORD;
	w = graphics.width(str);
	return w;
}
QString BaseTools::judgeStrWithPageNumber(StrContentData * source, ViewObj * effectPage)
{
	QString str = source->getString();
	ViewObj * owner = (ViewObj *)source->getOwner();
	if (source->getOwner()->isPageNumber())
	{
		ViewObj * container = owner->findViewFromParent(ConstantsID::ContainerId::PAGE_TITLE, true);
		if (!container)
		{
			container = owner->getParentViewById(ConstantsID::ContainerId::PAGE_BOTTOM);
		}
		OperateMgr * operate = owner->getOperateMgr();
		int pageListNum = operate->getPagePanel()->getChildren()->size();
		SystemParams * sys = operate->getSystemParams();
		char type = 0;
		if (owner->isObj(ConstantsID::ContainerId::TITLE_PAGE_NUMBER))
		{
			type = sys->getTitleNumViewType();
		}
		else
		{
			type = sys->getBottomNumViewType();
		}
		int pageIdx = effectPage->getViewIndex();
		// 仅显示页码
		if (type == BaseConstants::SystemParams::ONLY_VIEW_PAGE_NUMBER)
		{
			str = QString::number(pageIdx + 1);
		}
		// 显示页码及总页数
		else
		{
			str = QString::number(pageIdx + 1) + "/" + QString::number(pageListNum);
		}
		str = StrConstants::PAGE_NUMBER_FIRST_WORD + str + StrConstants::PAGE_NUMBER_LAST_WORD;
	}
	return str;
}
void BaseTools::drawChildrenUnderline(QPainter &pPainter, RowViewParam * rowParam, int x, int y
	, QList<BaseContainer *> * children, int start, int end, char type, int thick)
{
	ViewObj * startObj = (ViewObj *)children->at(start);
	BaseViewParam * startParam = startObj->getViewParam();
	float startX = startParam->getX();
	x += startX;
	int size = children->size();
	int disW = 0;
	if (end == size)
	{
		end = size - 1;
		disW = 1;
	}
	ViewObj * endObj = (ViewObj *)children->at(end);
	BaseViewParam * endParam = endObj->getViewParam();
	if (disW == 1)
		disW = endParam->getTotalW();
	int w = endParam->getX() - startX + disW;
	int rowInsideTop = rowParam->getInsideTop();
	int rowInsideH = rowParam->getInsideH();
	y += rowInsideTop + rowInsideH;
	StrContentData * startData = (StrContentData *)startObj->getContentData();
	int tempColor = 0;
	switch (type)
	{
		// 单下划实线
	case UIConstants::LineType::SINGLE_LINE:
		tempColor = startData->getFontColor();
		if (tempColor == UIConstants::MyColor::GRAYER)
		{
			//QColor c(UIConstants::MyColor::BLACK);
			setLineType(pPainter, UIConstants::MyColor::BLACK_COLOR_OBJ, 1, type);
		}
		break;
		// 单下划虚线
	case UIConstants::LineType::SINGLE_DOT_LINE:
		setLineType(pPainter, startData->getFontColorObj(), 1, type);
		break;
	case BaseConstants::NONE:
		return;
	}
	pPainter.drawLine(x, y, x + w, y);
}
void BaseTools::drawChildrenBackground(QPainter &pPainter, RowViewParam * rowParam, int x, int y
	, QList<BaseContainer *> * children, int start, int end, int color)
{
	ViewObj * startObj = (ViewObj *)children->at(start);
	BaseViewParam * startParam = startObj->getViewParam();
	float startX = startParam->getX();
	x += startX;
	int size = children->size();
	int disW = 0;
	if (end == size)
	{
		end = size - 1;
		disW = 1;
	}
	ViewObj * endObj = (ViewObj *)children->at(end);
	BaseViewParam * endParam = endObj->getViewParam();
	if (disW == 1)
		disW = endParam->getTotalW();
	int w = endParam->getX() - startX + disW;

	QColor * colorObj = BaseMath::createColor(color);

	int h = rowParam->getTotalH();

	pPainter.fillRect(x, y, w, h, *colorObj);
}
void BaseTools::refreshTempContainerAndTempIdx(BaseContainer * focus, BaseTemplate *& tempParent, int & tempIdx)
{
	//ViewObj * focus = pressEnd->getFocusObj();
	//char dir = pressEnd->getDirection();

	tempIdx = focus->getTempIndex();
	//tempParent = focus->getTempParent();
	while (tempParent)
	{
		if (tempParent->isObj(ConstantsID::ContainerId::TITLE_NORMAL_TEMP_CONTAINER)
			|| tempParent->isObj(ConstantsID::ContainerId::BOTTOM_NORMAL_TEMP_CONTAINER)
			|| tempParent->isObj(ConstantsID::ContainerId::CONTENT_NORMAL_TEMP_CONTAINER)

			|| tempParent->isObj(ConstantsID::ContainerId::NODE_CONTAINER)

			|| tempParent->isObj(ConstantsID::ContainerId::EXCEL_CONTENT_NORMAL_TEMP_CONTAINER)
			)
		{
			return;
		}
		tempIdx = tempParent->getTempIndex();
		tempParent = tempParent->getTempParent();
	}
}
void BaseTools::judgeTitleOrBottomOutArea(ViewObj * layer)
{
	if (layer->isObj(ConstantsID::ContainerId::EXCEL_TXT_LAYER))
	{
		ViewObj * normalRow = layer->findViewFromParent(ConstantsID::ContainerId::NORMAL_ROW, true);
		layer->getLayout()->replaceChildren();
		layer = normalRow->getViewParent();
		if (!layer)
		{
			return;
		}
	}
	// 如果在页眉、页脚中，需判断插入后是否超出限制高度
	if (layer && layer->isObj(ConstantsID::ContainerId::TITLE_BOTTOM_TXT_LAYER))
	{
		OperateMgr * operateMgr = layer->getOperateMgr();
		layer->getLayout()->replaceChildren();
		BaseViewParam * layerParam = layer->getViewParam();
		int h = layerParam->getTotalH();
		ViewObj * page = layer->getParentViewById(ConstantsID::ContainerId::NORMAL_PAGE);
		LayoutViewParam * pageParam = (LayoutViewParam *)page->getViewParam();
		int pageInsideH = pageParam->getInsideH();
		int maxH = 0;
		SystemParams * sysParam = operateMgr->getSystemParams();
		// 如果为页眉
		if (layer->getViewParent()->isObj(ConstantsID::ContainerId::PAGE_TITLE))
		{
			maxH = sysParam->getTitleMaxH();
		}
		// 页脚
		else
		{
			maxH = sysParam->getBottomMaxH();
		}
		if (h > maxH)
		{
			operateMgr->setNeedUndo(true);
		}
	}
}
int BaseTools::getFirstNodeIdx(QList<BaseContainer *> * list, int start, int num)
{
	if (num == -1)
	{
		num = list->size() - start;
	}
	int firstNodeIdx = -1;
	int end = start + num;
	BaseContainer * child = NULL;
	for (int i = start; i < end; i++)
	{
		child = list->at(i);
		if (child->isNode())
		{
			firstNodeIdx = i;
			break;
		}
	}
	return firstNodeIdx;
}
void BaseTools::copyObjListToObjList(QList<BaseContainer *> * srcList, QList<BaseContainer *> * destList, bool isClearDestList)
{
	if ((!destList) || (!srcList))
	{
		return;
	}
	if (isClearDestList)
	{
		if (destList->size()>0)
		{
			destList->clear();
		}
	}
	int srcSize = srcList->size();
	for (int i = 0; i < srcSize; i++)
	{
		destList->push_back(srcList->at(i));
	}
}


bool BaseTools::isOverOrEqualsVersion(int cur, QString dest)
{
	int destValue = convertQStringOfVersionToInt(dest);
	return cur >= destValue;
}
int BaseTools::convertQStringOfVersionToInt(QString source)
{
	QList<QString> versionList = source.split(".");
	int size = versionList.size();
	int index = size;
	int currentInt = 0;
	int totalInt = 0;
	for (int i = 0; i < size; i++)
	{
		index--;
		currentInt = versionList.at(i).toInt();
		currentInt = currentInt*qPow(10, index);
		totalInt += currentInt;
	}
	return totalInt;
}
void BaseTools::println(QString source)
{
}
void BaseTools::println(QString source, int value)
{
	source = source + QString::number(value);
	println(source);
}
long BaseTools::refreshStartTime()
{
	mStartTime = mQTime.currentMSecsSinceEpoch();
	return mStartTime;
}
void BaseTools::printSpendTime(QString source, int value)
{
	QString dest = source+" " + QString::number(value);
	printSpendTime(dest);
}
void BaseTools::printSpendTime(long &startTime, QString source, int value)
{
	QString dest = " " + source + " " + QString::number(value);
	printSpendTime(startTime, dest);
}
void BaseTools::printSpendTime(QString source, bool needRefreshStartTime)
{
	return printSpendTime(mStartTime, source, needRefreshStartTime);
}
void BaseTools::printSpendTime(long &startTime, QString source, bool needRefreshStartTime)
{
	long curTime = mQTime.currentMSecsSinceEpoch();
	long dis = curTime - startTime;
	if (dis > 1)
	{
		qDebug() << source << " " << dis;
	}
	if (needRefreshStartTime)
	{
		startTime = mQTime.currentMSecsSinceEpoch();
	}
}