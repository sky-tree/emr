#ifndef BASE_CONTAINER_H_
#define BASE_CONTAINER_H_


#include "BaseElement.h"
#include "ConstantsID.h"
class WriterManager;
class BaseContentData;
class BaseChildrenManager;
class NodeMgr;
class NodeChildrenMgr;
class LogicManager;
class ViewObj;
class BaseTemplate;
class BaseLogic;
class BaseParagraph;
class OperateMgr;
class StrContentData;
class ParagraphData;
class ExcelTemplete;
class BaseContainer : public BaseElement
{
public:
	BaseContainer(WriterManager * writerMgr, int objID);
	BaseContainer(WriterManager * writerMgr, int classID, int objID);
	BaseContainer(WriterManager * writerMgr, BaseContainer * source);
	virtual~BaseContainer();

public:
	void initContentData(BaseContentData * data);
	void setBlockType(int blockType);
	bool isBlock(char bit);
	void initChildrenMgr(BaseChildrenManager * childrenMgr);
	void initLogicMgr(LogicManager * logicMgr);

public:
	virtual void haveDeleteFromParent(BaseContainer * parent);
	virtual void haveAddToParent(BaseContainer * parent);
	void setParent(BaseContainer * parent);
	void clearParent(int classID);
	void setTempleteParent(BaseContainer * parent);
	bool isObj(int destID);
	bool isView();
	bool isTemp();
	bool isParagraph();
	bool isTxt();
	bool isCommonTxt();
	bool isEnterImgOrHLine();
	bool isPageNumber();
	bool isRow();
	bool isExcelRow();
	char isHorizonOrEndPageRow();
	bool isEnter();
	bool isNode();
	char isHorizontalOrEndPageLine();
	bool isTitleOrBottomView();
	bool isTxtWindow();
	bool isTemplateElement();//如果是，才可以强转为baseTmplete
	bool isTempContainer();
	bool isTempContainerExceptDest(int exceptID);
	//bool isTitleBottomOrContentContainer();
	BaseTemplate * getTempParent();
	virtual BaseParagraph * getParagraphParent();
	virtual ViewObj * getViewParent();

	ViewObj * getFirstView();
	ViewObj * getLastView();

	BaseLogic * getLogicByClassID(int id);

	//int getUserID();

	short getStatus();
	void changeStatus(short status, bool circleChildren);

	bool isTempleteParent(BaseContainer * obj, bool isCircle);
	ViewObj* getFlagObj();
	void getFocusObjAndDir(ViewObj* & focusObj,char & dir);
	void savePressStartAndEnd();
	void backSetPressStartAndEnd(bool freshChooseList);
	StrContentData * getFocusStrData();
	virtual bool haveMask();
	int getMaskColor();
	void setMaskColor(int color);
	void clearMaskColor();
	QString getCodeId();
	void setCodeId(QString id);
	//void getCodeIdFromTemp();

	int getTempIndex();
	void setTempIndex(int idx);
	void refreshChildrenIndex(int start);
public:
	WriterManager * getWriterMgr();
	OperateMgr * getOperateMgr();
	void checkIsNeedUndo();
	BaseChildrenManager * getChildrenMgr();
	void setChildWriteManger(WriterManager * writeManger);
	NodeMgr * getNodeMgr();
	NodeChildrenMgr * getNodeChildrenMgr();
	QList<BaseContainer *> * getChildren();
	BaseContentData * getContentData();
	StrContentData * getStrContentData();
	ParagraphData * getParagraphData();
	BaseContainer * getChild(int idx);
	BaseContainer * getLastChild(bool isCircle);
	// -2、没有子对象管理器
	char haveChildren();
	bool haveChild(BaseContainer * child);

	int getObjID();
	//// 不断向上找，判断所在TempContainer中的索引
	//int getIndexFromTempContainer();

	BaseContainer * findChildrenByID(int objID, bool circle);
	BaseContainer * findLastContainer();
	void findNormalTempContainerFromParent(BaseContainer *&normalTempContainer,int &index);
	BaseContainer * findContainerFromChildren(int objId ,int index = -1);
	BaseContainer * findContainerFromBrothers(int objId, int index = -1);
	ViewObj * findViewFromParent(int objId, bool circle);
	BaseContainer * findTempFromParent(int objId, bool circle);
	BaseContainer * findTempFromParent(int objId, bool circle,int &index);
	ViewObj * findParentRow(bool circle);
	BaseContainer * findLastElementFromChildren(bool stopWhenFindView = false);
	BaseContainer * findFirstElementFromChildren(bool stopWhenFindView = false);


	int myUpdate();
	ViewObj * getParentViewById(int id);
	ViewObj* getFirstChildView(bool returnThisWhenNoChildren = false);
	ViewObj * getLastChildView(bool returnThisWhenNoChildren = false);
	ViewObj* getNextView(int endParentobjId = ConstantsID::ContainerId::CONTENT_TEMP);
	ViewObj* getPreView(int endParentobjId = ConstantsID::ContainerId::CONTENT_TEMP);
	bool canEditTempleteProperty();
	bool canEditTemplete();
	bool templateParentCanWrite();
	void releaseFromWindow();
	void refreshViewTxtParamOnly(StrContentData * focusTxtData, ViewObj * obj);

	BaseContainer * getChooseObj();
	int getChooseObjId();
	void setChooseObjId(int chooseObjId);
	//void setPreFocus();
	virtual void showTidy(); //整洁显示
	bool getIsShowTidy();
	int delChild(BaseContainer * obj, bool refreshIdx);
virtual	void setWriteManager(WriterManager* writeManger,bool isCircle = true);
public:
	virtual bool setDrawTxtParam(StrContentData* focusData){ return true; };
	virtual StrContentData*getSelfFocusData()
	{
		return NULL;
	};
	void addExcelTempelateToOperate(ExcelTemplete* excelTemp);
static	bool isShow(BaseContainer* container);
static	bool isShowOnNormalRow(ViewObj* view);
protected:
	ViewObj * mViewParent;
	BaseTemplate * mTempParent;
	BaseParagraph * mParagraphParent;
	WriterManager * mWriterMgr;
	BaseContentData * mContentData;
	BaseChildrenManager * mChildrenMgr;
	LogicManager * mLogicManager;
	// 阻挡类型
	int mBlockParentType;

	// 对象id
	int mObjID;
	// 状态
	short mStatus;
	// 遮挡色
	int mMaskColor;

	int mTempIndex;
	bool mNeedCopyChildrenWhenPast;
	// 拖选tempID，-1代表可直接拖选自己，>=0则向上查找temp
	int mCanChooseID;

	bool mIsShowTidy;
	//// 对象唯一编码，用于数据库查找等：默认0、无
	//QString  mCodeId;
	//// 读写权限:默认BaseConstants::NONE,自动递归找父temp的权限
	//int mWriteAuthority;
};

#endif
