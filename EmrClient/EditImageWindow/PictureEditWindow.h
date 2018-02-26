#ifndef _PICTURE_EDIT_WINDOW_
#define _PICTURE_EDIT_WINDOW_

#include <QtWidgets/qdialog.h>
#include <QBoxLayout>
#include <QMap>
#include "ImgContentData.h"
#include "BaseEditParam.h"

class QGraphicsView;
class CCQGraphicsScene;
class QGraphicsItem;

#pragma once


class MyGraphicsScene;
class QButtonGroup;

class PictureEditWindow :public QDialog
{
	Q_OBJECT
public:
	explicit PictureEditWindow(ImgConTentData* imageData, QWidget *parent = 0);
	~PictureEditWindow();
	void init();
	void closeEvent(QCloseEvent *event);
	void resizeEvent(QResizeEvent * event);
	void getImageSnap(ImgConTentData* ImageData);
	static const int DIALOG_MIN_WIDTH = 700;
	static const int DIALOG_MIN_HEIGHT = 400;
	static const int DIALOG_MIN_TOOLS_HEIGHT = 35;
	void canleFocus();

protected:
	void keyPressEvent(QKeyEvent *event);
private:
	void initToolBar();
	void initItem();
	void saveSnap();
signals:

	public slots :
		void actionClicked();
private:
	QGraphicsView		* mGraphicsview;
	MyGraphicsScene		* mScene;
	QPushButton			* mTxtButton;
	QPushButton			* mFreeLineButton;
	QPushButton			* mLineButton;
	QPushButton			* mArrowButton;
	QPushButton			* mCircularButton;
	QPushButton			* mEllipseButton;
	QPushButton			* mRoundRectButton;
	QPushButton			* mAttributeButton;
	QPushButton			* mSaveButton;
	QBoxLayout			* mBoxLayout;
	ImgConTentData		* mImageData;
	QList<StrItemData>	  mItems;
	QButtonGroup		* mButtonGroup;
};

#endif
