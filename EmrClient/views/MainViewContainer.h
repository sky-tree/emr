/*********************************************************************************
*Copyright(C),Shine
*FileName:  MainViewContainer
*Author:  caokewen
*Version:  1.0
*Date:  //2016/3/23
*Description:  //此类为主框体容器
**********************************************************************************/
#ifndef MAINVIEWCONTAINER_H
#define MAINVIEWCONTAINER_H
 
#include <QMainWindow>
#include <QGroupBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QSplitter>
#include"qevent.h"
#include"QtXml\qdom.h"
#include"qmenu.h"
#include"qmenubar.h"
#include"qtoolbar.h"
#include"qfontcombobox.h"
#include"qtextformat.h"
#include"qlistview.h"
#include"selectmubandlg.h"
#include"mubaneditdlg.h"
#include"qlabel.h"
#include"addmubandlg.h"
#include"customDelegate/treeWidget.h"
#include"customDelegate/TreeItemMimeData.h"
#include"qmessagebox.h"
#include "dataModel/DBDataModel/DBManager.h"
#include "dataModel/BaseDataType.h"
#include "version.h"

// 代码一定要是: UTF-8(BOM)      
//qt版本高于等于qt5，VS版本高于或等于VS2010    
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class QSqlQueryModel;
class DataBaseControl;
class QDataStream;
class MyAppEntrance;
class MainViewContainer : public QMainWindow
{
	Q_OBJECT
public:
	enum PageSetType
	{
		ParagraphNumber,
		TopPageNumber,
		BottomPageNumber,
		PageOfHeader,
		PageOfFooter,
		LineType,

	};
private:
	//文件主菜单
	QMenu *				m_pFileMenu;//主菜单
	QMenu *				m_pFileExportMenu;//二级菜单（导出）
	QMenu *				m_pFileSubmit;//提交病历
	QAction *			m_pTxtAct;//导出txt
	QAction *			m_pHtmlAct;//导出html
//	QAction *			m_pNewAct;//新建
	QAction *			m_pSubmitCurrent;//提交当前打开病历
	QAction *			m_pSubmitMultipl;//提交多份病历
	QAction *			m_pOpenAct;//打开
	QAction *			m_pSaveAct;//保存
	QAction *			m_pExportAct;//另存为
	QAction *			m_pPrintAct;//打印
	QAction *			m_pPrintPreviewAct;//打印预览
	QAction *			m_pExitAct;//退出
	
	//编辑菜单
	QMenu *				m_pEditMenu;//编辑
	QAction *			m_pUndoAct;//撤销
	QAction *			m_pRedoAct;//恢复
	QAction *			m_pCutAct;//剪切
	QAction *			m_pCopyAct;//复制
	QAction *			m_pPasteAct;//粘贴

// 	QAction *			m_pFindAct;//查找
// 	QAction *			m_pReplaceAct;//替换
// 	QAction *			m_pSelectAllAct;//全选
// 	QAction *			m_pDeleteAct;//删除

	//字体菜单
	QMenu *				m_pFontMenu;//字体主菜单
	QMenu *				m_pFormatMenu;//格式二级菜单
	QAction *			m_pBoldAct;//加粗
	QAction *			m_pItalicAct;//倾斜
	QAction *			m_pUnderlineAct;//下划线
	QAction *			m_pSuperscriptAct;//字体上标
	QAction *			m_pSubscriptAct;//字体下标
	QAction *			m_pColorAct;//字体颜色

	//段落菜单
	QMenu *				m_pParagraphMenu;//段落设置主菜单
	QMenu *				m_pTextSpaceMenu;//文本行距二级菜单
	QAction *			m_pLeftAlignAct;//左对齐
	QAction *			m_pCenterAct;//居中
	QAction *			m_pRightAlignAct;//右对齐
	QAction *			m_pJustifyAct;//两端对齐
	QAction *			m_pTextSpacintAct1;//文本行距1倍
	QAction *			m_pTextSpacintAct2;//文本行距2倍
	QAction *			m_pTextSpacintAct3;//文本行距3倍
// 	QAction *			m_pSubIndentationAct;//
// 	QAction *			m_pAddIndentationAct;//
	QAction *			m_pParagraphAttrubuteAct;//段落属性

