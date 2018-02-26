#include "Cursor.h"
#include "qDebug.h"
/**/
Cursor * Cursor::cursor=NULL;
Cursor::Cursor() :entrence(NULL)
{
	static CGarbo Garbo;
}


Cursor::~Cursor()
{
	if (entrence)
	{
		entrence = NULL;
	}
}
void Cursor::initCursor(QWidget* entrence){
	if (!cursor){
		cursor = new Cursor();
	}
	cursor->entrence = entrence;
}
//void Cursor::deleteCursor(QMainWindow* entrence)
//{
//	if (cursor)
//	{
//		delete cursor;
//		cursor = NULL;
//	}
//}
// 正常鼠标箭头
void Cursor::slotArrow()
{
	if (cursor->mCurrentType == CursorType::Arrow)
	{
		return;
	}
	else
	{
		cursor->mCurrentType = CursorType::Arrow;
	}
	cursor->entrence->setCursor(Qt::ArrowCursor);
}
// 鼠标+读档转圈
void Cursor::slotBusy()
{
	cursor->entrence->setCursor(Qt::BusyCursor);
}
// 右手握拳
void Cursor::slotCloseHand()
{
	cursor->entrence->setCursor(Qt::ClosedHandCursor);
}
// 十字架
void Cursor::slotCross()
{
	cursor->entrence->setCursor(Qt::CrossCursor);
}
// 禁止图标
void Cursor::slotForbidden()
{
	cursor->entrence->setCursor(Qt::ForbiddenCursor);
}
// 手指指定
void Cursor::slotHand()
{
	cursor->entrence->setCursor(Qt::PointingHandCursor);
}
// I形光标
void Cursor::slotIbeam()
{
	
	if (cursor->mCurrentType == CursorType::Ibeam)
	{
		return;
	}
	else
	{
		cursor->mCurrentType = CursorType::Ibeam;
	}
	cursor->entrence->setCursor(Qt::IBeamCursor);
}
// 手掌停止手势
void Cursor::slotOpenHand()
{
	cursor->entrence->setCursor(Qt::OpenHandCursor);
}
// 四向箭头
void Cursor::slotSizeAll()
{
	if (cursor->mCurrentType == CursorType::SizeAll)
	{
		return;
	}
	else
	{
		cursor->mCurrentType = CursorType::SizeAll;
	}
	cursor->entrence->setCursor(Qt::SizeAllCursor);
}
// 右上、左下箭头
void Cursor::slotSizeB()
{
	cursor->entrence->setCursor(Qt::SizeBDiagCursor);
}
// 左上、右下箭头
void Cursor::slotSizeF()
{
	cursor->entrence->setCursor(Qt::SizeFDiagCursor);
}
// 左、右双箭头
void Cursor::slotSizeH()
{
	cursor->entrence->setCursor(Qt::SizeHorCursor);
}
// 上、下双箭头
void Cursor::slotSizeV()
{
	cursor->entrence->setCursor(Qt::SizeVerCursor);
}
// 向上箭头
void Cursor::slotUpArrow()
{
	cursor->entrence->setCursor(Qt::UpArrowCursor);
}
// 上下拖动
void Cursor::slotVSplit()
{
	if (cursor->mCurrentType == CursorType::VSplit)
	{
		return;
	}
	else
	{
		cursor->mCurrentType = CursorType::VSplit;
	}
	cursor->entrence->setCursor(Qt::SplitVCursor);
}
// 左右拖动
void Cursor::slotHSplit()
{
	if (cursor->mCurrentType == CursorType::HSplit)
	{
		return;
	}
	else
	{
		cursor->mCurrentType = CursorType::HSplit;
	}
	cursor->entrence->setCursor(Qt::SplitHCursor);
}
// loading转圈
void Cursor::slotWait()
{
	cursor->entrence->setCursor(Qt::WaitCursor);
}
// 鼠标+右下角小问号
void Cursor::slotWhatsThis()
{
	cursor->entrence->setCursor(Qt::WhatsThisCursor);
}
// 无光标
void Cursor::slotBlank()
{
	cursor->entrence->setCursor(Qt::BlankCursor);
}

void Cursor::slotCustom()
{
	QCursor *myCursor = new QCursor(QPixmap(":/images/cursor-self.png"), -1, -1);
	//setCursor(*myCursor);
}
void Cursor::refreshFlag(const QCursor & source)
{
	if (&source)
	{
		if (cursor)
		{
			if (cursor->entrence)
			{
				cursor->entrence->setCursor(source);
			}
		}
	}
}
