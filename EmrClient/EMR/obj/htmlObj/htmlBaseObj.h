#ifndef _HTMLBASEOBJ_H_
#define _HTMLBASEOBJ_H_
#include "BaseElement.h"
#include "qstring.h"
#include "qpoint.h"
#include "QColor.h"
class htmlBaseObj : public BaseElement
{
public:
	htmlBaseObj(int classId);
	~htmlBaseObj();
	void setType(char type);
	char getType();
	void setStr(QString str);
	QString getStr();
	void setPicturePath(QString path);
	QString getPicturePath();
	void setWidth(int width);
	int getWidth();
	void setHeight(int height);
	int getHeight();
	void setStartPoint(int x,int y);
	void setEndPoint(int x, int y);
	QPoint getStartPoint();
	QPoint getEndPoint();
	void setFontSize(int size);
	int getFontSize();
	void setFontColor(QColor color);
	QColor getFontColor();
	void setX(int x);
	void setY(int y);
	void setItalic(bool isItalic);
	bool getItalic();
	int getX();
	int getY();
	void setBold(bool isBold);
	bool getBold();
public:
	QByteArray * getByteArray();
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	QPoint mStartPoint;
	QPoint mEndPoint;
	QString mStr;
	QColor mFontColor;
	int mFontSize;
	QByteArray* mByteArray;
	//int mFontColor;
	char mType;
	bool mIsBold;
	bool mIsItalic;
	const  float ratio = 0.8;
public:
	static const char STR = 1;
	static const char IMG = 2;
	static const char LINE = 3;
};
#endif