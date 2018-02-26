#ifndef _LINE_H_
#define _LINE_H_
class Line
{
public:
	Line(int postion) :mDragPositon(0){
		mPosition = postion;
	}
	Line(Line*line){
		mPosition = line->mPosition;
		mDragPositon = line->mDragPositon;
	}
	~Line(){

	}
	void setDragPosition(int position){ mDragPositon = position; }
	void addDragPosition(int addDis){ mDragPositon += addDis; }
	void setPosition(int position){ mPosition  = position; }
	void addPostion(int addDis){ mPosition += addDis; }
	void setAllPosition(int position){ mPosition = mDragPositon = position; }
	void addAllPosition(int addDis){ mPosition += addDis; mDragPositon += addDis; }
	int  getPosition(){ return mPosition;}
	int getDragPosition(){ return mDragPositon; }
private:
int mPosition;
int mDragPositon;
};
#endif
