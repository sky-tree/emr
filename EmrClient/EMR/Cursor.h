#ifndef _CURSOR_H_
#define _CURSOR_H_
#include <QtWidgets/QMainWindow>
enum CursorType
{
	Arrow = 1,
	Ibeam,
	VSplit,
	HSplit,
	SizeAll
};
class Cursor
{
public:
	Cursor();
	~Cursor();
	CursorType mCurrentType;
	static	void initCursor(QWidget* entrence);
	//static void deleteCursor(QMainWindow* entrence);
	static	void slotArrow();
	static void slotBusy();
	static void  slotCloseHand();
	static void  slotCross();
	static void  slotForbidden();
	static void  slotHand();
	static void  slotIbeam();
	static void  slotOpenHand();
	static void  slotSizeAll();
	static void  slotSizeB();
	static void  slotSizeF();
	static void  slotSizeH();
	static void  slotSizeV();
	static void  slotUpArrow();
	static void  slotVSplit();
	static void	slotHSplit();
	static void  slotWait();
	static void  slotWhatsThis();
	static void  slotBlank();
	static void  slotCustom();
   static void refreshFlag(const QCursor & source);
   QWidget* entrence;
	static Cursor * cursor;
	class CGarbo
	{
	public:
		~CGarbo()
		{
			if (Cursor::cursor)
			{
				delete Cursor::cursor;
				Cursor::cursor = NULL;
			}
		};
	};
};
#endif

