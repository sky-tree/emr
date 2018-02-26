#ifndef WRITER_INPUT_MANAGER_H_
#define WRITER_INPUT_MANAGER_H_



#include "MousePressParams.h"

class WriterManager;
class OperateMgr;

#include <QString>
// 书写器整体输入控制
class WriterInputManager
{
public:
	WriterInputManager(WriterManager * writer);
	~WriterInputManager();

public:

	void addMousePress(int type, int x, int y);
	void addMouseRelease(int type, int x, int y);
	void addMouseMove(int type, int x, int y);
	void addMouseDrag(int type, int x, int y);
	void addMouseDouble(int type, int x, int y);

	void addMouseDrop(int type, int x, int y, QMap<QString, QString> * data);

	void addKeyPress(QString str, bool isSysKey = false);
	void addKeyRelease(QString str);

	void addMouseScroll(int angle);

	void keyPressOnRootView(QString str);
	void myUpdate();
public:
	MousePressParams * getCurMousePressParams();
	MousePressParams * getCurMouseDragParams();
	bool isPressRight();

private:
	void keyPress(QString str);
	void clearMouse();
	void clearKey();
public :
	int mouseDrop(int type, int x, int y, QMap<QString, QString> * data);
private:

	bool mIsPressRight;
	// 鼠标按下列表
	char mMousePressIdx;
	bool mMousePressHandling;
	MousePressParams ** mMousePressList;
	// 鼠标松开列表
	char mMouseReleaseIdx;
	MousePressParams ** mMouseReleaseList;
	// 鼠标拖动列表
	char mMouseDragIdx;
	MousePressParams ** mMouseDragList;
	// 鼠标移动列表
	char mMouseMoveIdx;
	MousePressParams ** mMouseMoveList;
	// 鼠标双击列表
	char mMouseDoubleIdx;
	MousePressParams ** mMouseDoublePressList;

	// 外面拖动进来松开
	char mMouseDropIdx;
	MousePressParams * mMouseDrop;

	// 键盘按下列表
	char mKeyPressIdx;
	bool * mIsSysKey;
	QString * mKeyPressList;

	// 键盘松开列表
	char mKeyReleaseIdx;
	QString * mKeyReleaseList;

	// 鼠标中键滚动
	char mMouseScrollIdx;
	int * mMouseScrollList;
private:
	WriterManager * mWriter;
	OperateMgr * mOperateMgr;
};


#endif
