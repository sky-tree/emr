#ifndef UI_INPUT_MANAGER_H_
#define UI_INPUT_MANAGER_H_
#include "QObject.h"


#include <QString>

class WriterManager;
class OperateMgr;
class BaseUIMsgData;
class MyAppEntrance;
class StrContentData;
class ParagraphData;

// 书写器整体输入控制
class UIInputManager :public QObject
{
	Q_OBJECT
public:
	UIInputManager(WriterManager * writer);
	~UIInputManager();

public:
	void addMsg(BaseUIMsgData * msg, bool removeSameType = false);
	bool haveUIIputNotHandle();
	void myUpdate();

public:
	/************************************************ 系统属性设置 开始 ***************************************************/

	ParagraphData * getFocusParagraphData();

	bool setTitleView(bool isView);
	bool setBottomView(bool isView);
	bool setTitleNumViewType(char type);
	bool setBottomNumViewType(char type);
	bool setParagraphNumView(bool isView);
	void putoutPDF();
	bool getTitleView();
	bool getBottomView();
	char getTitleNumViewType();
	char getBottomNumViewType();
	bool getParagraphNumView();


	bool setPageParams(bool immediately, bool isHorizontal, short paperType, int insideT, int insideB, int insideL, int insideR, int titleMaxH, int bottomMaxH);
	void getPageParams(bool &isHorizontal, short &paperType, int &insideT, int &insideB, int &insideL, int &insideR, int &titleMaxH, int &bottomMaxH);


	//bool setPaperType(short type);

	// 全选
	void selectAll();
	// 设置是否整洁显示
	//bool setShowTidy(bool tidy);
	bool isShowTidy();
	// 设置文件读写权限
	bool setFileAuthority(bool isImmediately, char authority);

	bool doCopy(MyAppEntrance* wedgit);
	bool doCut(MyAppEntrance* wedgit);
	bool doPast(MyAppEntrance* wedgit);
	bool doUndo();
	bool doRedo();

	
	// 由于外面UI点击save按钮时，并没有开启新线程，同时需要一个byteAray的返回值，所以暂时直接调用operateMgr的doSave
	void doSave(QByteArray &ba);	//以前的接口， 保存未压缩的数据
	QByteArray getByteArray(bool needCloseWriteAuthority, bool isFromRightMenu);	//以前的接口， 获取未压缩的数据
	void doSaveAndCompress(QByteArray &ba); //获取压缩过得ByteArray
	QByteArray getByteArrayAndCompress(bool needCloseWriteAuthority, bool isFromRightMenu); //获取压缩过得ByteArray

	void clearMacroData();//发信号清空书写器所有宏的数据
	// 打印预览
	bool printPreView();
	// 打印
	bool doPrint();
	//续打
	void sendContinuingPrintSign(int startParagraph, int endParagraph);
	// 添加日期
	bool addCurrentTime();
	// 插入特殊串
	bool addSpecialTxt(QString txt);
	// 从外部复制字符串
	bool addTxtFromOutside(QString txt);
	// 插入图片
	bool addImg(QString imgPath);
	// 设置网格线
	bool setRowBottomLineType(char type);
	char getRowBottomLineType();
	// 插入横线
	bool addHorizontalLine();
	// 插入分页符
	bool addEndPageLine();
	/************************************************ 系统属性设置 结束 ***************************************************/

	void setTitleCanWrite(bool canWrite);
	void setBottomCanWrite(bool canWrite);

	bool setExcelParagraphAlignVerticalType(char dir);

	
	
	/************************************************ 段落属性设置 开始 ***************************************************/

	// 设置左中右对齐
	bool setHorizonAlignType(char alignType);
	char getHorizonAlignType();
	// 设置行间距倍数
	bool setRowDisTimes(float times);
	// 设置段落属性
	//bool setParagraphParams(int disTop, int disBottom, int rowDisH, int insideLeft, int insideRight, char indentType, int indentValue);
	bool setParagraphIndentParams(int insideLeft, int insideRight, char indentType, int indentValue);

	int getParagraphDisTop();
	int getParagraphDisBottom();
	float getParagraphRowDisHTimes();
	int getParagraphInsideLeft();
	int getParagraphInsideRight();
	char getParagraphIndentType();
	int getParagraphIndentValue();
	/************************************************ 段落属性设置 结束 ***************************************************/

	/************************************************ 文字属性设置 开始 ***************************************************/
	bool setFontBold(bool bold);
	bool setFontItalic(bool italic);
	bool setFontSize(int size);
	bool setFontColor(int color);
	bool setBackgroundColor(int color);
	bool setFlowType(char type);
	bool setFontFamily(QString family);

	bool setUnderlineParams(char type, int value);
	bool setDelLineParams(char type, int value);

	bool getFontBold();
	bool getFontItalic();
	int getFontSize();
	int getFontColor();
	int getBackgroundColor();
	char getFontFlowType();
	QString getFontFamily();
	void setHandleOverPreparRestart();
	/************************************************ 文字属性设置 结束 ***************************************************/

	/************************************************ 查找、替换 开始 ***************************************************/
	bool findStr(char type, QString findStr);
	bool replaceStr(char type, QString findStr, QString replaceStr);
	/************************************************ 查找、替换 结束 ***************************************************/


	// 关闭当前所有1级模板的读写权限
	void closeAllLevelOneTempleteWriteAutority(bool isImmediately);
	// 关闭除当前id以外的所有1级模板的读写权限
	void closeAllLevelOneTempleteWriteAutorityExcept(bool isImmediately, QString codeId);
	// 删除指定1级模板
	void deleteLevelOneTempleteByCodeId(bool isImmediately, QString codeId);
	void gotoMedicalRecord(bool isImmediately, QString recodeId); //光标跳转到指定的病程记录首部
	//void openMedicalRecordEditAuthority(bool isImmediately, QString recodeId); //开启病程记录某条记录的编辑权限
	void openMedicalRecordEditAuthorityByFocus(QString & recodeId); //按当前光标开启病程记录某条记录的编辑权限
	// 插入病程记录
	void loadOtherMedicalRecord(bool isImmediately, QString id, QByteArray * source);
	void loadOtherMedicalRecordInCompressedData(bool isImmediately, QString id, QByteArray * source);
	void clearAllData(); //f发信号清空书写器所有数据
private:
	void clearMsg(int haveDoneIndex);
	void removeSameTypeMsg(BaseUIMsgData * msg);

private:

private:
	WriterManager * mWriter;
	OperateMgr * mOperateMgr;
	QList<BaseUIMsgData *> * mUIMsgList;
	bool mIsHandlingInputList;
	int mCurrentIndex;
};


#endif
