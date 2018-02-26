#ifndef _IMAGE_MOUSE_INPUT_H_
#define _IMAGE_MOUSE_INPUT_H_


#pragma once
#include "BaseMouse.h"
#include "ViewObj.h"
class ImageMouseInput :public BaseMouse
{
public:
	ImageMouseInput(ViewObj * owner);
	ImageMouseInput(ViewObj * owner, BaseMouse * scoure);
	~ImageMouseInput();
public:
	int mousePress(int type, int x, int y); 
	int mouseRelease(int type, int x, int y);
	int mouseDrag(int type, int x, int y); 
	int mouseDoublePress(int type, int x, int y);
private:
	void doDoublePress(bool isAction);

};

#endif
