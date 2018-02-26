#include "MyAppEntrance.h"




#include "ToolBarClass.h"

#include "WriterManager.h"
#include "UIInputManager.h"
#include "WriterInputManager.h"

#include "UIMsgResizeWindow.h"
#include "treeWidget.h"
#include "TreeItemMimeData.h"
#include "BaseMath.h"
#include "BaseConstants.h"
#include "StrConstants.h"
#include "BaseViewParam.h"
#include "FileData.h"
#include "Cursor.h"
#include "TempleteTypeEnumeration.h"
#include "OperateMgr.h"
#include "ViewObj.h"
#include "AuthorityConstants.h"
#include "UIConstants.h"
#include "BaseTools.h"

#include "ShortcutKeyInputParam.h"
#include "EditImageWindow/PictureEditWindow.h"
#include <QMessageBox.h>
#include <qDebug.h>
#include "htmlBaseObj.h"
#include "MessageBox.h"
#include "QClipboard.h"
#include <QFileDialog>
#include "BaseSystem.h"
#include "CompressTool.h"

MyAppEntrance::MyAppEntrance(QString sourceFileName, int fileType, int authority, QWidget *parent) :QMainWindow(parent),
//, mPatientData(NULL)
mTimer(NULL)
{
	//setFocusPolicy(Qt::WheelFocus);
	//setAttribute(Qt::WA_KeyCompression);
	//setAttribute(Qt::WA_InputMethodEnabled);
	//setMouseTracking(true);
	//setAcceptDrops(true);
	mToolBar = NULL;
	mPainter = new QPainter(this);
	mWriterManager = NULL;
	mIsTimePause = 1;

	mFileData = new FileData(sourceFileName, fileType, authority);

	// 设置开启输入法
	setFocusPolicy(Qt::WheelFocus);
	setAttribute(Qt::WA_KeyCompression);
	setAttribute(Qt::WA_InputMethodEnabled);
	setMouseTracking(true);
	setAcceptDrops(true);

	//init(NULL);
}