	//页面菜单
	QMenu *				m_pPageMenu;//页面菜单
	//表格菜单
	QMenu *				m_pTableMenu;//表格二级菜单
	QMenu *				m_pCelltableMenu;//拆分单元格
	QMenu *				m_pInsertcellTable;//插入
	QMenu *				m_pDeletecellTable;//删除
	QMenu *				m_pTableSettingMenu;//属性设置

	QAction *			m_pMergeCellsAct;//合并单元格
	QAction *			m_pSplitCellsAct;//横向拆分
	QAction *			m_pSplitTableAct;//竖向拆分
	QAction *			m_pInsertLeftAct;//从左边插入列
	QAction *			m_pInsertRightAct;//从右边插入列
	QAction *			m_pInsertAboveAct;//从上边插入列
	QAction *			m_pInsertBlowAct;//从下边插入列
	QAction *			m_pCellSettingAct;//单元格属性
	QAction *			m_pTableSettingAct;//表格属性
	QAction *			m_pMultiplySplitCellAct;//多行/列拆分
	QAction *			m_pDeleteCellAct;//删除单元格
	QAction *			m_pDeleteRowAct;//删除整行
	QAction *			m_pDeleteColumnAct;//删除整列


	//插入菜单
	QMenu *				m_pInsertMenu;//插入主菜单
	QAction *			m_pInsertTableAct;//插入表格
	QAction *			m_pInsertImageAct;//插入图片
//	QAction *			m_pInsertSymbolAct;//符号
	QAction *			m_pInsertSpecialSymbolAct;//特殊符号
	QAction *			m_pPageBreaksAct;//分页符
	QAction *			m_pLineAct;//横线
	QAction *			m_pGridLineAct;//网格线


	//打印菜单
	QMenu  *			m_pPrintMenu;//打印主菜单
	QAction *			m_pGeneralPrint;//普通打印
	QAction *			m_pContinuousPrint;//续打
	QAction *			m_pChoosePrint;//打印预览

	QAction *			m_pFinishAct;//完成病历

	//template
	QMenu *				m_pTemplateMenu;//模板
	QAction *			m_pTemplateAct;//模板设置

	QMenu *				m_pHelpMenu;
	QAction *			m_pAboutAct;
	QAction *			m_pAdmin;

	//ToolBar
	QToolBar *			m_pSaveToolBar;//保存
	QToolBar *			m_pFileToolBar;//文件
	QToolBar *			m_pFontToolBar;//字体


	//右键树节点
	QAction *			m_pAddEmr;//新增病历
	QAction *			m_pModifyEmr;//修改病历
	QAction *			m_pDeleteEmr;//删除病历
	QAction *			m_pSaveEmr;//保存病历
	QAction *			m_pRename;//重命名
	QAction *			m_pShowReport;//显示报告
	//右键模板节点
	QAction *			m_pAddTemplate;//新增模板
	QAction *			m_pModifyTemplate;//修改模板
	QAction *			m_pDeleteTemplate;//删除模板
	QAction *			m_pSaveTemplate;//保存模板
	//树节点样式
	QString				m_strTreeStyle;//节点样式

	QTreeWidgetItem *	m_pPreItem;//上一个元素

	//连接数据库
	DataBaseControl *	m_pDataBaseControl;//数据库控制
	DBManager *			m_pDbManager;//数据库manager
	QSqlQueryModel*		m_pModel;//类型


	QGroupBox *			m_pTopHGroupBox;//
	QGroupBox *			m_pTopHMsgGroupBox;//顶部消息容器
	QGroupBox *			m_pMainHGroupBox;//主界面容器
	QGroupBox *			m_pLeftVGroupBox;//容器
	QGroupBox *			m_pRightVGroupBox;//容器
	QTreeWidget *		m_pMainMenuTreeWidget;//病历树
	QTreeWidget *		m_pMainMenuBottomTreeWidget;//隐藏树，暂时无用
	treeWidget *		m_pMainMenuRightTreeWidget;//工具树
	QTreeWidget *		m_pMainMenutempleteTreeWidget;//模板树
	QVBoxLayout *		m_pAllLayout;//所有布局
	QHBoxLayout *		m_pRightLayout;//右侧布局
	QHBoxLayout *		m_pMainLayout;//主界面布局
	QLabel *			m_pTopMsgTitle;//顶部布局

	MRTemplate			m_Templatedata;//
	QString				m_pAddTemplateId;//标记是否新增病理时候需要加载模板


	QVBoxLayout *		m_pLeftLayout;//

