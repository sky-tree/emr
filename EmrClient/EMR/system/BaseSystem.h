#ifndef _BASE_SYSTEM_H_
#define _BASE_SYSTEM_H_



#include <QFont>
#include <QImage>
#include <QImage>
class WriterManager;

class BaseContainer;

class ViewObj;

class ObjDrawManager;
class ObjKeyManager;
class ObjMouseManager;

class AuthorityViewLineAndStr;

class BaseDraw;
class BaseMouse;

class BaseViewParam;

class BaseContentData;
class GlobalImgData;
class StrContentData;
class ParagraphData;
class QSharedMemory;
class QMimeData;
// 单例静态类，与整个应用程序同周期，保存一些全局静态变量,如：全局图片数据等
class BaseSystem
{
private:
	BaseSystem();
	virtual ~BaseSystem();


public:
	static BaseSystem * getInstance();
public:
	//void setCopyList(QList<BaseContainer *> * list, bool directSetWhenNull, WriterManager *writeManager);
	//QList<BaseContainer *> * getCopyList();

	ParagraphData * useBufferParagraphData(QList<ParagraphData *> * useParagraphDataList, int& useIndex);
	QList<ParagraphData *> * getBufParagraphDataForCopyList();
	//void clearBufferParagraphDataIndex();
	//int getBufParagraphDataIndex();

	void setCopyList(WriterManager * writerMgr, QList<BaseContainer *> * list, bool isAbs, bool needWriteToOtherThread
		, QList<ParagraphData *> * paragraphList);
	 void CopyListToList( QList<BaseContainer *> * srclist, WriterManager * destWriterMgr, QList<BaseContainer *> * dstlist);
	QList<BaseContainer *> * createCopyFromList(WriterManager * writerMgr, QList<BaseContainer *> * list);

	BaseContainer * getContainerCopy(WriterManager * writerMgr, BaseContainer * source);
	BaseViewParam * getViewParamCopy(ViewObj * owner, BaseViewParam * source);
	ObjDrawManager * getDrawMgrCopy(ViewObj * owner, ObjDrawManager * source);
	BaseDraw * getDrawCopy(ViewObj * owner, BaseDraw * source);
	BaseContentData * getContentDataCopy(BaseContainer * owner, BaseContentData * source);
	BaseMouse * getMouseInputCopy(ViewObj * owner, BaseMouse * source);
	ObjKeyManager * getKeyMgrCopy(ViewObj * owner, ObjKeyManager * source);
	ObjMouseManager * getMouseMgrCopy(ViewObj * owner, ObjMouseManager * source);

	void clearCopyList();
	QList<BaseContainer *> * getCopyList();

	QList<BaseContainer *> * getClearTempContainerList();
	QList<int> * getTempIntList();
	GlobalImgData * getImg(QString imgName);
	
	QFont * getFont();
	//void initDefaultStrData();
	StrContentData * getDefaultStrData();
	ParagraphData * getDefaultParagraphData();

	void refreshPainterFont(QPainter &pPainter, StrContentData * data);
	void loadFromClipboard(QByteArray* byteArray);
	void saveToClipboard(QByteArray* byteArray);
	long getSystemId();
	void setCopyListWriteManger(WriterManager* writeManger, bool isCircle);
	//int * getLayoutReplaceMaxType();
//public slots:
	//void deleteMimeData();
private:
	static BaseSystem * mSystem;
	 QList<GlobalImgData *> * mImgDataList;
	// 用于局部临时运算用的全局静态obj容器列表
	 QList<BaseContainer *> * mTempBaseContainerList;
	 QList<int> * mTempIntList;

	 QFont * mFont;

	 StrContentData * mDefaultStrData;
	 ParagraphData * mDefaultParagraphData;

private:
	// 全局权限绘制列表: mTotalAuthorityViewLineAndStrList[]
	QList<AuthorityViewLineAndStr *> * mTotalAuthorityViewLineAndStrList;
	// 复制对象列表
	QList<BaseContainer *> * mCopyList;

	int mBufferParagraphDataIndex;

	// 复制粘贴时缓存的段落属性参数(复制粘贴，第一段及最后一段取当前段属性，中间段从此处缓存取值)
	QList<ParagraphData *> * mBufferParagraphDataForCopy;
	// 缓存绘制参数列表
	//QList<DrawTxtParam *> * mDrawParamList;
	QString mMIMETypes = "text/emrcopy";
	long mSystemId;
	//QMimeData *mMimeData;
	// 垃圾回收，用于回收mSystem
	class CGarbo
	{
	public:
		~CGarbo()
		{
			if (BaseSystem::mSystem)
			{
				delete BaseSystem::mSystem;
			}
		};
	};
};

#endif // _BASE_SYSTEM_H_
