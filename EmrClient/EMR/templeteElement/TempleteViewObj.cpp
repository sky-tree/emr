#include "TempleteViewObj.h"
#include "BaseViewParam.h"
#include "BaseTemplate.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "ExcelCellData.h"
#include "BaseMath.h"
#include "OperateMgr.h"
#include "TempleteViewObjKeyManager.h"
#include "TempleteViewObjMouseManager.h"
#include "WriterManager.h"
#include "FileData.h"
#include "StrContentData.h"
TempleteViewObj::TempleteViewObj(WriterManager * mgr, BaseTemplate * templeteParent, int objId)
: ViewObj(mgr, ConstantsID::ClassId::TEMPLATE_VIEW_OBJ_CLASS, objId, false)
//, mKeyMgr(NULL)
//, mMouseMgr(NULL)
{
	mKeyMgr = new TempleteViewObjKeyManager(this);
	mMouseMgr = new TempleteViewObjMouseManager(this);
	setTempleteParent(templeteParent);
}
TempleteViewObj::TempleteViewObj(WriterManager * mgr, TempleteViewObj* otherObj) : ViewObj(mgr, otherObj)
{
	mKeyMgr = new TempleteViewObjKeyManager(this);
	mMouseMgr = new TempleteViewObjMouseManager(this);
	setTempleteParent(otherObj->getTempParent());
}

TempleteViewObj::~TempleteViewObj()
{
	if (mKeyMgr)
	{
		delete mKeyMgr;
		mKeyMgr = NULL;
	}
	if (mMouseMgr)
	{
		delete mMouseMgr;
		mMouseMgr = NULL;
	}
}
//ObjKeyManager * TempleteViewObj::getKeyMgr()
//{
//	return mKeyMgr;
//}
//ObjMouseManager * TempleteViewObj::getMouseMgr()
//{
//	return mMouseMgr;
//}

bool TempleteViewObj::setDrawTxtParam(StrContentData* focusData)
{
	if (isObj(ConstantsID::ContainerId::EXCEL_PAGE))
	{
		ExcelCellData*	cellData = (ExcelCellData *)getContentData();
		if (cellData)
		{
			bool isSuccess;
			isSuccess = cellData->mRootTemplete->setDrawTxtParam(focusData);

			BaseTemplate* templateParent = getTempParent();
			if (templateParent->isObj(ConstantsID::ContainerId::EXCEL_TEMPLETE))
			{
				addExcelTempelateToOperate((ExcelTemplete*)templateParent);
			}
			return isSuccess;
		}
	}
	return ViewObj::setDrawTxtParam(focusData);
}