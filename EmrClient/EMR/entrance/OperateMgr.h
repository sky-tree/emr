#ifndef OPERATE_MGR_H_
#define OPERATE_MGR_H_


#include <QList>
#include <QString>
#include <QDateTime>
#include <qmap.h>


class BaseIOTools;
class WriterManager;
class FlagParam;

class FileData;

class SystemParams;

class BaseChildrenManager;
class BaseContainer;
class ViewObj;
class BaseTemplate;
class BaseParagraph;
class LayoutViewParam;
class CodeParams;

class AutoSaveMgr;

class ActionMgr;
class StepActionManger;
class TxtWinData;
class StrContentData;
class ParagraphData;
class BaseViewParam;
class BaseAction;
class ExcelTemplete;
class ExcelCellData;
class ExcelVerticalLayoutAction;

class QPrinter;

enum ActionTypeOnDeleteObj
{
	ChooseDelete = 1,
	Delete,
	BackSpace
};
class OperateMgr
{
public:
	OperateMgr(WriterManager * writer, BaseIOTools * ioTools);
	~OperateMgr();

public:

	void myUpdate();

	void changeStatus(char status);
	// 存档
	//void doSaveToLocalFile();
	void doSaveToByteArry(QByteArray&byarry, bool needCloseWriteAuthority);
	void doSaveDataOnly(bool needCloseWriteAuthority);

	void autoSave();

	// 读档
	void doLoad1(bool isLoad);
	void doInsertRecord(QString code, bool isCreate);

	//void activeRecordFromRecordList(QString recordID);
	void activeRecordFromRecordListByFocus(QString & recodeId);

	QByteArray getByteArray();

	BaseContainer * getTitleTemp();
	BaseContainer * getBottomTemp();
	WriterManager * getWriterMgr();
	SystemParams * getSystemParams();
	FileData * getFileData();
	int getVersionInt();
	QString getVersion();

	void clearView(bool needAddFlagToLogicList);
	void clearRootTempAndView();
	ViewObj * getFlag();
	ViewObj * getFocus();
	//一下记录了前一次的焦点位置
	void setFocusOfEndPress(ViewObj * foucus, char dir);
	void getFocusAndDirection(ViewObj * &focus, char &dir);
	void getFocusAndDirectionAndPagePanY(ViewObj * &focus, char &dir, BaseViewParam *& pagePanelViewParam, int &pagePanY);
	void setPress(char pressType, ViewObj * focus, char dir, bool isFreshStrData = true);
	//void setPreFocus();
	void savePressStartAndEnd(char saveType = 0); //0 表示都保存，1表示保存 start，2表示end 
	void backSetPressStartAndEnd(bool freshChooseList,char saveType = 0);
	void savePressStartAndEnd2(char saveType = 0); //0 表示都保存，1表示保存 start，2表示end 
	void backSetPressStartAndEnd2(bool freshChooseList, char saveType = 0);
	void checkIsNeedUndo();
	int getFocusRowInsideW();

	void refreshFlagLogic();

	FlagParam * getPressStart();
	FlagParam * getPressEnd();

	ViewObj * getWindow();
	TxtWinData * getWinData();

	ViewObj * getPagePanel();
	// 设置当前逻辑激活页，用于对象跨页输入、排版等处理
	void setEffectLogicPage(ViewObj * source);
	ViewObj * getEffectLogicPage();
	// 设置当前绘制激活页，用于跨页绘制处理
	void setEffectLoopPage(ViewObj * source);
	ViewObj * getEffectLoopPage();
	// 设置文件读写权限
	bool setFileAuthority(int authority);

	void setTitleView(bool isView);
	void setBottomView(bool isView);
	// 显示页眉中的页码
	void setTitleNumberViewType(char type);
	// 显示页脚中的页码
	void setBottomNumberViewType(char type);
	// 设置段落号是否显示
	void setParagraphNumberView(bool isView);

	// 设置当前托选项是否反序
	void setUpsidedown(bool upsidedown);
	//void setSaving(bool saving);

	bool isFree();
	bool isSaving();

	bool findStr(short type, QString str);
	bool replaceStr(short type, QString findStr, QString replaceStr, bool needCreateAction);

