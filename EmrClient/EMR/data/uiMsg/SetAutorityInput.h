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
	// �رյ�ǰ����1��ģ��Ķ�дȨ��
	void closeAllLevelOneTempleteWriteAutority();
	// �رճ���ǰid���������1��ģ��Ķ�дȨ��
	void closeAllLevelOneTempleteWriteAutorityExcept(QString codeId);
	// ɾ��ָ��1��ģ��
	void deleteLevelOneTempleteByCodeId(QString codeId);
	// �����ļ�Ȩ��
	void setFileAuthority(int authority);
	
	SetAutotityOperateType mOperateType;
	//NormalPageTxtWinData *mPageTxtWinData;
	//WriterManager * mWriterManager;
	QString mCodeId;
	int mAuthotity;
};
#endif
