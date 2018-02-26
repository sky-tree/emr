#ifndef BASE_TOOLS_H_
#define BASE_TOOLS_H_


#include <QList>
#include <QColor>
#include <QPainter>



class BaseContainer;
class ViewObj;
class BaseTemplate;

class RowViewParam;

class TxtWinData;
class StrContentData;
class ParagraphData;

class WriterManager;

// 基本工具类
class BaseTools
{
public:
	BaseTools();
	~BaseTools();

public:
public:

	static void openAllRowReplace(BaseContainer * list);
	static bool haveExcel(QList<BaseContainer *> * list);
	static QVector<QRect> getCurClip(QPainter &pPainter);
	static void addToListWithoutSame(QList<ParagraphData *> * list, ParagraphData * data);
	static int getMaxPageNumberWidth(TxtWinData * winData, QFontMetrics  graphics, char type);
	static QString judgeStrWithPageNumber(StrContentData * source, ViewObj * effectPage);
	static void setLineType(QPainter &painter, QColor * roundLineColor, int roundLineThick, char type);
	static bool drawLines(QPainter &pPainter, QColor * color, int thick, char type, int x0, int y0, int x1, int y1);

	static void getObjListFromExcel(BaseContainer * source, QList<BaseContainer *> * destList);
	static void exchangeObjToViewList(BaseContainer * source, QList<BaseContainer *> * destList);
	static void exchangeListToViewList(QList<BaseContainer *> *sourceList, int start, int end, QList<BaseContainer *> * destList);
	static void seperateListByEnter(QList<QList<BaseContainer *> *> * seperateAllViewChildren, QList<BaseContainer *> * source);

	static void removeAllChildrenFromList(QList<BaseContainer *> * list, BaseContainer * parent);
	static void setChooseColor(BaseContainer * obj, int color);

	static void drawChildrenUnderline(QPainter &pPainter, RowViewParam * rowParam, int x, int y
		, QList<BaseContainer *> * children, int start, int end, char type, int thick);
	static void drawChildrenBackground(QPainter &pPainter, RowViewParam * rowParam, int x, int y
		, QList<BaseContainer *> * children, int start, int end, int color);

	static void refreshTempContainerAndTempIdx(BaseContainer * focus, BaseTemplate *& tempParent, int & tempIdx);

	static void judgeTitleOrBottomOutArea(ViewObj * layer);

	static int getFirstNodeIdx(QList<BaseContainer *> * list, int start, int end);
	static void copyObjListToObjList(QList<BaseContainer *> * srcList, QList<BaseContainer *> * destList, bool isClearDestList);

public:
	static bool isOverOrEqualsVersion(int cur, QString dest);
	static int convertQStringOfVersionToInt(QString version);

public:
	// 开始登记起始时间
	static void println(QString source);
	static void println(QString source, int value);
	static long refreshStartTime();
	static void printSpendTime(QString source, int value);
	static void printSpendTime(QString source, bool needRefreshStartTime = true);
	static void printSpendTime(long &startTime, QString source, int value);
	static void printSpendTime(long &startTime, QString source, bool needRefreshStartTime = true);

private:
	static QDateTime mQTime;
	static long mStartTime;
};

#endif
