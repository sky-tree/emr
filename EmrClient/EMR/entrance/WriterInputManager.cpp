#include "WriterInputManager.h"

#include <QDebug>




#include "WriterManager.h"
#include "ObjKeyManager.h"
#include "ObjMouseManager.h"
#include "OperateMgr.h"

#include "FlagParam.h"

#include "ViewObj.h"
#include "ExcelLayout.h"
#include "OperateMgr.h"
#include "BaseConstants.h"
#include "StrConstants.h"
#include "GlobalTemplate.h"
#include "TempleteTypeEnumeration.h"
#include "ConstantsID.h"
#include "UIConstants.h"


#include "CreateTableDlg.h"
#include "StrContentData.h"
#include "ShortcutKeyInputParam.h"
#include "UIInputManager.h"
#include "SpecialSymbol.h"
#include "Cursor.h"
WriterInputManager::WriterInputManager(WriterManager * writer)
{
	mMousePressHandling = false;
	mIsPressRight = false;
	mWriter = writer;
	mOperateMgr = mWriter->getOperateMgr();
	/********************************************** 初始化鼠标相关 *************************************************/
	mMousePressList = new MousePressParams *[BaseConstants::MAX_MOUSE_BUFFER];
	mMouseReleaseList = new MousePressParams *[BaseConstants::MAX_MOUSE_BUFFER];
	mMouseDragList = new MousePressParams *[BaseConstants::MAX_MOUSE_BUFFER];
	mMouseMoveList = new MousePressParams *[BaseConstants::MAX_MOUSE_BUFFER];
	mMouseDoublePressList = new MousePressParams *[BaseConstants::MAX_MOUSE_BUFFER];
	mMouseScrollList = new int[BaseConstants::MAX_MOUSE_BUFFER];

	for (int i = 0; i < BaseConstants::MAX_MOUSE_BUFFER; i++)
	{
		mMousePressList[i] = new MousePressParams();
		mMouseReleaseList[i] = new MousePressParams();
		mMouseDragList[i] = new MousePressParams();
		mMouseMoveList[i] = new MousePressParams();
		mMouseDoublePressList[i] = new MousePressParams();
		mMouseScrollList[i] = 0;
	}

	mMouseDrop = new MousePressParams();

	clearMouse();

	mIsSysKey = new bool[BaseConstants::MAX_KEY_BUFFER];
	mKeyPressList = new QString[BaseConstants::MAX_KEY_BUFFER];
	mKeyReleaseList = new QString[BaseConstants::MAX_KEY_BUFFER];
	for (int i = 0; i < BaseConstants::MAX_KEY_BUFFER; i++)
	{
		mKeyPressList[i].clear();
		mIsSysKey[i] = false;
	}
	clearKey();

	/********************************************** 初始化鼠标相关 *************************************************/
}


