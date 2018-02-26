#include "FlagFlashLogic.h"

#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseViewParam.h"

#include "BaseTools.h"

#include "ConstantsID.h"
#include "UIConstants.h"
#include "BaseConstants.h"

FlagFlashLogic::FlagFlashLogic(BaseContainer * viewOwner) : BaseLogic(viewOwner, ConstantsID::ClassId::FLAG_FLASH_LOGIC)
{
}

FlagFlashLogic::~FlagFlashLogic()
{
}
void FlagFlashLogic::refreshLogic()
{
	clearTime();
	ViewObj * owner = (ViewObj *)mOwner;
	//if (!mOwner->canWrite())
	//{
	//	setBlock(true);
	//	mViewOwner->getViewParam()->setViewType(BaseConstants::NONE);
	//}
	//else
	{
		setBlock(false);
		owner->getViewParam()->setViewType(BaseConstants::ViewType::NORMAL);
	}
	//WriterManager * writeMgr = mViewOwner->getWriterMgr();
	//writeMgr->getAuthorityMgr()->getFileData()->canWrite();
	//->getReadWriteAuthority()
}
int FlagFlashLogic::myUpdate()
{
	//BaseTools::println("FlagFlashLogic::myUpdate() 111");
	int returnType = BaseConstants::NONE;
	if (isBlock())
	{
		return returnType;
	}
	BaseLogic::addTime();
	ViewObj * owner = (ViewObj *)mOwner;
	BaseViewParam * param = owner->getViewParam();
	if (param->getViewType() >= BaseConstants::NONE)
	{
		if (getTime() == BaseConstants::DEFAULT_FLASH_VIEW_FRAME)
		{
			param->setViewType(BaseConstants::NONE);
		}
		else if (getTime() >= BaseConstants::DEFAULT_FLASH_CIRCLE_FRAME)
		{
			clearTime();
			param->setViewType(BaseConstants::ViewType::NORMAL);
		}
		//owner->getViewParam()->setViewType(BaseConstants::ViewType::NORMAL);
	}
	//BaseTools::println("FlagFlashLogic::myUpdate() 222");

	return returnType;
}