	char findContainerChildren(QList<BaseContainer *> * children, char type, QString findStr, int startIdx,int & startIndexInSourceChldren,QString replaceStr="");
	char findTemplateChildren(BaseContainer * templateObj, char type, QString findStr, QString replaceStr = "");
	// 按左键
	void pressLeft();
	// 按右键
	void pressRight();
	// 按上键
	void pressUp();
	// 按下键
	void pressDown();

	// 按回车
	void pressEnter(int userID, bool needCreateAction = true);
	// 按backspace键
	int pressBackspace(char addActionType, bool isJudgeType = true);
	// 按del键
	int pressDel(char addActionType, bool isJudgeType = true);
	// 动作撤销
	void doUndo();
	// 动作重做
	void doRedo();
	// 复制
	void doCopy();
	// 剪切
	void doCut();
	// 粘贴
	void doPast();

	void addImgScaleAction(BaseViewParam * viewParam, int w, int h);			//图片缩放动作

	// 刷新焦点文字属性
	void refreshFocusTxtData();
	StrContentData * getFocusTxtData();


	// 设置段落属性
	void setFocusParagraphIndentParam(int inL, int inR, char indentType, int indentValue);
	// 设置横向对齐方式
	void setHorizonAlignType(char type);
	// 设置段落行间距
	void setRowDisTimes(float times);

	void repointFocusParagraphDataAndExcelVerticalAlignType(ViewObj * focus);
	ParagraphData * getFocusParagraphData();

	// 当前是否为整洁显示
	bool isOnShowTidy();
	// 设置是否整洁显示
	void showTidy(bool isTidy);
	// 打印
	void doPrint();
	//加入系统当前时间
	void addCurrentTime();

	QString getFocusMedicalCodeID();

	void clearAllAction();


	void addNoTitleNode(QString nodeID, QString nodeName, int level);
	void addTitleNode(QString nodeID, QString nodeName, int level, QString titleName);


	void setTitleOrBottomCanWrite(int id, bool canWrite);

	/********************************************************* 文字属性设置 开始 **************************************************************************/
	// 设置表格内段落对齐:BaseConstants::Direction::UP,DOWN,MIDDLE
	void setExcelParagraphAlignVerticalType(int dir);
	ExcelVerticalLayoutAction * createSetExcelLayoutAction( QList<ExcelCellData *>*changeCells, QList<int> unDoPositionList, QList<int> redoPositionList, bool & isSuccess);
	// 设置字体
	void setFontFamily(QString family);
	// 设置粗体
	void setFontBold(bool bold);
	// 设置斜体
	void setFontItalic(bool italic);
	// 设置字体大小
	void setFontSize(int size);
	// 设置字体颜色
	void setFontColor(int color);
	// 设置字体背景色
	void setBackgroundColor(int color);
	// 设置上下标
	void setFlowType(char type);
	// 设置下划线:type、下划线类型（暂时只有UIConstants::LineType::SINGLE_LINE）;value、下划线像素厚度
	void setUnderlineParam(char type, short value);
	// 设置删除线:type、下划线类型（暂时只有UIConstants::LineType::SINGLE_LINE）;value、下划线像素厚度
	void setDelLineParam(char type, short value);
	/********************************************************* 文字属性设置 结束 **************************************************************************/

	void resizeWindow(int w, int h);
	char setPageParams(bool needAddAction, bool isHorizontal, short paperType, int pageInsideTop, int pageInsideBottom, int pageInsideLeft, int pageInsideRight, int titleMaxH, int bottomMaxH);
	void getPageParams(bool &isHorizontal, short &paperType, int &pageInsideTop, int &pageInsideBottom, int &pageInsideLeft, int &pageInsideRight, int &titleMaxH, int &bottomMaxH);

	//bool setPaperType(short type, bool needAddAction);

	// 全选：ctrl+A
	void selectAll();

