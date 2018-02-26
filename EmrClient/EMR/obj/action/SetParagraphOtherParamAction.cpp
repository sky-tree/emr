#include "SetParagraphOtherParamAction.h"

//#include "NormalPageTxtWinData.h"
//#include "ParagraphTemplete.h"
#include "BaseViewParam.h"

#include "OperateMgr.h"
#include "BaseContainer.h"
#include "ViewObj.h"
#include "BaseTemplate.h"
#include "BaseScaleParam.h"

#include "WriterManager.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

#include "ViewFactory.h"

#include "GlobalTemplate.h"

#include "MainViewContainer.h"

SetParagraphOtherParamAction::SetParagraphOtherParamAction(StepActionManger * stepActonManger) : BaseAction(stepActonManger, ConstantsID::ClassId::SET_PARAGRAPH_OTHER_PARAM_ACTION)
{
}

SetParagraphOtherParamAction::~SetParagraphOtherParamAction()
{
}
void SetParagraphOtherParamAction::setParagraphNumberView(bool pre, bool isView)
{
	/*mType = BaseConstants::SetPageParamActionType::SET_PARAGRAPH_VIEW;
	mPreParagraphNumberIsView = pre;
	mDestParagraphNumberIsView = isView;*/
}
void SetParagraphOtherParamAction::setPageTitleView(bool pre, bool isView)
{
	/*mType = BaseConstants::SetPageParamActionType::SET_TITLE_VIEW;
	mPreTitleIsView = pre;
	mDestTitleIsView = isView;*/
}
void SetParagraphOtherParamAction::setPageBottomView(bool pre, bool isView)
{
	/*mType = BaseConstants::SetPageParamActionType::SET_BOTTOM_VIEW;
	mPreBottomIsView = pre;
	mDestBottomIsView = isView;*/
}
void SetParagraphOtherParamAction::showTidy(bool pre, bool tidy)
{
	mType = BaseConstants::SetSystemParamType::SET_SHOW_TIDY;
	mPreTidy = pre;
	mDestTidy = tidy;
}
void SetParagraphOtherParamAction::setPageTitleNumView(bool pre, bool isView)
{
	/*mType = BaseConstants::SetPageParamActionType::SET_PAGE_TITLE_VIEW_MODE;
	mPrePageTitleNumView = pre;
	mDestPageTitleNumView = isView;*/
}
void SetParagraphOtherParamAction::setPageBottomNumView(bool pre, bool isView)
{
	/*mType = BaseConstants::SetPageParamActionType::SET_PAGE_BOTTOM_VIEW_MODE;
	mPrePageBottomNumView = pre;
	mDestPageBottomNumView = isView;*/
}
void SetParagraphOtherParamAction::unDo()
{
	switch (mType)
	{
	case BaseConstants::SetSystemParamType::SET_PARAGRAPH_NUM_VIEW_TYPE:
		//MainViewContainer::getInstance()->lineNumAct->setChecked(mPreParagraphNumberIsView);
		//mOperateMgr->getWriterMgr()->setParagraphNumberView(mPreParagraphNumberIsView, false);
		break;
	case BaseConstants::SetSystemParamType::SET_TITLE_VIEW_TYPE:
		//MainViewContainer::getInstance()->hideOrDisplayHeaderAct->setChecked(mPreTitleIsView);
		//((NormalPageTxtWinData *)mWinData)->setPageTitleViewType(mPreTitleIsView, false);
		break; 
	case BaseConstants::SetSystemParamType::SET_BOTTOM_VIEW_TYPE:
		//MainViewContainer::getInstance()->hideOrDisplayFooterAct->setChecked(mPreBottomIsView);
		//((NormalPageTxtWinData *)mWinData)->setPageBottomViewType(mPreBottomIsView, false);
		break;
	case BaseConstants::SetSystemParamType::SET_SHOW_TIDY:
//		MainViewContainer::getInstance()->m_pLayoutAct->setChecked(mPreTidy);
		mOperateMgr->getWriterMgr()->showTidy(mPreTidy, false);
		break;
	/*case BaseConstants::SetSystemParamType::set:
		MainViewContainer::getInstance()->hideOrDisplayTitlePageAct->setChecked(mPrePageTitleNumView);
		((NormalPageTxtWinData *)mWinData)->setPageTitleNumView(mPrePageTitleNumView, false, true);
		break;
	case BaseConstants::SetPageParamActionType::SET_PAGE_BOTTOM_VIEW_MODE:
		MainViewContainer::getInstance()->linen->setChecked(mPrePageBottomNumView);
		((NormalPageTxtWinData *)mWinData)->setPageBottomNumView(mPrePageBottomNumView, false, true);
		break;*/
	}
}
void SetParagraphOtherParamAction::reDo()
{
	switch (mType)
	{
	case BaseConstants::SetSystemParamType::SET_PARAGRAPH_NUM_VIEW_TYPE:
		//MainViewContainer::getInstance()->lineNumAct->setChecked(mDestParagraphNumberIsView);
		//((NormalPageTxtWinData *)mWinData)->setParagraphNumberView(mDestParagraphNumberIsView, false);
		break;
	case BaseConstants::SetSystemParamType::SET_TITLE_VIEW_TYPE:
		//MainViewContainer::getInstance()->hideOrDisplayHeaderAct->setChecked(mDestTitleIsView);
		//((NormalPageTxtWinData *)mWinData)->setPageTitleViewType(mDestTitleIsView, false);
		break;
	case BaseConstants::SetSystemParamType::SET_BOTTOM_VIEW_TYPE:
		//MainViewContainer::getInstance()->hideOrDisplayFooterAct->setChecked(mDestBottomIsView);
		//((NormalPageTxtWinData *)mWinData)->setPageBottomViewType(mDestBottomIsView, false);
		break;
	case BaseConstants::SetSystemParamType::SET_SHOW_TIDY:
//		MainViewContainer::getInstance()->m_pLayoutAct->setChecked(mDestTidy);
		mOperateMgr->getWriterMgr()->showTidy( mDestTidy, false);
		break;
	/*case BaseConstants::SetPageParamActionType::SET_PAGE_TITLE_VIEW_MODE:
		MainViewContainer::getInstance()->hideOrDisplayTitlePageAct->setChecked(mDestPageTitleNumView);
		((NormalPageTxtWinData *)mWinData)->setPageTitleNumView(mDestPageTitleNumView, false, true);
		break;
	case BaseConstants::SetPageParamActionType::SET_PAGE_BOTTOM_VIEW_MODE:
		MainViewContainer::getInstance()->linen->setChecked(mDestPageBottomNumView);
		((NormalPageTxtWinData *)mWinData)->setPageBottomNumView(mDestPageBottomNumView, false, true);
		break;*/
	}
}