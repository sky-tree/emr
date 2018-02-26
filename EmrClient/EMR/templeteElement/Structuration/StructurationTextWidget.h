#ifndef _STRUCTURATIONTEXTWIDGET_H_
#define _STRUCTURATIONTEXTWIDGET_H_
#include "BracketWidget.h"
#include "qstring.h"
#include <qobject.h>
//qt版本高于等于qt5，VS版本高于或等于VS2010
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
//class QCalendarWidget;
class StructurationTextWidget :
//	public QObject,
	public BracketWidget
{
	//Q_OBJECT
public:
	StructurationTextWidget(WriterManager * mgr, BaseTemplate * templeteParent, int objId);
	StructurationTextWidget(WriterManager * mgr, StructurationTextWidget * otherStructureation);
	~StructurationTextWidget();
	void initNumercial();
	virtual	int mousePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseRelease(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseMove(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDrag(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseDoublePress(int type, int x, int y, BaseContainer * obj);
	virtual	int mouseScroll(int angle, BaseContainer * obj);
	virtual	int keyPress(QString str, BaseContainer * obj);
	virtual	int keyRelease(QString str, BaseContainer * obj);
	/*void setDlgType(QString dlgType);
	QString getDlgType();*/
	void setValueType(QString dlgType);
	QString getValueType();
	/*void setDataTime(QString dlgType);
	QString getDataTime();*/
	void setMaxValue(QString dlgType);
	QString getMaxValue();
	void setMinValue(QString dlgType);
	QString getMinValue();
	void setLength(QString dlgType);
	QString getLength();
	void setDicimalLen(QString dlgType);
	QString getDicimalLen();
	void setTimeFomat(QString timeFormat);
	QString getTimeFomat();
	void setHadGetCurrentTime(bool hadSet);
	bool getHadGetCurrentTime();
	void setIsTwentyFourFomat(bool isTwentyFour);
	bool getIsTwentyFourFomat();
	virtual	bool setTempleteProperty(QMap<QString, QString>& setMap, QList<ViewObj *>* strObjList, QList<ViewObj *>* defaultObjList);
	void setPropertyOnActionRedoOrUndo(QMap<QString, QString>& setMap, QList<ViewObj *> * strObjList, QList<ViewObj *> * defaultObjList);
	void addSetTempletePropertyAction(QMap<QString, QString>& beforMap, QMap<QString, QString>& map, QList<ViewObj *> & beforStrObjList, ViewObj* beforFocus, char beforFocusDir);
public:
	void getCurrentTime();
	bool isCanGetCurrentTime();
private:
//QString mDlgType;
QString mValueType;//数值类型true，日期类型值为 "false"
//QString dataTime;//日期
QString mMaxValue;
QString mMinValue;
//QCalendarWidget *mCaledar;
//QString content;//数值时类容
QString mLength;//总长度
QString mDecimalLen;//小数长度
QString mTimeFomat;//日期格式
bool mIsTwentyFourFomat;
bool mHadGetCurrentTime;//是否自动获取过系统时间
//public  slots:
//void setDate();

};
#endif