	/********************************************************* 刷新拖选项 开始 ****************************************************************************/
	void clearChooseList(bool needChangeStatus);
	void refreshChooseList(bool needChangeStatus, bool needJudgePrePress, bool needRefreshUpsidedown);
	bool isChoosed(BaseContainer *obj);
	int getChoosedSize();
	QList<BaseContainer *> *getChooseList();
	bool hadExcelCellInChooseList();
	/********************************************************* 刷新拖选项 结束 ****************************************************************************/
	int getUserID();
	// 输入字符串
	void addStr(QString str, bool needCreateAction, bool needJudgeFileType = true);
	void addImg(QString str);
	void addFixTextWidget(QMap<QString, QString> *testMap);
	BaseContainer * createNodeTile(CodeParams * codeParams);
	void addStructurationTextWidget(QMap<QString, QString> *testMap);
	void addInputTextWidget(QMap<QString, QString> *testMap);
	void addMenStruationTemplete(QMap<QString, QString> *testMap);
	void addMacroTemplet(QMap<QString, QString> *testMap);
	void addTempleteChoiceWedget(QMap<QString, QString> *testMap);
	void addUnfoldChoiceWedget(QMap<QString, QString> *testMap);
	void addExcel(int row, int col, bool isShowBorder);
	bool addHorizontalLine();
	bool addEndPageLine();
	bool setRowBottomLineType(char type, bool needAddAction, bool needRefreshToolbar);
	char getRowBottomLineType();

	BaseContainer * getPageByIndex(int pageIndex);

	void setNeedUndo(bool needUndo);
	bool getNeedUndo();
	void refreshPagePanelByFocus();
	void setNeedRefreshPagePanelYByFlag(bool needRefresh);

	int * getTotalClip();
	void refreshTotalClipInside(int x, int y, int w, int h);
	void refreshTotalClipOutside(int x, int y, int w, int h);
	void clearTotalClip();

	BaseParagraph * getContentParagraphByIndex(int idx);
public:
	// 根据文字及段落属性刷新toolbar按钮
	void refreshToolbarParams();

	void setDestEffectLogicPageIndex(int idx);
	void refreshEffectLogicPageByDestIndex();

public:
	void addToChooseList(BaseContainer * obj);

	bool needRefreshChooseList();

	void judgeListLess(ViewObj * first, ViewObj * last, bool needOpenFirstRowReplace);
	void judgeListOverflow(ViewObj * first, ViewObj * last, bool needOpenFirstRowReplace);
	void judgeListLess(QList<BaseContainer *> * list, bool needOpenFirstRowReplace);
	void judgeListOverflow(QList<BaseContainer *> * list, bool needOpenFirstRowReplace);
	/********************************************************* 刷新拖选项 结束 *********************************************************************/

	bool judgeHaveOtherStrParamTxt(char type, QString dest);
	bool judgeHaveOtherParamTxt(char type, int dest);

	// 当文档不以enter结尾时，代表出现问题，则自动删除最后一个enter后的内容
	bool removeObjAfterLastEnter(BaseChildrenManager * mgr);
private:
	/********************************************************* 刷新拖选项 开始 **********************************************************************/
	void addRowChildrenToChooseList(ViewObj * startLayer, ViewObj * endLayer, int startPageIdx, int endPageIdx, int startRowIdx, int endRowIdx
		, int startObjIdx, int endObjIdx);
	void addToChooseList(BaseContainer * parent, int startObjIdx, int endObjIdx);
private:
	void addAllContentToViewSideWithOutParagraph();
	// 根据当前焦点对象来刷新焦点段落数据
	void repointFocusParagraphData(ViewObj * focus);
	void repointFocusExcelVerticalAlignType(ViewObj * focus);

	bool refreshChooseListUpsidedown(ViewObj * startFocus, char startDir, ViewObj * endFocus, char endDir, ViewObj * startRow, ViewObj * endRow);
	//bool judgeCanDelObj(int operateType, BaseContainer * obj);
	bool judgeCanDelChooseList();
	//bool judgeCanDel();
	bool judgeCanDelFixTempOnMedicalRecod(BaseContainer * obj);
	bool judgeCanDelObj(BaseContainer * obj, ActionTypeOnDeleteObj actionType);
	bool judgeCanDelEnter(BaseContainer * obj);
	int delChooseList(char addActionType, bool isJudgeType);

	ViewObj * refreshNearestObj(char dir);

	void resetFocusWhenCopy();
	bool isFocusInChooseList();

	bool judgeCurFocusCanWrite(bool isPressBackspace, bool needJudgeFileType);
	bool judgeCurFocusCanWrite(ViewObj * focus, char dir, BaseTemplate *& tempParent, int & tempIdx, bool isPressBackspace
		, bool needJudgeFileType);


	int getWinMaxY(LayoutViewParam * winParam, int & winH);
	//void refreshTitleTemp();
	//void refreshBottomTemp();

	//void addTestImg();

