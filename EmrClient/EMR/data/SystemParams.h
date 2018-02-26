#ifndef _SYSTEM_PARAMS_H_
#define _SYSTEM_PARAMS_H_


#include <QString>

class BaseIOTools;

class WriterManager;
class OperateMgr;
class BaseContainer;
class ViewObj;



class PatientInfo;
class UserInfo;

class CodeParams;


// 鼠标点击参数
class SystemParams
{
public:
	SystemParams(OperateMgr * operateMgr);
	~SystemParams();

public:
	void doSave(BaseIOTools * ioTools);
	void doLoad(BaseIOTools * ioTools);

	void initUserInfo(UserInfo * source);
	void initPatientInfo(PatientInfo * source);

	UserInfo * getCurrentUserInfo();
	PatientInfo * getPatientInfo();

	QString getVersion();

	//static bool isOverOrEqualsVersion(QString dest);

	//void setPaperType(short type);
	bool isPaperHorizontal();
	char getRowBottomLineType();
	short getPaperType();

	void setTitleCanWriteByViewSide(bool canWrite);
	bool canTitleWriteByViewSide();
	void setBottomCanWriteByViewSide(bool canWrite);
	bool canBottomWriteByViewSide();


	bool refreshTitleNumNext(BaseContainer * obj);
	bool refreshTitleNumNext(QList<BaseContainer *> * obj, int start, int num);
	bool refreshBottomNumNext(BaseContainer * obj);
	bool refreshBottomNumNext(QList<BaseContainer *> * obj, int start, int num);

	void refreshTitleAndBottomNumIndex();

	int getPageInsideTop();
	int getPageInsideBottom();
	int getPageInsideLeft();
	int getPageInsideRight();
	int getTitleMaxH();
	int getBottomMaxH();

	ViewObj * getTitlePageNumber();
	ViewObj * getBottomPageNumber();

	bool isTitleView();
	bool isBottomView();
	char getTitleNumViewType();
	char getBottomNumViewType();
	bool isParagraphNumView();

	bool isTidy();

	bool haveChanged(bool isHorisontal, short paperType, int pageInsideT, int pageInsideB, int pageInsideL, int pageInsideR, int titleMaxH, int bottomMaxH);
	char setPageParams(bool isHorisontal, short paperType, int pageInsideT, int pageInsideB, int pageInsideL, int pageInsideR, int titleMaxH, int bottomMaxH);

	void setTitleView(bool isView, bool absSet = false);
	void setBottomView(bool isView, bool absSet = false);
	void setTitleNumberViewType(char type, bool isAbs);
	void setBottomNumberViewType(char type, bool isAbs);
	//void setTitleNumberViewTypeOnly(char type);
	//void setBottomNumberViewTypeOnly(char type);
	void setParagraphNumberView(bool isView, bool absSet = false);

	void setTidy(bool tidy);
	void setRowBottomLineType(char type);

	void initParams(bool needCreateNumber = true);
	void initTitleOrBottomPageNumber(bool needCreateNumber);

	ViewObj * createTitleNumber(WriterManager * writer, int tempIdx);
	ViewObj * createBottomNumber(WriterManager * writer, int tempIdx);

	void initTitleAndBottomView();
	int getVersionInt();
private:
	void saveUserInfo(BaseIOTools * ioTools);
	void savePatientInfo(BaseIOTools * ioTools);
	void loadUserInfoList(BaseIOTools * ioTools);
	void loadPatientInfo(BaseIOTools * ioTools);

	void saveFirstParamsWithDifferentVersions(BaseIOTools * ioTools);
	void loadFirstParamsWithDifferentVersions(BaseIOTools * ioTools);
	void saveNormalParamsWithDifferentVersions(BaseIOTools * ioTools);
	void loadNormalParamsWithDifferentVersions(BaseIOTools * ioTools);

	void setDestSaveVersion(int dest);

private:
	void showTitle();
	void hideTitle();
	void showBottom();
	void hideBottom();

	//void showTitleNumber(bool absSet = false);
	//void hideTitleNumber(bool absSet = false);
	//void showBottomNumber(bool absSet = false);
	void showParagraphNumber();
	void hideParagraphNumber();
private:
	OperateMgr * mOperateMgr;

	BaseContainer * mTitleTempContainer;
	BaseContainer * mBottomTempContainer;
	// 版本号
	QString mVersion;
	int mVersionInt;
	int mDestSaveVersionInt;


	// 文件提交类型
	char mMarkType;


	bool mIsPaperHorizontal;
	char mRowBottomLineViewType;
	short mPaperType;


	PatientInfo * mPatientInfo;

	// 当前医生
	UserInfo * mCurrentUserInfo;
	// 历史用户数据列表，包括创建者
	QList<UserInfo *> * mUserInfoList;


	int mPageInsideTop;
	int mPageInsideBottom;
	int mPageInsideLeft;
	int mPageInsideRight;
	int mTitleMaxH;
	int mBottomMaxH;

	bool mIsTitleView;
	bool mIsBottomView;
	char mTitleNumberViewType;
	char mBottomNumberViewType;
	bool mIsParagraphNumberView;
	// 是否整洁模式
	bool mIsTidy;

	ViewObj * mTitleNumber;
	ViewObj * mBottomNumber;
	ViewObj * mTitleNumNext;
	ViewObj * mBottomNumNext;
	int mTitleNumTempIdx;
	int mBottomNumTempIdx;

	ViewObj * mTitleView;
	ViewObj * mBottomView;


	// 绘制时是否可写，此属性不存档。用于区分不同模式页眉页脚是否可编辑等情况
	bool mTitleCanWriteByViewSide;
	bool mBottomCanWriteByViewSide;
};

#endif // _SYSTEM_PARAMS_H_
