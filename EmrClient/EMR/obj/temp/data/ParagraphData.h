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

	// ��ȡ����һ�μ��
	float getParagraphDisTop();
	// ��ȡ����һ�μ��
	float getParagraphDisBottom();
	// ��ȡ�����м�౶��
	float getRowDisHTimes();
	// ʵ���м��
	float getRowDisHValue();
	// ��ȡ������߾�
	int getInsideLeft();
	// ��ȡ�����ұ߾�
	int getInsideRight();
	// ��ȡ��������
	int getIndentValue();
	// ��ȡ��������
	char getIndentType();

	char getHorizonAlignType();

	// ���ö��������Ҷ���
	bool setHorizonAlignType(char type, bool needReplaceRow);

	// ��������һ�μ��
	//bool setParagraphDisTop(int value);
	// ��������һ�μ��
	//bool setParagraphDisBottom(int value);
	// ���ö������м��
	bool setRowDisHTimes(float times, bool needReplaceNow);
	// ���ö�����߾�
	bool setInsideLeft(int value, bool needReplaceRow);
	// ���ö����ұ߾�
	bool setInsideRight(int value, bool needReplaceRow);
	// ������������
	bool setIndentParam(int inL, int inR, char type, int value, bool needReplaceRow);

	//void resetParam(ParagraphData * source);
	void resetIndentParams();

	void judgeLessOrMoreRowList();
	void replaceRowAignType();
	void addTxtLayerToReplaceList();

private:
	// ��������һ�μ��
	int mParagraphDisTop;
	// ��������һ�μ��
	int mParagraphDisBottom;
	// �������м�౶��
	float mRowDisHTimes;
	float mRowDisHValue;

	// �������
	int mInsideLeft;
	// �Ҳ�����
	int mInsideRight;
	// ��������
	char mIndentType;
	// ����ֵ
	int mIndentValue;

	// ������뷽ʽ
	char mHorizonAlignType;

};

#endif
