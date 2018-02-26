#ifndef _HTMLOUTPUTMANAGER_H_
#define _HTMLOUTPUTMANAGER_H_
#include "BaseElement.h"
#include "qlist.h"
#include "ConstantsID.h"
class htmlBaseObj;
class ViewObj;
class QImage;
class htmlOutPutManger :
	public BaseElement
{
public:
	htmlOutPutManger(int classid = ConstantsID::ClassId::HTML_MANAGER);
	~htmlOutPutManger();
	void getHtmlObj(QList<htmlBaseObj*> * resultList, ViewObj * rootView);
	void handleViewObj(ViewObj * view,int x,int y);
	void handleTextViewObj(ViewObj * view, int x, int y, htmlBaseObj  * &outHtmlObj, htmlBaseObj  * &outLineHtmlObj);
	void handleImgViewObj(ViewObj * view, int x, int y);
	void handleExcelPageViewObj(ViewObj * view, int x, int y);
	void removeSamePositionLine();
	QString saveImage(QImage *image);
	void saveImageInByteArray(QImage *image, QByteArray * byteArray);
	void getTxt(ViewObj * rootView,QString & str);
	bool isSameTxtContentDataViewObj(ViewObj * firstView, ViewObj * secondView);
	void handleSameTxtViewObjList(QList<ViewObj *>& txtViewList,int x,int y);
	QList<htmlBaseObj*> * mTemptResultList;
	static const QString IMG_DIR_PATH;
	static const QString IMG_SUFFIX;
};
#endif