	QSplitter   *		m_pSplitter;//
	QSplitter   *		m_pSplitter1;//

	QString				m_strFlag;//
	QString				m_strIsPhysician;//

	QLabel *			m_pTopTitleLabel;//
	QHBoxLayout *		m_pTopMsgLayout;//
	QLabel *			m_pTopMsgTitlePic;//
	QPixmap *			m_pPic;//

	QMap<QString, QString>						m_mapSql;//
	QMap<QString, QString>						m_mapSqlTemplate;//
	//处理新增模板
	QMap<QString, QString>						m_mapAddTemplate;//
	QMap<QString, QMap<QString, QString>>		m_mapAllTemplate;//
	QMap<QString, MRTemplate>					m_mapAllMedical;//

	bool						m_bIsAllTempAdd;//
	static MainViewContainer *	m_pInstance;//
	WriteAuthority				m_wirteAth;	//当前病人的医生集
	QMap<QString, MdcAuthority>	m_stlMdcAth;//当前加载的病历的提交层级
	bool						m_bIsAdminUser;

public:
//	QAction *			m_pLayoutAct;//整洁显示
	QAction *			m_pHideOrDisplayHeaderAct;//页眉
	QAction *			m_pHideOrDisplayFooterAct;//页脚
	QAction *			m_pHideOrDisplayTopPageNumber;//顶部页码
	QAction *			m_pHideOrDisplayBottomPageNumber;//底部页码
	QAction *			m_pLineNumAct; //行号
	QAction *			m_bPageAct; // 页面属性设置
	bool				m_bIsSupperUser; //是否是超级用户
	QString				m_strSpecialId;//用于记录病程记录的特殊Id;
	QTimer *			m_pDeleteTimer;//
	bool				m_bIsClose;//
	bool				m_bIsClickCanccel;//修改病历/模板时，是否点击取消按钮
	QList<MyAppEntrance *>* m_pListAppEntrance;//

	bool				m_bHideOrDisplayTopPageNumber;
	bool				m_bHideOrDisplayBottomPageNumber;

	bool 				m_bHideOrDisplayHeaderAct; 
	bool 				m_bHideOrDisplayFooterAct; 

	bool				m_bIsAuthor;

public:
	explicit MainViewContainer(QWidget *parent = 0);
	~MainViewContainer();
private:

	/*创建菜单栏*/
	void setMedicalPopMenu(QTreeWidgetItem* pCurItem,QMenu* pMenu);
	void setTemplatePopMenu(QTreeWidgetItem* pCurItem, QMenu* pMenu);
	bool closeWindow();
	void createMenus();
	void createActions();
	void createToolBars();
	void createAllLayout();
	void createMenu();
	void createTopHorGroupBox();
	void createTopHorMsgGroupBox();
	void createMainHorGroupBox();
	void createLeftVerGroupBox();
	void createRightVerticalGroupBox(QGroupBox *groupBox);
	void createConnectSignalSlot();
	void refreshRightContainer(QGroupBox *groupBox);
	void ReadXml(QString pPath);
    void listDom(QDomElement& docElem, QTreeWidgetItem *pItem);
	void LoadViews(QString pViews,bool isTemplate,QTreeWidgetItem *pItem,QString id="");
	void textSize(const QString &p);
	void createAllTreeNode();
	void loadTemplateData(QTreeWidget *widget, QString tableName, QMap<QString, QString>&map);
	void setIsTemplateVisable(QString isVisable);
	QString medicalRecordsId(bool isTemplate,QTreeWidgetItem *item,QString str);
	QTreeWidgetItem* getItemToId(QTreeWidget *widget, QString id);
	
	QGroupBox* getUserCheckView();

	/*病历，模板设置*/
	void addTreeNodeAndTab(QTreeWidgetItem *item,QString strName="",bool isTemplate=false);
	QString getItemName(QTreeWidgetItem *item);
	QTreeWidgetItem * getSpecialItem(QTreeWidget *widget, QString str);
	bool isExist(QTreeWidgetItem *item, QString str);
	void  createTreeNodeTips(QTreeWidget *widget, QTreeWidgetItem *item, QString str);
	int   getItemSerialNumber(QTreeWidgetItem *item);
	
