#include "SetAutorityInput.h"

#include "BaseConstants.h"
//#include "NormalPageTxtWinData.h"
#include "ViewObj.h"
//#include "BaseTemplete.h"
#include "AuthorityConstants.h"
#include "WriterManager.h"
//#include "AuthorityMgr.h"
#include "FileData.h"
#include "LogicManager.h"
#include "BaseTemplate.h"
#include "TempContainerMgr.h"
#include "OperateMgr.h"
#include "CodeTempData.h"

SetAutorityInput::SetAutorityInput(WriterManager * writeMgr, SetAutotityOperateType operateType) :BaseUIMsgData(writeMgr,BaseConstants::UIInputType::SET_AUTORITITY)
//, mWriterManager(NULL)
//, mPageTxtWinData(NULL)
, mCodeId(-1)
, mAuthotity(-1)
{
	//mPageTxtWinData = (NormalPageTxtWinData*)winData;
	mOperateType = operateType;
}
SetAutorityInput::SetAutorityInput(WriterManager * writeMgr, SetAutotityOperateType operateType, QString codeId) : BaseUIMsgData(writeMgr,BaseConstants::UIInputType::SET_AUTORITITY)
//, mWriterManager(NULL)
//, mPageTxtWinData(NULL)
, mCodeId(-1)
, mAuthotity(-1)
{
//	mPageTxtWinData = (NormalPageTxtWinData*)winData;
	mOperateType = operateType;
	mCodeId = codeId;
}
SetAutorityInput::SetAutorityInput(WriterManager * writeMgr, SetAutotityOperateType operateType, int  authotity) : BaseUIMsgData(writeMgr,BaseConstants::UIInputType::SET_AUTORITITY)
//, mWriterManager(NULL)
//, mPageTxtWinData(NULL)
, mCodeId(-1)
, mAuthotity(-1)
{
	//mPageTxtWinData = (NormalPageTxtWinData*)winData;
	//mWriterManager = writeMgr;
	mOperateType = operateType;
	mAuthotity = authotity;
}
SetAutorityInput::~SetAutorityInput()
{
	//mPageTxtWinData = NULL;
	//mWriterManager = NULL;
}
void SetAutorityInput::doAction()
{
	switch (mOperateType)
	{
	case CloseAllLevelOneTempleteWriteAutority:
	{
		closeAllLevelOneTempleteWriteAutority();
	}
		break;
	case CloseAllLevelOneTempleteWriteAutorityExcept:
		closeAllLevelOneTempleteWriteAutorityExcept(mCodeId);
		break;
	case DeleteLevelOneTempleteByCodeId:
		deleteLevelOneTempleteByCodeId(mCodeId);
		break;
	case SetFileAuthority:
		setFileAuthority(mAuthotity);
		break;
		/*case LoadOtherMedicalRecord:
			loadOtherMedicalRecord(mAuthotity);
			break;*/
	default:
		break;
	}
}
void SetAutorityInput::closeAllLevelOneTempleteWriteAutority()
{
	BaseTemplate* totalRootTemp = mWriterMgr->getRootTemp();
	BaseContainer * totalTempContent = totalRootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	QList<BaseContainer *> * recordList = totalTempContent->getChildren();
	CodeTempData * data = NULL;
	for (int i = 0; i < recordList->size(); i++)
	{
		BaseContainer * temp = recordList->at(i);
		data = (CodeTempData *)temp->getContentData();
		data->setAuthority(AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
	}
}
void SetAutorityInput::closeAllLevelOneTempleteWriteAutorityExcept(QString codeId)
{

	BaseTemplate* totalRootTemp = mWriterMgr->getRootTemp();
	BaseContainer * totalTempContent = totalRootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	QList<BaseContainer *> * recordList = totalTempContent->getChildren();
	CodeTempData * data = NULL;
	bool haveSameId = false;
	for (int i = 0; i < recordList->size(); i++)
	{
		BaseContainer * temp = recordList->at(i);
		data = (CodeTempData *)temp->getContentData();
		QString tempId = temp->getCodeId();
		if (tempId == codeId)
		{
			data->setAuthority(AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT);
			haveSameId = true;
		}
		else
		{
			data->setAuthority(AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY);
		}
	}
	if (haveSameId)
	{
//		ViewObj * focus = temp->getFirstChildView();
// 		mPageTxtWinData->setPressStartOrEnd(focus, BaseConstants::Direction::LEFT, false);
// 		mPageTxtWinData->refreshPagePanelYByFlag();
	}
}
void SetAutorityInput::deleteLevelOneTempleteByCodeId(QString sourceCodeId)
{
	BaseTemplate * rootTemp = mWriterMgr->getRootTemp();
	BaseContainer * contentTemp = rootTemp->findChildrenByID(ConstantsID::ContainerId::CONTENT_TEMP, false);
	TempContainerMgr * containerChildrenMgr = (TempContainerMgr *)contentTemp->getChildrenMgr();
	BaseContainer * destContainer = containerChildrenMgr->getLastChild();
	TempContainerMgr * destChildrenMgr = (TempContainerMgr *)destContainer->getChildrenMgr();
	BaseContainer * lastContainer = contentTemp->getLastChild(false);
	containerChildrenMgr->delChild(lastContainer, true);
	destChildrenMgr->delRecordChildrenToParagraphAndView();
	BaseContainer * lastTemp = containerChildrenMgr->getLastChild();
	ViewObj * lastView = lastTemp->getLastView();
	OperateMgr * operateMgr = mWriterMgr->getOperateMgr();
	operateMgr->setPress(BaseConstants::MousePressType::PRESS_AND_RELEASE, lastView, BaseConstants::Direction::LEFT);
}
void SetAutorityInput::setFileAuthority(int authority)
{
	/*mWriterManager->getAuthorityMgr()->getFileData()->initFileReadWriteAuthority(authority);
	ViewObj * flag = mWinData->getFlagObj();
	if (flag)
	{
		flag->getLogicManager()->doRefreshLogic();
	}*/
}