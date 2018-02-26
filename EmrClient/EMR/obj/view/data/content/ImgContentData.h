#ifndef IMG_CONTENT_DATA_H_
#define IMG_CONTENT_DATA_H_



#include <QString>
#include <QImage>

#include "BaseContentData.h"

class ViewObj;
class GlobalImgData;
class BaseEditParam;

class ImgConTentData : public BaseContentData
{
public:
	ImgConTentData(BaseContainer * owner, CodeParams * codeParams);
	ImgConTentData(BaseContainer * owner, CodeParams * codeParams, QString imgName);
	ImgConTentData(BaseContainer * owner, CodeParams * codeParams, ImgConTentData * source);
	virtual~ImgConTentData();

public:
	QImage* getSnap();
	QImage* getImg();
	QString getImgName();
	BaseEditParam * getBaseEditParam();
	void setSnap(QImage * snap);
	void setWightAndHeight(int w, int h);

private:
	void init();
	void destroy();

protected:
	void saveSelfData(BaseIOTools *iOTools);
	void loadSelfData(BaseIOTools *iOTools);

private:

	ViewObj * mViewOwner;
	GlobalImgData * mImgData;
	//QString mImgName;

	//QRect * mImgRect;
	//QImage * mImage;
	// ץͼ
	QImage * mSnap;
	BaseEditParam * mBaseEditParam;
	float mScaleW;
	float mScaleH;

	bool mIsSnaping;

	//BaseTemplete * mEditChildrenTemplete;
};

#endif
