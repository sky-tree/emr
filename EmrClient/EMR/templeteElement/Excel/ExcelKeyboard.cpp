#include "ExcelKeyboard.h"
#include "qstring.h"
#include "OperateMgr.h"
#include "ExcelTemplete.h"
#include "TempleteTypeEnumeration.h"
#include "SetFocusAction.h"
#include "ConstantsID.h"
#include "BaseConstants.h"
#include "FlagParam.h"
#include "ExcelRelayoutAction.h"
//#include "FlagParam.h"
#include "StrConstants.h"
#include "QVariant.h"
ExcelKeyboard::ExcelKeyboard(ExcelTemplete * excelTemplate) : BaseTempleteKeyboard(excelTemplate, ConstantsID::ClassId::EXCEL_KEYBOARD)
, mExcelLayout(NULL)
{
	mOwnerExcelTemplete = excelTemplate;
}


ExcelKeyboard::~ExcelKeyboard()
{
	if (mExcelLayout)
	{
		mExcelLayout = NULL;
	}
	if (mOwnerExcelTemplete)
	{
		mOwnerExcelTemplete = NULL;
	}
}
void ExcelKeyboard::init()
{
	mExcelLayout = mOwnerExcelTemplete->getExcelLayout();
}

int ExcelKeyboard::keyPress(QString str, BaseElement * obj){
	
	OperateMgr *  operateMgr = mOwnerExcelTemplete->getOperateMgr();
	QList<BaseContainer *> *  chooseListT = operateMgr->getChooseList();
	int chooseListSize = chooseListT->size();
	int returnType = BaseConstants::NONE;
	if (str.compare(TempleteTypeEnumeration::EXCEL_DEVIDE_H) == 0)
	{
		if (!mOwnerExcelTemplete->mCanOperateExcelCell)
		{
			return 1;
		}
		/*if (chooseListSize!=1){
			return 1;
		}
		ViewObj * temptViewobj = (ViewObj *)chooseListT->at(0);*/
		ViewObj * excelCellView = NULL;
		if (chooseListSize>1)
		{
			//有可能是其它表格的
			return 1;
		}
		else if (chooseListSize==1)
		{
			ViewObj * tempObj1 = (ViewObj *)chooseListT->at(0);
			if (mExcelLayout->isMyCell(tempObj1))
			{
				excelCellView = tempObj1;
			}
		}
		else
		{
			ViewObj * focus = NULL;
			char dir = 0;
			mOwnerExcelTemplete->getFocusObjAndDir(focus, dir);
			ViewObj * tempObj1 = focus->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
			if (mExcelLayout->isMyCell(tempObj1))
			{
				excelCellView = tempObj1;
			}
		}
		if (excelCellView)//真，光标在当前表格页
		{
			OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
			operateMgr->createNewAction();
			//设置焦点对象操作回退
			ViewObj * preFocus = NULL;
			char preDir = 0;
			mOwnerExcelTemplete->getFocusObjAndDir(preFocus, preDir);

			if (mExcelLayout->dividCell(excelCellView, false, 2))
			{
				//设置焦点对象操作回退
				SetFocusAction*  setFocusAction = new SetFocusAction(operateMgr->getCurStepMgr());
				setFocusAction->initAction(preFocus, preDir, preFocus, preDir);
				operateMgr->addToLocalAction(setFocusAction);

				ExcelRelayoutAction* relayoutAction = new ExcelRelayoutAction(operateMgr->getCurStepMgr(), mOwnerExcelTemplete);
				operateMgr->addToLocalActionAt(relayoutAction, 1);
			}
			else
			{
				//winData->deleteAction();
			}
			
			returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}
		else
		{
			return 0;
		}


	}
	else if (str.compare(TempleteTypeEnumeration::EXCEL_DEVIDE_V) == 0)
	{
		if (!mOwnerExcelTemplete->mCanOperateExcelCell)
		{
			return 1;
		}
		/*if (chooseListSize != 1){
			return 1;
		}
		ViewObj * temptViewobj = (ViewObj *)chooseListT->at(0);*/
		ViewObj * excelCellView = NULL;
		if (chooseListSize>1)
		{
			//有可能是其它表格的
			return 1;
		}
		else if (chooseListSize == 1)
		{
			ViewObj * tempObj1 = (ViewObj *)chooseListT->at(0);
			if (mExcelLayout->isMyCell(tempObj1))
			{
				excelCellView = tempObj1;
			}
		}
		else
		{
			ViewObj * focus = NULL;
			char dir = 0;
			mOwnerExcelTemplete->getFocusObjAndDir(focus, dir);
			ViewObj * tempObj1 = focus->getParentViewById(ConstantsID::ContainerId::EXCEL_PAGE);
			if (mExcelLayout->isMyCell(tempObj1))
			{
				excelCellView = tempObj1;
			}
		}
		if (excelCellView)//真，光标在当前表格页
		{
			OperateMgr * operateMgr = mOwnerExcelTemplete->getOperateMgr();
			operateMgr->createNewAction();
			//设置焦点对象操作回退
			ViewObj * preFocus = NULL;
			char preDir = 0;
			mOwnerExcelTemplete->getFocusObjAndDir(preFocus, preDir);

			if(mExcelLayout->dividCell(excelCellView, true, 2))
			{
				//设置焦点对象操作回退
				SetFocusAction*  setFocusAction = new SetFocusAction(operateMgr->getCurStepMgr());
				setFocusAction->initAction(preFocus, preDir, preFocus, preDir);
				operateMgr->addToLocalAction(setFocusAction);

				ExcelRelayoutAction* relayoutAction = new ExcelRelayoutAction(operateMgr->getCurStepMgr(), mOwnerExcelTemplete);
				operateMgr->addToLocalActionAt(relayoutAction, 1);
			}
			else
			{
				//winData->deleteAction();
			}
			
			returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
		}
		else
		{
			return 0;
		}
		
	}
	else if (str.compare(TempleteTypeEnumeration::EXCEL_MERGER) == 0)
	{
		if (!mOwnerExcelTemplete->mCanOperateExcelCell)
		{
			return 1;
		}
		QList<ViewObj *> * chooseList = new QList<ViewObj *>();
		//chooseList->push_back((ViewObj *)mViewOwner->getChild(0));
		// chooseList->push_back((ViewObj *)mViewOwner->getChild(1));
		if (chooseListSize<2){
			returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
			return returnType;
		}
		if (!ExcelTemplete::isMyCell(chooseListT, mOwnerExcelTemplete))
		{
			return 0;
		}
		
		
		ViewObj * tempObj = NULL;
		for (int i = 0; i < chooseListSize; i++)
		{
			tempObj = (ViewObj *)chooseListT->at(i);
			chooseList->push_back(tempObj);
		}
		//ViewObj * first = (ViewObj *)mViewOwner->getChild(0);
		//ExcelCellData * cellData = mExcelLayout->getCellDataFromCell(first);
		//mExcelLayout->mCellsTempt1->clear();
		//mExcelLayout->seekCellsHaveLine(BaseConstants::PlaceType::LEFT, cellData->mLeft, mExcelLayout->mCells, mExcelLayout->mCellsTempt1);
		//ExcelCellData * cellDataTemp;
		/*for (int i = 0; i < mExcelLayout->mCellsTempt1->size(); i++){
			cellDataTemp = mExcelLayout->mCellsTempt1->at(i);
			chooseList->push_back(cellDataTemp->getViewOwner());
		}*/
	bool isMerge =	mExcelLayout->mergeCell(chooseList);
	if (isMerge)
	{
		operateMgr->clearChooseList(true);
		//chooseListT->clear();
	}
		delete chooseList;
		chooseList = NULL;
		returnType = BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	}
	else if (str.compare(StrConstants::MyKeyBoard::BACK_SPACE) == 0 || str.compare(StrConstants::MyKeyBoard::DEL) == 0)
	{
		if (chooseListSize == mExcelLayout->mCells->size())
		{
		
			ViewObj * tempObj = NULL;
			BaseContainer* tempContainer = NULL;
			bool isAllViewObj = true;
			bool isAllMyCell = true;
			for (int i = 0; i < chooseListSize; i++){
				tempContainer = chooseListT->at(i);
				if (!tempContainer->isView()){
					isAllViewObj = false;
					break;
				}
				else
				{
					tempObj = (ViewObj*)tempContainer;
					if (!mExcelLayout->isMyCell(tempObj))
					{
						isAllMyCell = false;
						break;
					}
				}
			}
			if (isAllViewObj&&isAllMyCell)
			{
				mOwnerExcelTemplete->deleteSelf();
				return 1;
			}
		}
	
	}
	else if (str.contains(TempleteTypeEnumeration::EXCEL_PROPERTY))
	{
		QStringList strs = str.split(":");
		QVariant tempValue = strs.at(1);
		bool isExelCellProperty = tempValue.toBool();
		if (isExelCellProperty)
		{
			if (ExcelTemplete::isMyCell(chooseListT, mOwnerExcelTemplete))
			{
				return 1;
			}
			else
			{
				return 0;
			}
			//单元格属性
			
		}
	}
	
	return returnType;
}
int ExcelKeyboard::keyRelease(QString str, BaseElement * obj){
	int returnType = BaseConstants::NONE;

	return returnType;
}
bool ExcelKeyboard::isSystemExcelKey(const QString & keyStr)
{
	bool isSeystemKeya = false;
	if (keyStr.contains(TempleteTypeEnumeration::SISTEM_SIGN))
	{
		isSeystemKeya = true;
	}
	return isSeystemKeya;
}