	void addNode(QString nodeID, QString nodeName, int level, CodeParams * titleParams);


private:
	void setTxtNumParams(char type, int value);
	void doSetFontBold(QList<BaseContainer *> * viewList, int value);
	void doSetFontItalic(QList<BaseContainer *> * viewList, int value);
	void doSetFontSize(QList<BaseContainer *> * viewList, int fontSize);
	void doSetFontColor(QList<BaseContainer *> * viewList, int color);
	void doSetBackgroundColor(QList<BaseContainer *> * viewList, int color);
	void doSetFlowType(QList<BaseContainer *> * viewList, int type);
public:
	ActionMgr* getActionMgr();
	StepActionManger * getCurStepMgr();
	void createNewAction();
	void deleteAction();
	void addToLocalAction(BaseAction * action);
	void addToLocalActionAt(BaseAction * action, int executeOrder = 0);
	QList<BaseAction *> * getLocalAction();
	BaseAction * getLocalActionLastAction();
public:
	void preliminaryTreatmentChooseList(bool& isContinue);
	bool CanFileWrite();
	QList<StrContentData *> * getBufDrawParamList();
	void loadOtherMedicalRecord(QString id, QByteArray * source);
	void loadOtherMedicalRecord(QString id, QDataStream * source);

	void gotoMedicalRecord(QString id);
	void refreshMiddleButtonPlace(float y, float pageH, float totalH);
	void addExcelTempelateToOperate(ExcelTemplete* excelTemp);
	void refreshExcel();
	void clearBufferExcelTemp();


	void setPaperTypeBySystemParam(QPrinter& printer);

private:
	// 添加页眉处的医院标题栏，禁止修改
	void addHospitalTitle(BaseContainer * titleContainer, bool isLoad);
	//如果再表格判断是否加入
	bool isContinueIfInExcelCell(ViewObj * focus,int maxWidthOfAddView);
	int findMaxWidthInTemplate(QList<BaseContainer *> &sourceList);
	bool canAddTemplateList(QList<BaseContainer *> &sourceList, ViewObj* focus);
	bool canAddTemplate(BaseContainer * addContainer, ViewObj* focus);
public:
	bool canExcelCellContainObjList(QList<BaseContainer *> &sourceList, ViewObj* focusInAddPage);
	bool canExcelCellContainObj(BaseContainer * addContainer, ViewObj* focusInAddPage);
private:
	WriterManager * mWriter;

	BaseIOTools * mIOTools;

	FileData * mFileData;

	SystemParams * mSystemParams;
	// 病程记录插入病程时，临时保存的新插入病程的系统信息
	SystemParams * mTempSystemParams;
	// 当前选中项列表
	QList<BaseContainer *> * mChooseList;
	// 当前文字属性
	StrContentData * mFocusTxtData;
	// 当前段落属性
	ParagraphData * mFocusParagraphData;
	// 当前表格内段落纵向对齐属性
	int mFocusParagraphVerticalAlignType;

	// 自动存档管理器
	AutoSaveMgr * mAutoSaveMgr;

	// 光标
	ViewObj * mFlag;
	// 按下光标
	FlagParam * mPressStart;
	FlagParam * mPrePressStart;
	FlagParam * mSavedPressStart;
	FlagParam * mSavedPressStart2;
	// 松开光标
	FlagParam * mPressEnd;
	FlagParam * mPrePressEnd;
	FlagParam * mSavedPressEnd;
	FlagParam * mSavedPressEnd2;
	////设置光标时，记录上一下的光标位置
	//FlagParam * mPreFlag;

	ViewObj * mWindow;
	TxtWinData * mWinData;

	ViewObj * mPagePanel;

	// 当前有效逻辑页（鼠标点中的页等）
	ViewObj * mEffectLogicPage;
	// 当前有效循环页（正在循环输入、绘制的页）
	ViewObj * mEffectLoopPage;

	int mDestEffectLogicPageIndex;

	// 当前显示范围:x、y、r、b
	int * mTotalClip;


	// 动作管理器
	ActionMgr * mActionMgr;

	// 判断本步骤是否需要返回，表示此步骤不合理，比如页眉中加入超出高度的内容
	bool mNeedUndo;
	// 是否需要根据flagY刷新pagePanel，每帧自动判断
	bool mNeedRefreshPagePanelYByFlag;

	char mStatus;
	// 拖选是否反向
	bool mIsUpsidedown;

	QList<StrContentData *> * mDrawParamList;
	QList<ExcelTemplete *> * mExcelTempelateListForFresh;
};
#endif
