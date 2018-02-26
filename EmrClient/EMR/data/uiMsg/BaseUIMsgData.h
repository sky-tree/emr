#ifndef BASE_UI_MSG_DATA_H_
#define BASE_UI_MSG_DATA_H_



#include "BaseElement.h"

class WriterManager;
class OperateMgr;

// UI事件(将作为信号缓存，然后在自身主线程依次执行。注意并不是即时执行!)
class BaseUIMsgData : public BaseElement
{
public:
	BaseUIMsgData(WriterManager * writerMgr, int classID);
	~BaseUIMsgData();

public:
	virtual void doAction() = 0;

protected:
	WriterManager * mWriterMgr;
	OperateMgr * mOperateMgr;

};

#endif
