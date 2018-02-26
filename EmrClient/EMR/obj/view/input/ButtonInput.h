#ifndef _VIEWBUTTON_H_
#define _VIEWBUTTON_H_
#pragma once
#include "ViewObj.h"
#include "BaseMouse.h"
#include "DrawButton.h"

class ButtonInput : public BaseMouse
{
public:
	ButtonInput(ViewObj * owner);
	ButtonInput(ViewObj * owner, WriterManager * writerMgr, QString resRelease, QString resPress);
	~ButtonInput();
private:
	WriterManager * mWriterMgr;
	QVector<QString> mResPath;
	QString mResRelease;
	QString mResPress;
	DrawButton * mImage;
	int			mType;
	int			mLastY;
	float		mScale;
public:
	void init(int type);
	int setMiddleButtonParam(int y,int pageH,int totalH);

	virtual	int mousePress(int type, int x, int y);
	virtual	int mouseRelease(int type, int x, int y);
	virtual	int mouseMove(int type, int x, int y);
	virtual	int mouseDrag(int type, int x, int y);
	virtual	int mouseDoublePress(int type, int x, int y);
	virtual	int mouseScroll(int angle);

	virtual void releaseFocusFromWindow();
	virtual void setScale(float scale);
private:
	bool changeMiddleButtonY(float type);
	bool isBeyondScrollPanel(float& settingY, float lastY);
	void refleshPagePanel(float settingY);
	void syncScale();
};

#endif