MyAppEntrance::~MyAppEntrance()
{
	mIsTimePause = 2;
	if (mTimer)
	{
		mTimer->stop();
		delete mTimer;
		mTimer = NULL;
	}
	if (mPainter)
	{
		delete mPainter;
		mPainter = NULL;
	}
	if (mToolBar)
	{
		delete mToolBar;
		mToolBar = NULL;
	}
	if (mFileData)
	{
		delete mFileData;
		mFileData = NULL;
	}
	if (mWriterManager)
	{
		delete mWriterManager;
		mWriterManager = NULL;
	}
}
void MyAppEntrance::initInCompressedData(QByteArray * byteArray, PatientData* data)
{
	QByteArray desData1;
	CompressTool::extractByteArray(*byteArray, desData1);
	init(&desData1, data);
}
void MyAppEntrance::init(QByteArray * byteArray, PatientData* data)
{
	//resize(QSize(1000, 800));

	//ViewFactory::init();


	//if (!mIoTools)
	//{
	//	mIoTools = new BaseIOTools(NULL);
	//}
	if (mToolBar == NULL)
	{
		mToolBar = new ToolBarClass(NULL, mFileData->getFileType());
		mToolBar->InitObjcet(this);
		//connect(mToolBar, SIGNAL(enterEvent(QEvent*)), this, SLOT(setCursorStyle(QMouseEvent*)));
	}
	if (mWriterManager == NULL)
	{
		mWriterManager = new WriterManager(this, "", 0);
	}
	if (data)
	{
		mWriterManager->setMacroData(data);
	}
	//if (mPatientData)
	//{
	//	mWriterManager->setMacroData(mPatientData);
	//	delete mPatientData;
	//	mPatientData = NULL;
	//}
	//if (mWriterManager != NULL)
	//{
	//	delete mWriterManager;
	//	mWriterManager = NULL;
	//}
	//mWriterManager = new WriterManager(this, "", 0, "", 0);
	//mIoTools->initWriteManager(mWriterManager);
	//if (mPatientData)
	//{
	//	mWriterManager->setMacroData(mPatientData);
	//	delete mPatientData;
	//	mPatientData = NULL;
	//}
	//mWriterManager->init(mIoTools);
	
	if (byteArray && byteArray->size() > 0)
	{
		ViewObj * rootView = mWriterManager->getRootView();
		if (rootView)
		{
			BaseViewParam * param = rootView->getViewParam();
			UIMsgResizeWindow * msg = new UIMsgResizeWindow(mWriterManager, QSize(param->getSelfW(), param->getSelfH()));
			UIInputManager * uiInputMgr = mWriterManager->getUIInput();
			uiInputMgr->addMsg(msg, true);
		}
		//saveByteArrayBeforOpen(*byteArray);
		mWriterManager->doLoadByteArray(*byteArray);

	}
	else
	{
		mWriterManager->getOperateMgr()->doLoad1(false);
		//mWriterManager->loadDataOnly(false);
	}

	initTimer();
}
void MyAppEntrance::initTimer()
{
	if (!mTimer)
	{
		mTimer = new QTimer();
		mTimer->start();
		setFrequency(BaseMath::changeFresequencyToMillisecond(BaseConstants::DEFAULT_FREQUENCY));
		connect(mTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
		mTimer->start();
	}
	timerResume();
}
void MyAppEntrance::insertRecordInCompressed(QByteArray * byteArray, QString code, PatientData* data, bool isCreate)
{
	QByteArray desData1;
	CompressTool::extractByteArray(*byteArray, desData1);
	insertRecord(&desData1, code, data, isCreate);
	//insertRecord(byteArray, code, data, isCreate);
}
void MyAppEntrance::insertRecord(QByteArray * byteArray, QString code, PatientData* data, bool isCreate)
{
	//setMacroData(data);
	if (mToolBar == NULL)
	{
		mToolBar = new ToolBarClass(NULL, mFileData->getFileType());
		mToolBar->InitObjcet(this);
	}

	if (mWriterManager == NULL)
	{
		mWriterManager = new WriterManager(this, "", 0);
	}
	if (data)
	{
		mWriterManager->setMacroData(data);
		//delete mPatientData;
		//mPatientData = NULL;
	}
	saveByteArrayBeforOpen(*byteArray,"解压后数据99.dat");
	if (byteArray && byteArray->size() > 0)
	{
		mWriterManager->doInsertRecordByteArray(*byteArray, code, isCreate);
	}
	initTimer();
}

WriterManager * MyAppEntrance::getWriterMgr()
{
	return mWriterManager;
}
UIInputManager * MyAppEntrance::getUIInputMgr()
{
	return mWriterManager->getUIInput();
}
ToolBarClass * MyAppEntrance::getToolbar()
{
	return mToolBar;
}
FileData * MyAppEntrance::getFileData()
{
	return mFileData;
}
void MyAppEntrance::resizeEvent(QResizeEvent *resizeEvent)
{
	QSize size = resizeEvent->size();
	if (mWriterManager)
	{
		UIMsgResizeWindow * msg = new UIMsgResizeWindow(mWriterManager, size);
		UIInputManager * uiInputMgr = mWriterManager->getUIInput();
		uiInputMgr->addMsg(msg, true);
	}
}
void MyAppEntrance::setFrequency(int frequency)
{
	mTimer->setInterval(frequency);
}
void MyAppEntrance::timerPause()
{
	mIsTimePause = 1;

}
void MyAppEntrance::doTimePause()
{
	if (mPainter->isActive())
	{
		mPainter->end();
	}
	mTimer->stop();
	//disconnect(mTimer, SIGNAL(timeout()),
	//	this, SLOT(myUpdate()));
}
void MyAppEntrance::timerResume()
{
	mIsTimePause = 0;
	if (mTimer->isActive())
	{
		return;
	}
	mTimer->start();
	//if (mTimer->isActive())
	//connect(mTimer, SIGNAL(timeout()),
	//	this, SLOT(myUpdate()));
}
void MyAppEntrance::setTimeStart()
{
	mTimer->start();
}
void MyAppEntrance::keyPressEvent(QKeyEvent *keyEvent)
{
	QString str = keyEvent->text();
	int keyValue = (int)((str.toStdString())[0]);
	//if (getFileAuthority() == AuthorityConstants::Authority::ONLY_READ)
	//{
	//	qDebug() << "keyPressEvent 111 " << str.size() << keyValue << (keyEvent->modifiers() == Qt::ControlModifier);
	//	//QMessageBox::information(NULL, "病历状态", "病历处于阅读状态，不可编辑", QMessageBox::Yes , QMessageBox::Yes);
	//	if (str.size() == 1 && keyValue == 3)
	//		return;
	//	if (str.size() == 0)
	//		return;
	//	MessageBox::showAboutMessageBox(QStringLiteral("病历状态"), QStringLiteral("病历处于阅读状态，不可编辑"));
	//	return;
	//}

	if (isReadOnly())
	{
		// 当为ctrl或者复制时
		if (keyEvent == QKeySequence::Copy || keyEvent->modifiers() == Qt::ControlModifier)
		{
			// 如果为剪切或者粘贴，则拦截
			if (keyEvent == QKeySequence::Cut || keyEvent == QKeySequence::Paste)
			{
				MessageBox::showAboutMessageBox(QStringLiteral("病历状态"), QStringLiteral("病历处于阅读状态，不可编辑"));
				return;
			}
		}
		else
		{
			int key = keyEvent->key();
			// 上下左右键不拦截
			if (key == Qt::Key_Up || key == Qt::Key_Down
				|| key == Qt::Key_Left || key == Qt::Key_Right)
			{

			}
			else
				MessageBox::showAboutMessageBox(QStringLiteral("病历状态"), QStringLiteral("病历处于阅读状态，不可编辑"));
			return;
		}
	}
	WriterInputManager * writerInput = mWriterManager->getInput();

	if (keyEvent == QKeySequence::SelectAll)
	{
		writerInput->addKeyPress(StrConstants::MyKeyBoard::SELECT_ALL, true);
		return;
	}
	//else if (keyEvent == QKeySequence::Redo)
	//{
	//	//keyEvent->accept();
	//	return;
	//}
	//#ifndef QT_NO_CLIPBOARD  
	//#endif

	//if (e->key() == Qt::Key_Backspace && !(e->modifiers() & ~Qt::ShiftModifier))
	//{
	//	goto accept;
	//}

#ifndef QT_NO_SHORTCUT
	else if (keyEvent == QKeySequence::Undo)
	{
		writerInput->addKeyPress(StrConstants::MyKeyBoard::UNDO, true);
		//doUndo();
		return;
	}
	else if (keyEvent == QKeySequence::Redo)
	{
		writerInput->addKeyPress(StrConstants::MyKeyBoard::REDO, true);
		//doRedo();
		return;
	}
#endif
#ifndef QT_NO_CLIPBOARD
	else if (keyEvent == QKeySequence::Cut)
	{
		QClipboard *board = QApplication::clipboard();
		board->clear();
		writerInput->addKeyPress(StrConstants::MyKeyBoard::CUT, true);
		return;
	}
	else if (keyEvent == QKeySequence::Copy)
	{
		QClipboard *board = QApplication::clipboard();
		board->clear();
		writerInput->addKeyPress(StrConstants::MyKeyBoard::COPY, true);
		return;
	}
	else if (keyEvent == QKeySequence::Paste)
	{
		QClipboard *board = QApplication::clipboard();
		//QString tsxt;
		//tsxt = board->text(QClipboard::Selection);
		if (!board->text().isEmpty())
		{
			getUIInputMgr()->addTxtFromOutside(board->text());
			return;
		}
		writerInput->addKeyPress(StrConstants::MyKeyBoard::PAST, true);
		return;
	}
#endif

	if (keyEvent->modifiers() == Qt::ControlModifier)
	{
	}
	else if (keyEvent->modifiers() == Qt::ShiftModifier)
	{
		if (str.size() > 0)
		{
			writerInput->addKeyPress(str);
		}
	}
	else if (keyEvent->modifiers() == Qt::ALT)
	{
		//if (str.size() > 0)
		//{
		//	writerInput->addKeyPress(str);
		//}
		return;
	}
	else if (keyEvent->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
	{
	}
	else if (keyEvent->modifiers() == (Qt::ControlModifier | Qt::AltModifier))
	{
	}
	else
	{
		str = keyValue;
		int key = keyEvent->key();
		switch (key)
		{
			// 大小写键不处理
		case Qt::Key_CapsLock:
			//writerInput->addKeyPress(BaseConstants::MyKeyBoard::CAPS_LOCK);
			break;
		case Qt::Key_Escape:
			writerInput->addKeyPress(StrConstants::MyKeyBoard::ESC);
			break;
		case Qt::Key_Backspace:;
			writerInput->addKeyPress(StrConstants::MyKeyBoard::BACK_SPACE);
			break;
		case Qt::Key_Delete:
			writerInput->addKeyPress(StrConstants::MyKeyBoard::DEL);
			break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			writerInput->addKeyPress(StrConstants::MyKeyBoard::ENTER);
			break;
		case Qt::Key_Tab:
			writerInput->addKeyPress(StrConstants::MyKeyBoard::TAB);
			break;
		case Qt::Key_Left:
			writerInput->addKeyPress(StrConstants::MyKeyBoard::LEFT);
			break;
		case Qt::Key_Right:
			writerInput->addKeyPress(StrConstants::MyKeyBoard::RIGHT);
			break;
		case Qt::Key_Up:
			writerInput->addKeyPress(StrConstants::MyKeyBoard::UP);
			break;
		case Qt::Key_Down:
			writerInput->addKeyPress(StrConstants::MyKeyBoard::DOWN);
			break;
		case Qt::Key_Alt:
		case Qt::Key_Shift:
			break;
		default:
			if (str.at(0) == 0)
			{
				break;
			}
			writerInput->addKeyPress(str);
			break;
		}
	}
	//qTxtCursor = this->textCursor();
	//qTextCursorLocation = qTxtCursor.position();
	//需要判断删除的位置是存在控件,判断位置
	//bool testBackSpace = ReadXml(qTextCursorLocation);
	//if (((e->modifiers() == Qt::ControlModifier&&keyValue == Qt::Key_X) || (keyValue == Qt::Key_Backspace)) && testBackSpace)
	{

	}
	//else
	{
		//QTextEdit::keyPressEvent(keyValue);
	}
}

void MyAppEntrance::inputMethodEvent(QInputMethodEvent *keyEvent)
{
	if (!keyEvent->commitString().isEmpty() || keyEvent->replacementLength())
	{
		WriterInputManager * writerInput = mWriterManager->getInput();
		QString str = keyEvent->commitString();
		writerInput->addKeyPress(str);
	}
}
void MyAppEntrance::mousePressEvent(QMouseEvent *event)
{
	
	int btn = event->button();
	int x = event->x();
	int y = event->y();
	QString str = "(" + QString::number(x) + "," + QString::number(y) + ")  " + QString::number(btn);

	WriterInputManager * writerInput = mWriterManager->getInput();
	if (btn == Qt::LeftButton)
	{
		writerInput->addMousePress(Qt::LeftButton, x, y);
	}
	else if (btn == Qt::RightButton)
	{
		writerInput->addMousePress(Qt::RightButton, x, y);
	}
	else if (btn == Qt::MidButton)
	{
		writerInput->addMousePress(Qt::MidButton, x, y);
	}
}
void MyAppEntrance::mouseMoveEvent(QMouseEvent *event)
{
	int btn = event->buttons();
	int x = event->x();
	int y = event->y();
	Cursor::initCursor(this);
	//Cursor::initCursor(this);
	//if (y < UIConstants::UI_TOOLBAR_H)
	//{
	//	setCursorStyle(event);
	//	return;
	//}
	WriterInputManager * writerInput = mWriterManager->getInput();
	QString str = "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")  " + QString::number(btn);
	if (btn & Qt::LeftButton)
	{
		writerInput->addMouseDrag(Qt::LeftButton, x, y);
	}
	else if (btn&Qt::RightButton)
	{
		writerInput->addMouseDrag(Qt::RightButton, x, y);
	}
	else if (btn&Qt::MidButton)
	{
		writerInput->addMouseDrag(Qt::MidButton, x, y);
	}
	else
	{

		writerInput->addMouseMove(Qt::LeftButton, x, y);
	}
}
void MyAppEntrance::dragEnterEvent(QDragEnterEvent *event)
{
	treeWidget  *source = qobject_cast<treeWidget *>(event->source());
	if (source)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
}
void MyAppEntrance::dropEvent(QDropEvent *event)
{
	WriterInputManager * writerInput = mWriterManager->getInput();

	treeWidget  *source = qobject_cast<treeWidget *>(event->source());
	if (source/* &&source !=this*/)
	{
		const TreeItemMimeData *pMimeData = (const TreeItemMimeData*)event->mimeData();
		QMap<QString, QString> testMap = pMimeData->DragItemData();
		//QMap<QString, QString> * temp = new QMap<QString, QString>(testMap);
		/*ViewObj* windowView = (ViewObj*)mRootView->getChildByObjId(ConstantsID::ContainerId::TXT_EDIT_WINDOW);
		windowView->clearInputFocus();*/
		writerInput->addMouseDrop(Qt::LeftButton, event->pos().x(), event->pos().y(), &testMap);
		setFocus();
		//QStringList test1 = pMimeData->formats();

		//if (testMap.contains(TempleteTypeEnumeration::TYPE))
		//{
		//	QString strType = testMap["dlgType"];
		//	QString strName = testMap["name"];
		//	if (strType.compare(TempleteTypeEnumeration::RADIO) == 0)
		//	{
		//		mWinData->addTempleteChoiceWedget(&testMap);
		//	}
		//	else if (strType.compare(TempleteTypeEnumeration::MULTIPLE_CHOICE) == 0)
		//	{
		//		mWinData->addTempleteChoiceWedget(&testMap);
		//	}
		//	else if (strType.compare(TempleteTypeEnumeration::HAVE_CHOICE) == 0)
		//	{
		//		mWinData->addTempleteChoiceWedget(&testMap);
		//	}

		//	int a = 1;
		//}
	}

}
void MyAppEntrance::mouseReleaseEvent(QMouseEvent *event)
{
	//Qt::MouseButton btn = event->button();
	int btn = event->button();
	int x = event->x();
	int y = event->y();
	WriterInputManager * writerInput = mWriterManager->getInput();
	QString str = "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")  " + QString::number(btn);
	if (btn & Qt::LeftButton)
	{
		writerInput->addMouseRelease(Qt::LeftButton, x, y);
	}
	else if (btn & Qt::RightButton)
	{
		writerInput->addMouseRelease(Qt::RightButton, x, y);
	}
	else if (btn & Qt::MidButton)
	{
		writerInput->addMouseRelease(Qt::MidButton, x, y);
	}
}
void MyAppEntrance::mouseDoubleClickEvent(QMouseEvent *event)
{
	QString str = "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")  " + QString::number(event->buttons());
	//int btn = event->button();
	int x = event->x();
	int y = event->y();
	WriterInputManager * writerInput = mWriterManager->getInput();
	if (event->button() == Qt::LeftButton)
	{
		writerInput->addMouseDouble(Qt::LeftButton, x, y);
	}
	else if (event->button() == Qt::RightButton)
	{
		writerInput->addMouseDouble(Qt::RightButton, x, y);
	}
	else if (event->button() == Qt::MidButton)
	{
		writerInput->addMouseDouble(Qt::MidButton, x, y);
	}

}
void MyAppEntrance::wheelEvent(QWheelEvent * event)
{
	int delta = event->delta();
	WriterInputManager * writerInput = mWriterManager->getInput();
	writerInput->addMouseScroll(delta);
}

