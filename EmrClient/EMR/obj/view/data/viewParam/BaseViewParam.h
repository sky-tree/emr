#ifndef BASE_VIEW_PARAM_H_
#define BASE_VIEW_PARAM_H_



#include "BaseContainerParam.h"

class BaseScaleParam;
class ViewObj;

class BaseViewParam : public BaseContainerParam
{
public:
	BaseViewParam(ViewObj * owner);
	BaseViewParam(ViewObj * owner, int classID);
	BaseViewParam(ViewObj * owner, BaseViewParam * source);
	~BaseViewParam();


public:
	void initParams(ViewObj * view, BaseViewParam * source);
	void init(float x, float y, int w, int h);
	void initDefaultWH(short w, short h);
	void initClip(int * clip);

	bool isView();
	char getViewType();
	void setViewType(char type);

	int * getClip();

	void setX(float value);
	void setY(float value);
	void addY(float value);
	bool setSelfW(int value, bool needAddToReplace = false);
	virtual bool setSelfH(int value, bool needAddToReplace = false);
	bool setSelfW(int value, char childrenOrParentType, bool needAddToReplace = false);
	virtual bool setSelfH(int value, char childrenOrParentType, bool needAddToReplace = false);

	float getX();
	float getY();
	float getAbsX(ViewObj * relativeObj);
	float getAbsY(ViewObj * relativeObj);
	int getSelfW();
	int getSelfH();
	float getTotalW();
	float getTotalH();

	short getDefaultW();
	short getDefaultH();

	void addRefreshWByChildren(bool circle, bool needAddToReplaceList);
	virtual void refreshWByChildren(bool circle);
	virtual void refreshHByChildren(bool circle);
	void refreshWByParent(char childrenOrParentType, bool needAddToReplace = false);
	void refreshHByParent(char childrenOrParentType, bool needAddToReplace = false);

	void initScaleParam(BaseScaleParam * scaleParam);
	BaseScaleParam * getBaseScaleParam();

protected:
	ViewObj * mViewOwner;
	BaseScaleParam * mScaleParam;

	char mViewType;

	float mX;
	float mY;
	int mSelfW;
	int mSelfH;

	short mDefaultW;
	short mDefaultH;

private:
	int * mClip;
};

#endif
