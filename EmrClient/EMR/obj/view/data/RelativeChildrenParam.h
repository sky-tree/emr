#ifndef RELATIVE_CHILDREN_PARAM_H_
#define RELATIVE_CHILDREN_PARAM_H_



class ViewObj;

class RelativeChildrenParam
{
public:
	RelativeChildrenParam(ViewObj * source, char minRelativeDir, ViewObj * minReferOb);
	RelativeChildrenParam(ViewObj * source, char minRelativeDir, ViewObj * minReferObj, char maxRelativeDir, ViewObj * maxReferObj, int * minValue);

	~RelativeChildrenParam();


public:
	void replace(bool needAddToReplace = false);
	float getX();
	float getY();

private:
	float getAreaValue(float source);
	void replaceSingle();
	void replaceBetween(bool needAddToReplace = false);

private:
	ViewObj * mSourceObj;
	char mMinRelativeDir;
	ViewObj * mMinReferObj;
	char mMaxRelativeDir;
	ViewObj * mMaxReferObj;
	// 最小值，不能低于这个值:NULL代表不限制
	int * mMinValue;
};

#endif