void MyAppEntrance::myUpdate()
{
	BaseTools::refreshStartTime();
	if (mIsTimePause != 0)
	{
		if (mIsTimePause == 1)
			doTimePause();
		// 应用已关闭
		else if (mIsTimePause == 2)
		{
		}
		return ;
	}
	if (mWriterManager)
	{
		//mThreadId = QThread::currentThread();
		mWriterManager->myUpdate();
		update();
	}
	BaseTools::printSpendTime("MyAppEntrance::myUpdate()");
	//mutex.unlock();
}


void MyAppEntrance::paintEvent(QPaintEvent *paintEvent)
{
	mPainter->begin(this);
	mWriterManager->draw(*mPainter);
	mPainter->end();
}

void MyAppEntrance::OpenEidtImageWindow(ImgConTentData* ImageData)
{
	PictureEditWindow *dlg = new PictureEditWindow(ImageData);
	dlg->exec();
	delete dlg;
}

void MyAppEntrance::getImageSnap(ImgConTentData* ImageData)
{
	PictureEditWindow *dlg = new PictureEditWindow(ImageData);
	dlg->getImageSnap(ImageData);
	delete dlg;
}

bool MyAppEntrance::isReadOnly()
{
	return mWriterManager->isFileReadOnly();
}
//int MyAppEntrance::getFileAuthority()
//{
//	return mWriterManager->getFileAuthority();
//
//}
QPainter * MyAppEntrance::getPainter()
{
	return mPainter;
}
//QFont* MyAppEntrance::getFont()
//{
//	return mFont;
//}
void MyAppEntrance::sendKeyEvent(QString signStr)
{
	if (isReadOnly())
	{
		MessageBox::showAboutMessageBox(QStringLiteral("病历状态"), QStringLiteral("病历处于阅读状态，不可编辑"));
		return;
	}
	//FlagParam * endPress = mWinData->getPressEnd();
	//ViewObj* focus = endPress->getFocus();
	if (signStr.contains(TempleteTypeEnumeration::SISTEM_SIGN))
	{
		QList<BaseContainer *> *  chooseListT = mWriterManager->getOperateMgr()->getChooseList();
		int chooseListSize = chooseListT->size();
		ViewObj * tempObj = NULL;
		bool issend = true;
		if (signStr.contains(TempleteTypeEnumeration::INPUT_PATTICULAR_SIGN))
		{

		}
		else
		{
			for (int i = 0; i < chooseListSize; i++)
			{
				tempObj = (ViewObj *)chooseListT->at(i);
				if (tempObj->getObjID() != ConstantsID::ContainerId::EXCEL_PAGE)
				{
					issend = false;
				}
			}
		}
		if (issend)
		{
			WriterInputManager * writerInput = mWriterManager->getInput();
			writerInput->addKeyPress(signStr);
		}
	}
	else
	{
		WriterInputManager * writerInput = mWriterManager->getInput();
		writerInput->addKeyPress(signStr, true);
	}


}
void MyAppEntrance::addExcel()
{
	ShortcutKeyInputParam * shortcutKeyParam = new ShortcutKeyInputParam(mWriterManager, ShortcutType::AddExcel, mWriterManager);
	mWriterManager->getUIInput()->addMsg(shortcutKeyParam);
}
//void MyAppEntrance::setPageProperty()
//{
//	mWriterManager->setPageProperty();
//}
void MyAppEntrance::showTidy(bool tidy, bool isLoop)
{

	ShortcutKeyInputParam * shortcutKeyParam = new ShortcutKeyInputParam(mWriterManager, ShortcutType::ShowTidy);
	shortcutKeyParam->initShowTidyAction(tidy);
	mWriterManager->getUIInput()->addMsg(shortcutKeyParam);
	//if (isSameThread())
	{
		if (mWriterManager->haveUIIputNotHandle())
		{
			mWriterManager->handleUIInput();
		}
	}
	//else
	//{
	//	while (true)
	//	{
	//		if (mWriterManager->haveUIIputNotHandle())
	//		{
	//			_sleep(500);
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}
	//}
	/*if (isLoop)
	{
	QEventLoop eventLoop;
	connect(this, SIGNAL(ShowTidyOver()), &eventLoop, SLOT(quit()));
	eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
	}*/
}
bool MyAppEntrance::isOnShowTidy()
{
	if (mWriterManager)
	{
		return mWriterManager->isOnShowTidy();

	}
	return false;
	/*bool returnValue;
	TemplateInputParam<bool> * getPageTitleNumView = new TemplateInputParam<bool>(mWinData, this, mWriterManager, GetOperatType::IsOnTidy);
	GET_OPERATE_INIT(getPageTitleNumView, &returnValue);
	return returnValue;*/
}
void MyAppEntrance::getHtml(QList<htmlBaseObj*> * resultList)
{
	//if (isSameThread())
	{
		bool isTidy = this->isOnShowTidy();
		mWriterManager->showTidy(true, false);
		//mWinData->refreshAllPage();
		mWriterManager->doGetHtml(resultList);
		mWriterManager->showTidy(isTidy, false);
	}
	//else
	{
	/*TemplateInputParam<bool> * getPageTitleNumView = new TemplateInputParam<bool>(mWinData, this, GetOperatType::GetHtml);
	getPageTitleNumView->initGetHtml(resultList);
	GET_OPERATE_INIT(getPageTitleNumView, NULL);*/
}
}
void MyAppEntrance::getStr(QString &str)
{
	mWriterManager->getStr(str);
}
//void MyAppEntrance::setMacroData(PatientData* data)
//{
//	if (!mPatientData)
//	{
//		mPatientData = new PatientData();
//	}
//	*mPatientData = *data;
//}
void MyAppEntrance::saveByteArrayBeforOpen(QByteArray data, QString fileName)
{
	//压缩使用例子
	/*QByteArray desData;s
	CompressTool::compressByteArray(data,desData);
	QByteArray desData1;
	CompressTool::extractByteArray(desData, desData1);
	int i = 0;*/

	
	//BaseSystem::getInstance()->saveToClipboard(&data);
	//QFileDialog* fDialog = new QFileDialog();
	//QString path = fDialog->getSaveFileName(this, QString("另存为"), "", QString("*.dat"));
	if (data.size()<=0)
	{
		QMessageBox::information(NULL, "文件大小有误", fileName + " 文件大小=" + QString::number(data.size()), QMessageBox::Yes);
		return;
	}
	QString path ;
	QString exePath = QCoreApplication::applicationDirPath();
	if (!exePath.endsWith(QDir::separator()))
	{
		exePath += QDir::separator();
	}
	path = exePath + fileName;
	if (path.isNull())
	{
		
		return;
	}
	//QByteArray array = appEntrance->getUIInputMgr()->getByteArray(false, false);
	if (!path.contains(".dat"))
	{
		path = path + ".dat";
	}

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly)) {
		//std::cerr << "Cannot open file for writing: "
		//	<< qPrintable(file.errorString()) << std::endl;
		QMessageBox::information(NULL, "保存状态1", "文件不可写", QMessageBox::Yes);
		return;
	}
	//delete fDialog;
	QDataStream out(&file);
	out << data;
	//QMessageBox::information(NULL, "保存状态2", "保存成功", QMessageBox::Yes);
	file.close();
}