#ifndef MOUSE_PRESS_PARAMS_H_
#define MOUSE_PRESS_PARAMS_H_



#include <QMap>
#include "qstring.h"
// 鼠标点击参数
class MousePressParams
{
public:
	MousePressParams();
	~MousePressParams();

public:
	int getX();
	int getY();
	int getType();
	void refresh(int type, int x, int y);
	void refresh(int type, int x, int y, QMap<QString, QString>  data);

	QMap<QString, QString>  getData();

private:
	int mType;
	int mX;
	int mY;

	QMap<QString, QString>  mData;
};

#endif
