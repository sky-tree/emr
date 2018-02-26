#ifndef STR_CONTENT_DATA_H_
#define STR_CONTENT_DATA_H_



#include <QString>
#include <QColor>
#include <QFont>
#include <QPainter>

#include "BaseContentData.h"

class BaseContainer;
class WriterManager;
class TxtViewParam;
class BaseTemplate;

//#include <QFontMetrics>
enum SetTxtType
{
	NONE=-1,
	FontSize,
	Family,
	Bold,
	FontColor,
	FlowType,
	FontItalic,
	UnderLineType,
	DelLineType,
	BackgroundColor,
	AlignType
};
// 文字相关属性
class StrContentData : public BaseContentData
{
public:

	StrContentData(BaseContainer * owner, CodeParams * codeParams);
	StrContentData(BaseContainer * owner, CodeParams * codeParams, QString str, int pointSize, QString family, int fontColor, int backgroundColor
		, char underLineType, short underLineThick, char delType, short delThick, bool italic, bool bold, char flowType);
	StrContentData(BaseContainer * owner, CodeParams * codeParams, QString str, StrContentData * data);

	~StrContentData();
public:
	void init();
protected:
	void saveSelfData(BaseIOTools *iOTools);
	void loadSelfData(BaseIOTools *iOTools);
public:

public:
	QString &getString();
	void setString(QString str);

	void refreshTxtParam(int dir, bool needReplaceRow);
	void refreshTxtParamBySourceParam(StrContentData * data, bool needRefreshWH);
	bool refreshTxtParamOnSetType(StrContentData * data);
	void refreshTxtParamExceptSetType(StrContentData * data, SetTxtType operateType);
	//void refreshTxtWH();
	void refreshTxtWH();
	void setTxtParamW(int w);
	//void refreshTxtWH(QFontMetrics * graphics, bool needReplaceRow);
	//void refreshTxtWH(QPainter &pPainter, bool needReplaceRow);
	void setBold(bool enable);
	bool getBold();

	void setItalic(bool italic);
	bool getItalic();
	void setFamily(QString family);
	QString getFamily();
	void setFlowType(char type);
	char getFlowType();

	void setFontColor(int color);
	void setBackgroundColor(int color);
	void setFontColor(QColor color);
	void setBackgroundColor(QColor color);
	int getFontColor();
	int getBackgroundColor();
	QColor * getFontColorObj();
	QColor * getBackgroundColorObj();

	char getUnderLineType();
	short getUnderLineThick();
	char getDelLineType();
	short getDelLineThick();
	void setUnderLine(char type, short thick);
	void setDelLine(char type, short thick);

	int getPointSize();
	void setPointSize(int pointSize);

	void setOperateType(SetTxtType type);

	void setNeedGray(bool needGray);
	bool getNeedGray();

	//int getStrW();
	//int getStrH();
	//int getSourceStrH();
	//int getAscent();
	//int getDisAscent();

	bool isSameTxtType(StrContentData * source);
	void setIsLockSelf(bool isLcok);
	void setIsSaveAndLoad(bool isSaveAndLoad);
	//void refreshPainterFont(QPainter &pPainter);
	//void refreshAppEntranceFontBySelf();
	//void initPainterAndFont(WriterManager * writ);
	void initPainterAndFont(BaseTemplate * tempTemplate);
private:

private:
	//int mStrW;
	//int mStrH;
	//int mSourceStrH;


	//int mAscent;
	//int mDisAscent;

private:
	QPainter *mPainter;
	//QFont *mFont;
	QString mString;

	QString mFontFamily;

	int mFontSize;
	bool mBold;

	bool mFontItalic;

	// 字体颜色
	int mFontColor;
	QColor * mFontColorObj;
	// 背景色
	int mBackgroundColor;
	QColor * mBackgroundColorObj;
	// 下划线类型
	char mUnderLineType;
	short mUnderLineThick;

	// 删除线类型
	char mDelLineType;
	short mDelLineThick;

	// 浮动类型：上下标
	char mFlowType;
	// 用于处理灰色字符等
	bool mNeedGray;

	TxtViewParam * mTxtViewParam;

public:
	////记录设置文字属性时，设置的是哪一种属性
	SetTxtType mOprateType;
	////是否锁定自己。锁定时，自己的属性不会因为函数调用改变
	//bool mIsLockSelf;
	//bool mIsSaveAndLoad; //元素对象时，不保存自己的属性，只保存mString
};

#endif
