#ifndef UI_MSG_RESIZE_WINDOW_H_
#define UI_MSG_RESIZE_WINDOW_H_


#include <QSize>

#include "BaseUIMsgData.h"

// UI 窗口大小变化事件（需要目标窗口大小，QSize）
class UIMsgResizeWindow : public BaseUIMsgData
{
public:
	UIMsgResizeWindow(WriterManager * writerMgr, QSize size);
	~UIMsgResizeWindow();

public:
	void doAction();

protected:
	QSize mSizeParam;
};

#endif
