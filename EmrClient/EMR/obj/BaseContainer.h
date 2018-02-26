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
	bool isTemplateElement();//����ǣ��ſ���ǿתΪbaseTmplete
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
	// -2��û���Ӷ��������
	char haveChildren();
	bool haveChild(BaseContainer * child);

	int getObjID();
	//// ���������ң��ж�����TempContainer�е�����
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
	virtual void showTidy(); //������ʾ
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
	// �赲����
	int mBlockParentType;

	// ����id
	int mObjID;
	// ״̬
	short mStatus;
	// �ڵ�ɫ
	int mMaskColor;

	int mTempIndex;
	bool mNeedCopyChildrenWhenPast;
	// ��ѡtempID��-1�����ֱ����ѡ�Լ���>=0�����ϲ���temp
	int mCanChooseID;

	bool mIsShowTidy;
	//// ����Ψһ���룬�������ݿ���ҵȣ�Ĭ��0����
	//QString  mCodeId;
	//// ��дȨ��:Ĭ��BaseConstants::NONE,�Զ��ݹ��Ҹ�temp��Ȩ��
	//int mWriteAuthority;
};

#endif