WriterInputManager::~WriterInputManager()
{
	if (mKeyPressList)
	{
		delete[] mKeyPressList;
		mKeyPressList = NULL;
	}
	if (mIsSysKey)
	{
		delete[] mIsSysKey;
		mIsSysKey = NULL;
	}
	if (mKeyReleaseList)
	{
		delete[] mKeyReleaseList;
		mKeyReleaseList = NULL;
	}
	if (mMouseScrollList)
	{
		delete[] mMouseScrollList;
		mMouseScrollList = NULL;
	}

	if (mMousePressList)
	{
		deletePointerArray(mMousePressList, BaseConstants::MAX_MOUSE_BUFFER);
		mMousePressList = NULL;
	}
	if (mMouseReleaseList)
	{
		deletePointerArray(mMouseReleaseList, BaseConstants::MAX_MOUSE_BUFFER);
		mMouseReleaseList = NULL;
	}
	if (mMouseDragList)
	{
		deletePointerArray(mMouseDragList, BaseConstants::MAX_MOUSE_BUFFER);
		mMouseDragList = NULL;
	}
	if (mMouseMoveList)
	{
		deletePointerArray(mMouseMoveList, BaseConstants::MAX_MOUSE_BUFFER);
		mMouseMoveList = NULL;
	}
	if (mMouseDoublePressList)
	{
		deletePointerArray(mMouseDoublePressList, BaseConstants::MAX_MOUSE_BUFFER);
		mMouseDoublePressList = NULL;
	}
	if (mMouseDrop)
	{
		delete mMouseDrop;
		mMouseDrop = NULL;
	}

	mWriter = NULL;
	mOperateMgr = NULL;
}
void WriterInputManager::addMousePress(int type, int x, int y)
{
	if (y < UIConstants::UI_TOOLBAR_H)
	{
		mOperateMgr->clearChooseList(true);
		return;
	}
	if (mMousePressIdx < BaseConstants::MAX_MOUSE_BUFFER - 1)
		mMousePressIdx++;

	mMousePressList[mMousePressIdx]->refresh(type, x, y);
}
void WriterInputManager::addMouseRelease(int type, int x, int y)
{
	if (y < UIConstants::UI_TOOLBAR_H)
	{
		mOperateMgr->clearChooseList(true);
		return;
	}
	if (mMouseReleaseIdx < BaseConstants::MAX_MOUSE_BUFFER - 1)
		mMouseReleaseIdx++;

	mMouseReleaseList[mMouseReleaseIdx]->refresh(type, x, y);
}
void WriterInputManager::addMouseDrag(int type, int x, int y)
{
	if (mMouseDragIdx < BaseConstants::MAX_MOUSE_BUFFER - 1)
		mMouseDragIdx++;

	mMouseDragList[mMouseDragIdx]->refresh(type, x, y);
}
void WriterInputManager::addMouseMove(int type, int x, int y)
{
	if (mMouseMoveIdx < BaseConstants::MAX_MOUSE_BUFFER - 1)
		mMouseMoveIdx++;

	mMouseMoveList[mMouseMoveIdx]->refresh(type, x, y);
}
void WriterInputManager::addMouseDouble(int type, int x, int y)
{
	if (y < UIConstants::UI_TOOLBAR_H)
	{
		mOperateMgr->clearChooseList(true);
		return;
	}
	if (mMouseDoubleIdx < BaseConstants::MAX_MOUSE_BUFFER - 1)
		mMouseDoubleIdx++;

	mMouseDoublePressList[mMouseDoubleIdx]->refresh(type, x, y);
}
void WriterInputManager::addMouseDrop(int type, int x, int y, QMap<QString, QString> * data)
{
	mMouseDropIdx = 0;
	mMouseDrop->refresh(type, x, y, *data);
}
void WriterInputManager::addMouseScroll(int scroll)
{
	if (mMouseScrollIdx < BaseConstants::MAX_MOUSE_BUFFER - 1)
		mMouseScrollIdx++;

	mMouseScrollList[mMouseScrollIdx] = scroll;
}
void WriterInputManager::addKeyPress(QString str, bool isSysKey)
{
	if (mKeyPressIdx < BaseConstants::MAX_KEY_BUFFER - 1)
		mKeyPressIdx++;
	mKeyPressList[mKeyPressIdx] = str;
	mIsSysKey[mKeyPressIdx] = isSysKey;
}
void WriterInputManager::addKeyRelease(QString str)
{
	if (mKeyReleaseIdx < BaseConstants::MAX_KEY_BUFFER - 1)
		mKeyReleaseIdx++;
	mKeyReleaseList[mKeyReleaseIdx] = str;
}
void WriterInputManager::clearMouse()
{
	mMousePressIdx = -1;
	mMouseMoveIdx = -1;
	mMouseDragIdx = -1;
	mMouseDoubleIdx = -1;
	mMouseReleaseIdx = -1;

	mMouseScrollIdx = -1;

	mMouseDropIdx = -1;
}
void WriterInputManager::clearKey()
{
	mKeyPressIdx = -1;
	mKeyReleaseIdx = -1;
}
MousePressParams * WriterInputManager::getCurMousePressParams()
{
	return mMousePressList[mMousePressIdx];
}
MousePressParams * WriterInputManager::getCurMouseDragParams()
{
	return mMouseDragList[mMouseDragIdx];
}
bool WriterInputManager::isPressRight()
{
	return mIsPressRight;
}
void WriterInputManager::keyPress(QString str)
{
	//if (ExcelKeyboard::isSystemExcelKey(str))
	//{
	//	if (str.contains(TempleteTypeEnumeration::INPUT_PATTICULAR_SIGN))
	//	{
	//		SpecialSymbol specialSymbolDialog;
	//		//specialSymbolDialog.show();
	//		if (specialSymbolDialog.exec() == QDialog::Accepted)
	//		{
	//			if (specialSymbolDialog.resultSymbol.size() > 0)
	//			{
	//				actionMgr = winData->getActionMgr();
	//				actionMgr->createNewAction();
	//				winData->addStr(specialSymbolDialog.resultSymbol);
	//			}

	//		}
	//	}
	//	return;
	//}
	//else
	//if (!mOperateMgr)
	//	mOperateMgr = mWriter->getOperateMgr();
	FlagParam * pressEnd = mOperateMgr->getPressEnd();
	if (!pressEnd)
	{
		return;
	}
	if (str.contains(TempleteTypeEnumeration::INPUT_PATTICULAR_SIGN))
	{
		SpecialSymbol specialSymbolDialog;
		//specialSymbolDialog.show();
		if (specialSymbolDialog.exec() == QDialog::Accepted)
		{
			if (specialSymbolDialog.resultSymbol.size() > 0)
			{
				OperateMgr *  operateMgr = mWriter->getOperateMgr();
				ActionMgr*	actionMgr = operateMgr->getActionMgr();
				//actionMgr->createNewAction();
				operateMgr->addStr(specialSymbolDialog.resultSymbol, true);
			}

		}
	}
	else if (str.contains(TempleteTypeEnumeration::SISTEM_SIGN))
	{
		return;
	}
	else if (str == StrConstants::MyKeyBoard::ESC)
	{
		//mOperateMgr->addEndPageLine();
		//mOperateMgr->setPaperType(UIConstants::PaperTypeAndWH::B5, true);
		//mOperateMgr->findStr(BaseConstants::SetSystemParamType::FIND_PRE_STR, "123");
		//mOperateMgr->findStr(BaseConstants::SetSystemParamType::FIND_NEXT_STR, "123");
		//mOperateMgr->replaceStr(BaseConstants::SetSystemParamType::REPLACE_PRE_STR, "123", "abc", true);
		//mOperateMgr->replaceStr(BaseConstants::SetSystemParamType::REPLACE_ALL_STR, "123", "abc", true);
		//mOperateMgr->replaceStr(BaseConstants::SetSystemParamType::REPLACE_ALL_STR, "123", "abc", true);
	}
	else if (str == StrConstants::MyKeyBoard::ENTER)
	{
		//mOperateMgr->clearChooseList(true);
		mOperateMgr->pressEnter(mOperateMgr->getUserID());
	}
	else if (str == StrConstants::MyKeyBoard::BACK_SPACE)
	{
		mOperateMgr->pressBackspace(BaseConstants::DoActionType::CREATE_STEP_AND_ADD_ACTION);
	}
	else if (str == StrConstants::MyKeyBoard::DEL)
	{
		mOperateMgr->pressDel(BaseConstants::DoActionType::CREATE_STEP_AND_ADD_ACTION);
	}
	else if (str == StrConstants::MyKeyBoard::LEFT)
	{
		mOperateMgr->clearChooseList(true);
		mOperateMgr->pressLeft();
	}
	else if (str == StrConstants::MyKeyBoard::RIGHT)
	{
		mOperateMgr->clearChooseList(true);
		mOperateMgr->pressRight();
	}
	else if (str == StrConstants::MyKeyBoard::UP)
	{
		mOperateMgr->clearChooseList(true);
		mOperateMgr->pressUp();
	}
	else if (str == StrConstants::MyKeyBoard::DOWN)
	{
		mOperateMgr->clearChooseList(true);
		mOperateMgr->pressDown();
	}
	else if (str == StrConstants::MyKeyBoard::TAB)
	{
		/*QMap<QString, QString> testMap;
		mOperateMgr->addFixTextWidget(&testMap);*/
		//mOperateMgr->addExcel(2,2,true);
	}
	else if (str == StrConstants::MyKeyBoard::SHIFT)
	{
	}
	else if (str == StrConstants::MyKeyBoard::UNDO)
	{
		mOperateMgr->doUndo();
	}
	else if (str == StrConstants::MyKeyBoard::REDO)
	{
		mOperateMgr->doRedo();
	}
	else if (str == StrConstants::MyKeyBoard::COPY)
	{
		mOperateMgr->doCopy();
	}
	else if (str == StrConstants::MyKeyBoard::CUT)
	{
		mOperateMgr->doCut();
	}
	else if (str == StrConstants::MyKeyBoard::PAST)
	{
		//for (int i = 0; i < 20; i ++)
			mOperateMgr->doPast();
	}
	else if (str == StrConstants::MyKeyBoard::SELECT_ALL)
	{
		mOperateMgr->selectAll();
	}
	else
	{
		mOperateMgr->addStr(str, true);
	}
}
int WriterInputManager::mouseDrop(int type, int x, int y, QMap<QString, QString> * data)
{
	int returnType = BaseConstants::NONE;

	//BaseWinData * winData = (BaseWinData *)mViewOwner->getContentData();
	if (data->contains(TempleteTypeEnumeration::TYPE))
	{
		//去掉上标下标
		StrContentData * tempFocusData = mOperateMgr->getFocusTxtData();
		if (!mOperateMgr->CanFileWrite())
		{
			return 1;
		}
		char flowType = tempFocusData->getFlowType();
		QString strType = data->value(TempleteTypeEnumeration::TYPE);
		QString strName = data->value(TempleteTypeEnumeration::NAME); //testMap["name"];
		tempFocusData->setFlowType(BaseConstants::NONE);

		bool iscontinue;
		mOperateMgr->preliminaryTreatmentChooseList(iscontinue);

		//ViewObj * focus = winData->getPressEnd()->getFocus();
		ViewObj * focus = NULL;
		char dir = 0;
		mOperateMgr->getFocusAndDirection(focus, dir);
		ViewObj * excelCellView = focus->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
		if (iscontinue)
		{
			mOperateMgr->clearChooseList(true);
		}
		else
		{
			mOperateMgr->clearChooseList(true);
		}
		if (!strType.compare(TempleteTypeEnumeration::EXCEL) == 0)
		{
			mOperateMgr->createNewAction();
			mOperateMgr->clearChooseList(true);
		}


		if (strType.compare(TempleteTypeEnumeration::RADIO) == 0)
		{
			mOperateMgr->addTempleteChoiceWedget(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::MULTIPLE_CHOICE) == 0)
		{
			mOperateMgr->addTempleteChoiceWedget(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::HAVE_CHOICE) == 0)
		{
			mOperateMgr->addTempleteChoiceWedget(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::UNFOLD_RADIO) == 0)
		{
			mOperateMgr->addUnfoldChoiceWedget(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::UNFOLD_MULTIPLE_CHOICE) == 0)
		{
			mOperateMgr->addUnfoldChoiceWedget(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::MENSTRUATION) == 0)
		{

			mOperateMgr->addMenStruationTemplete(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::INPUT_TEXT) == 0)
		{
			mOperateMgr->addInputTextWidget(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::KEYWORD) == 0)
		{
			//winData->addKeywordWidgent();
		}
		else if (strType.compare(TempleteTypeEnumeration::EXCEL) == 0)
		{
			//

			if (excelCellView)
			{
				return returnType;
			}

			CreateTableDlg *test = new CreateTableDlg();
			test->setModal(true);
			test->show();
			if (test->exec() == QDialog::Accepted)
			{

				QMap<QString, QString> testMap = test->getMap();
				int row = 2;
				int col = 2;
				bool isFrame = false;
				if (testMap.contains("row"))
				{
					QString str = testMap["row"];
					row = str.toInt();
				}
				if (testMap.contains("column"))
				{
					QString str = testMap["column"];
					col = str.toInt();
				}
				if (testMap.contains("isFrame"))
				{
					QString str = testMap["isFrame"];
					if (str.compare("true") == 0)
					{
						isFrame = true;
					}
				}
				bool isCanCreate = true;
				if (row == 0 || col == 0)
				{
					isCanCreate = false;

				}
				if (focus)
				{
					ViewObj* rowObj = focus->getParentViewById(ConstantsID::ContainerId::NORMAL_ROW);
					if (!ExcelLayout::canGenerateExcel(row, col, rowObj))
					{
						isCanCreate = false;
					}
				}
				else
				{
					isCanCreate = false;
				}

				if (!isCanCreate)
				{
					tempFocusData->setFlowType(flowType);
					delete test;
					return 1;
				}
				//WriterManager * tempWriteManager = mViewOwner->getWriterManager();
				WriterManager * tempWriteManager = mWriter;
				if (tempWriteManager)
				{
					UIInputManager * inputManager = tempWriteManager->getUIInput();
					if (inputManager)
					{

						mOperateMgr->createNewAction();
						ShortcutKeyInputParam * shortcutKeyParam = new ShortcutKeyInputParam(tempWriteManager, ShortcutType::GenerateExcel);
						inputManager->addMsg(shortcutKeyParam);
						shortcutKeyParam->initGenerateExcel(row, col, isFrame);
					}

				}

			}
			delete test;
		}
		else if (strType.compare(TempleteTypeEnumeration::MACRO) == 0)
		{
			mOperateMgr->addMacroTemplet(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::FIXTEXT) == 0)
		{
			mOperateMgr->addFixTextWidget(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::STRUCTURE) == 0)
		{
			mOperateMgr->addStructurationTextWidget(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::TIME) == 0)
		{
			//winData->addTimeTextWidgent(data);
		}
		else if (strType.compare(TempleteTypeEnumeration::NUMBER_FILTER) == 0)
		{
			//winData->addNumberFilterTextWidget(data);
		}
		tempFocusData->setFlowType(flowType);
	}

	return returnType;
}
void WriterInputManager::myUpdate()
{

	ViewObj * rootView = mWriter->getRootView();
	if (!rootView)
		return;
	/******************************************************* 鼠标处理 ***************************************************/
	int len = 0;
	char type = 0;
	ObjMouseManager * mouseMgr = rootView->getMouseMgr();
	if (!mMousePressHandling)
	{
		mMousePressHandling = true;
		if (mMousePressIdx >= 0)
		{
			mOperateMgr->changeStatus(BaseConstants::OperateStatus::MOUSE_PRESS_DOWN);
			len = mMousePressIdx + 1;
			//mMousePressIdx = -1;
			int chooseSize = mWriter->getOperateMgr()->getChoosedSize();
			for (int i = 0; i < len; i++)
			{
				mMousePressIdx = i;
				type = mMousePressList[i]->getType();
				// 如果非只读模式
				if (!mWriter->isFileReadOnly())
				{
					//改bug 3598
					if (type == Qt::RightButton&&chooseSize == 0)
					{
						mIsPressRight = true;
						mouseMgr->mousePress(Qt::LeftButton, mMousePressList[i]->getX(), mMousePressList[i]->getY());
						mouseMgr->mouseRelease(Qt::LeftButton, mMousePressList[i]->getX(), mMousePressList[i]->getY());
						mIsPressRight = false;
					}
				}
				mouseMgr->mousePress(type, mMousePressList[i]->getX(), mMousePressList[i]->getY());
			}
			mMousePressIdx = -1;
		}
		mMousePressHandling = false;
	}

	if (mMouseDragIdx >= 0)
	{
		len = mMouseDragIdx + 1;
		for (int i = 0; i < len; i++)
		{
			mMouseDragIdx = i;
			mouseMgr->mouseDrag(mMouseDragList[i]->getType(), mMouseDragList[i]->getX(), mMouseDragList[i]->getY());
		}
		mMouseDragIdx = -1;
	}
	if (mMouseMoveIdx >= 0)
	{
		len = mMouseMoveIdx + 1;
		mMouseMoveIdx = -1;
		int returnValue = 0;
		for (int i = 0; i < len; i++)
		{
			returnValue = mouseMgr->mouseMove(mMouseMoveList[i]->getType(), mMouseMoveList[i]->getX(), mMouseMoveList[i]->getY());
		}
		if (returnValue == BaseConstants::NONE)
		{
			Cursor::slotArrow();
		}
	}
	if (mMouseDropIdx >= 0)
	{
		mMouseDropIdx = -1;
		mouseDrop(mMouseDrop->getType(), mMouseDrop->getX(), mMouseDrop->getY(), &mMouseDrop->getData());
	}
	if (mMouseDoubleIdx >= 0)
	{
		mOperateMgr->changeStatus(BaseConstants::OperateStatus::MOUSE_PRESS_DOWN);
		len = mMouseDoubleIdx + 1;
		mMouseDoubleIdx = -1;
		for (int i = 0; i < len; i++)
		{
			mouseMgr->mouseDoublePress(mMouseDoublePressList[i]->getType(), mMouseDoublePressList[i]->getX(), mMouseDoublePressList[i]->getY());
		}
	}
	if (mMouseReleaseIdx >= 0)
	{
		len = mMouseReleaseIdx + 1;
		mMouseReleaseIdx = -1;
		for (int i = 0; i < len; i++)
		{
			//mMouseReleaseIdx = i;
			type = mMouseReleaseList[i]->getType();
			//if (type == Qt::RightButton)
			//{
			//	rootView->mousePress(Qt::LeftButton, mMouseReleaseList[i]->getX(), mMouseReleaseList[i]->getY());
			//	rootView->mouseRelease(Qt::LeftButton, mMouseReleaseList[i]->getX(), mMouseReleaseList[i]->getY());
			//}
			mouseMgr->mouseRelease(type, mMouseReleaseList[i]->getX(), mMouseReleaseList[i]->getY());
		}
		mOperateMgr->changeStatus(BaseConstants::OperateStatus::FREE);
	}
	if (mMouseScrollIdx >= 0)
	{
		len = mMouseScrollIdx + 1;
		for (int i = 0; i < len; i++)
		{
			mouseMgr->mouseScroll(mMouseScrollList[mMouseScrollIdx]);
		}
		mMouseScrollIdx = -1;
	}


	//clearMouse();

	/******************************************************* 鼠标处理 ***************************************************/

	/******************************************************* 键盘处理 ***************************************************/
	//WindowKeyManager * keyManger = mWindow->getKeyManager();
	//if (keyManger)
	{
		ObjKeyManager * keyMgr = rootView->getKeyMgr();
		if (mKeyPressIdx >= 0)
		{
			if (mOperateMgr->isFree())
			{
				len = mKeyPressIdx + 1;
				int returnValue = 0;
				for (int i = 0; i < len; i++)
				{
					if (mIsSysKey[i])
					{
						continue;
					}
					returnValue = keyMgr->keyPress(mKeyPressList[i]);
				}
				if (returnValue == 0)
				{
					for (int i = 0; i < len; i++)
						keyPress(mKeyPressList[i]);
				}
			}
		}
	}
	clearKey();
	/******************************************************* 键盘处理 ***************************************************/
}
void WriterInputManager::keyPressOnRootView(QString str)
{
	ViewObj * rootView = mWriter->getRootView();
	if (!rootView)
		return;
	ObjKeyManager * keyMgr = rootView->getKeyMgr();
	keyMgr->keyPress(str);
}