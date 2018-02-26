#ifndef PARAGRAPH_DATA_H_
#define PARAGRAPH_DATA_H_


#include <QList>


#include "BaseContentData.h"


class ParagraphData : public BaseContentData
{
public:
	ParagraphData(BaseContainer * owner);
	ParagraphData(BaseContainer * owner, ParagraphData * source);
	~ParagraphData();

public:
	void init();
	void init(ParagraphData * source);

protected:
	void saveSelfData(BaseIOTools *iOTools);
	void loadSelfData(BaseIOTools *iOTools);

public:

	bool isSameParagraphParamsExceptAlignType(ParagraphData * source);
	void setParagraphDataWithOutAlignTypeBySource(ParagraphData * source);

	// 获取与上一段间距
	float getParagraphDisTop();
	// 获取与下一段间距
	float getParagraphDisBottom();
	// 获取段内行间距倍数
	float getRowDisHTimes();
	// 实际行间距
	float getRowDisHValue();
	// 获取段内左边距
	int getInsideLeft();
	// 获取段内右边距
	int getInsideRight();
	// 获取缩进像素
	int getIndentValue();
	// 获取缩进类型
	char getIndentType();

	char getHorizonAlignType();

	// 设置段落左中右对齐
	bool setHorizonAlignType(char type, bool needReplaceRow);

	// 设置与上一段间距
	//bool setParagraphDisTop(int value);
	// 设置与下一段间距
	//bool setParagraphDisBottom(int value);
	// 设置段落内行间距
	bool setRowDisHTimes(float times, bool needReplaceNow);
	// 设置段落左边距
	bool setInsideLeft(int value, bool needReplaceRow);
	// 设置段落右边距
	bool setInsideRight(int value, bool needReplaceRow);
	// 设置缩进参数
	bool setIndentParam(int inL, int inR, char type, int value, bool needReplaceRow);

	//void resetParam(ParagraphData * source);
	void resetIndentParams();

	void judgeLessOrMoreRowList();
	void replaceRowAignType();
	void addTxtLayerToReplaceList();

private:
	// 本段与上一段间距
	int mParagraphDisTop;
	// 本段与下一段间距
	int mParagraphDisBottom;
	// 段落内行间距倍数
	float mRowDisHTimes;
	float mRowDisHValue;

	// 左侧缩进
	int mInsideLeft;
	// 右侧缩进
	int mInsideRight;
	// 缩进类型
	char mIndentType;
	// 缩进值
	int mIndentValue;

	// 横向对齐方式
	char mHorizonAlignType;

};

#endif
