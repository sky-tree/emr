#ifndef _SET_AUTORITY_INPUT_H_
#define _SET_AUTORITY_INPUT_H_
#include "BaseUIMsgData.h"
enum SetAutotityOperateType
{
	CloseAllLevelOneTempleteWriteAutority,
	CloseAllLevelOneTempleteWriteAutorityExcept,
	DeleteLevelOneTempleteByCodeId,
	SetFileAuthority
};
//class NormalPageTxtWinData;
class WriterManager;
class SetAutorityInput :
	public BaseUIMsgData
{
public:
	SetAutorityInput(WriterManager * writeMgr, SetAutotityOperateType operateType);
	SetAutorityInput(WriterManager * writeMgr, SetAutotityOperateType operateType, QString codeId);
	SetAutorityInput(WriterManager * writeMgr, SetAutotityOperateType operateType, int  authotity);
	//SetAutorityInput(BaseWinData * winData, SetAutotityOperateType operateType, WriterManager * writerManager, int  authotity);
	~SetAutorityInput();
public:
	void doAction();
	// 关闭当前所有1级模板的读写权限
	void closeAllLevelOneTempleteWriteAutority();
	// 关闭除当前id以外的所有1级模板的读写权限
	void closeAllLevelOneTempleteWriteAutorityExcept(QString codeId);
	// 删除指定1级模板
	void deleteLevelOneTempleteByCodeId(QString codeId);
	// 设置文件权限
	void setFileAuthority(int authority);
	
	SetAutotityOperateType mOperateType;
	//NormalPageTxtWinData *mPageTxtWinData;
	//WriterManager * mWriterManager;
	QString mCodeId;
	int mAuthotity;
};
#endif