	QString FileDialogPath(QString name);
/************************************************************************/
/* Date: 2017.9.6                                                       */
/* ver:  3.0															*/
/************************************************************************/
	void initHospitalTitle();

protected:
	void closeEvent(QCloseEvent *);

public:
	void setPrompts(QString id);
	void setMenuStatus(bool isEnable);
	void modifyMenuStatus(int type, bool isChecked);
	static MainViewContainer* getInstance();
	void destory();
	QTreeWidget* getMainMenuTreeWidget();

	QTreeWidget* getMainmenuTemplateWidget();

	DataBaseControl* getDataBaseControl();

	void setPopAuthority(QTreeWidgetItem* curItem, QMenu *popMenu);
	void setPopAuthority(QMenu *popMenu, MdcAuthority mdcAth);


	bool isAuthority(QTreeWidgetItem *pItem, bool isTemplate);
	bool getPermissionsFormPid(QString pid);

	void setStyle();
	void setMenuEnable(bool flag = false);
	void setSuperUser(bool flag = false);//暂时无用
	bool isTemplate() const;
	void resizeEvent(QResizeEvent *resizeEvent);//暂时无用
	QTreeWidgetItem* GetMedicalUnSaveNode(QTreeWidgetItem *pItem);
	void deleteEMR(MyAppEntrance *app);
	void setSaveDataFromRightMenu(bool isFromRightMenu); //黄洪保存数据时，调用来源区分
	bool mSaveDataIsFromRightMenu;

	private slots:
	/*点击树节点事件信号槽*/
	void  rightClick(const QPoint& ps);
	void  rightClickTemplate(const QPoint&ps);

	/*新增病历弹出窗口*/
	void selectmubandlg();
	void deleteAppEntrance();
	void textColor();

	void saveTreeNodeAnadTab();
	void paragraphSetting();
	void renameTriggered();
	void exportTxt();
	void exportHtml();
	void addTemplateDlg();

	void deleteNode(QTreeWidget *pWidget);
	void modifyNode(QTreeWidget *pWidget);
	void saveNode(QTreeWidget *pWidget);

	void setTemplateData(QTreeWidgetItem *pWidget);
	void setTextSpace();


	void fileOpen();
	void fileSave();
	void fileSaveAs();
	//表格的插入
	void insertLeftTable();
	void  insertRightTable();
	void  insertAboveTable();
	void  insertBlowTable();
	//表格的删除
	void deleteCellTable();
	void deleteRowTable();
	void deleteColumnTable();
	//表格的设置
	void cellSettingTable();
	void multiplySplitCellTable();
	void tableSettingTable();
	//打赢
	void generalPrintData();
	QString getId();

	void setMenuPageEnable(MyAppEntrance *app);
	void showReport();					//尹露 显示报告
	void aboutVersion();
	void adminLogin();

	void submitCurrent();
	void submitMultipl();


	public slots:
	bool eventFilter(QObject *watched, QEvent *event);
	void modifyTreeNodeAndTab();
	void textBold();
	void leftAlignActTriggered();
	void rightAlignActTriggered();
	void centerAlignActTriggered();
	void textItalic();
	void textUnderline();
	void superscrip();
	void subscript();

	void templateSet();
	void saveTemplateData(QTreeWidgetItem* pItem, QString isModify, bool isFromRightMenu = false);
	void saveMedicalData(QTreeWidgetItem* pItem, QString isModify, bool isFromRightMenu = false);
	void showSelectMainMenuItem(QTreeWidgetItem *item, int column);
	//缺省参数：当修改时，需要删除之前新增的节点时候使用（防止弹出两个框（1：是否保存，2：删除后不可恢复）
	void deleteTreeNodeAndTab(bool isModifyforDelete = false);
	void printView();
	void dapper(bool state);
	void displayLineNumber(bool state);
	void onPageActTriggered();
	void displayPageHeader(bool state);
	void displayPageFooter(bool state);
	void displayTitlePage(bool state);
	void displayBottomPage(bool state);
	void insertImage();
	void insertTable();
	void mergeCells();
	void horSplitCells();
	void verSplitCells();
	void continuePrintClicked();
	void insertSpecialSymbol();
	void doCopyPasteCutUndoRedo();
	void insertPageBreaks();
	void insertLine();
	void insertGridLine(bool stats);
public:
	void modifyTreeNodeAndTab(QString strItem);
	void setTreeCurrentItem(QString strItem);
	void submitMdc(QString id);
	void modifyTree(QString id, int grade);
	void modifyTreeItem(QTreeWidgetItem * qTreeItem, int grade);

};
#endif // MAINVIEWCONTAINER_H
