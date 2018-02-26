#ifndef FLAG_PARAM_H_
#define FLAG_PARAM_H_



class ViewObj;




class FlagParam
{
public:
	FlagParam();
	~FlagParam();

public:
	bool isSame(FlagParam * source);
	ViewObj * getFocusObj();
	char getDirection();

	void setParam(ViewObj * focus, char dir);
	void setParam(FlagParam * source);
	//void setDirection(char dir);
	 
protected:

protected:
	ViewObj * mFocusObj;

	char mDirection;
};

#endif
