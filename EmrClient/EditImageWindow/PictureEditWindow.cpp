#include "pictureeditwindow.h"

#include <QGraphicsView>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <qactiongroup.h>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QButtonGroup>

#include "BaseEditParam.h"
#include "PictureShape.h"
#include "EditImageWindow/Scene/MyGraphicsScene.h"
#include "EditImageWindow/Method/Txt/TextItem.h"
#include "EditImageWindow/Dialog/AttributeDialog.h"


PictureEditWindow::PictureEditWindow(ImgConTentData* imageData, QWidget *parent) : QDialog(parent)
{
	setAttribute(Qt::WA_InputMethodEnabled,true);
	setAttribute(Qt::WA_KeyCompression);
	this->setWindowTitle(QString::fromLocal8Bit("图片编辑器"));
	mImageData = imageData;
	init();
}
PictureEditWindow::~PictureEditWindow()
{
}
void PictureEditWindow::saveSnap()
{
	mScene->cancelFoucs();
	QImage *snap = new QImage(mImageData->getImg()->width(), mImageData->getImg()->height(), QImage::Format_ARGB32);
	QPainter painter(snap);
	painter.setRenderHint(QPainter::Antialiasing);
	mScene->render(&painter);
	mImageData->setSnap(snap);
	bool saveSuccess = snap->save("E:/test_scene.jpg");
}


void PictureEditWindow::init()
{
	mScene = new MyGraphicsScene(mImageData->getImgName(),this);
	mGraphicsview = new QGraphicsView(mScene);
	mGraphicsview->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	mGraphicsview->setParent(this);
	mGraphicsview->sizeAdjustPolicy();
	mGraphicsview->resize(QSize(mScene->width(), mScene->height()));
	mGraphicsview->setContextMenuPolicy(Qt::ActionsContextMenu);
	mGraphicsview->move(0, DIALOG_MIN_TOOLS_HEIGHT);
 	mGraphicsview->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
 	mGraphicsview->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	initToolBar();
	if (mScene->width() < DIALOG_MIN_WIDTH && mScene->height() < DIALOG_MIN_HEIGHT + DIALOG_MIN_TOOLS_HEIGHT)
	{
		this->setMinimumSize(QSize(DIALOG_MIN_WIDTH, DIALOG_MIN_HEIGHT + DIALOG_MIN_TOOLS_HEIGHT));
		//移动到中间位置
		mGraphicsview->move((DIALOG_MIN_WIDTH - mScene->width())/2,
			(DIALOG_MIN_HEIGHT - mScene->height())/2);
		this->resize(QSize(DIALOG_MIN_WIDTH, DIALOG_MIN_HEIGHT));
	}
	else if(mScene->width() < DIALOG_MIN_WIDTH && mScene->height() > DIALOG_MIN_HEIGHT + DIALOG_MIN_TOOLS_HEIGHT)
	{
		this->setMinimumSize(QSize(DIALOG_MIN_WIDTH, mScene->height() + DIALOG_MIN_TOOLS_HEIGHT));
		mGraphicsview->move((DIALOG_MIN_WIDTH - mScene->width()) / 2,
			(DIALOG_MIN_HEIGHT - mScene->height()) / 2);
		this->resize(QSize(DIALOG_MIN_WIDTH, DIALOG_MIN_HEIGHT));
	}
	else if(mScene->width() > DIALOG_MIN_WIDTH && mScene->height() < DIALOG_MIN_HEIGHT + DIALOG_MIN_TOOLS_HEIGHT)
	{
		this->setMinimumSize(QSize(mScene->width(), DIALOG_MIN_HEIGHT + DIALOG_MIN_TOOLS_HEIGHT));
		mGraphicsview->move((DIALOG_MIN_WIDTH - mScene->width()) / 2,
			(DIALOG_MIN_HEIGHT - mScene->height()) / 2);
		this->resize(QSize(DIALOG_MIN_WIDTH, DIALOG_MIN_HEIGHT));
	}
	else
	{ 
		this->setMinimumSize(QSize(mScene->width(), mScene->height() + DIALOG_MIN_TOOLS_HEIGHT));
	}
	initItem();
}

void PictureEditWindow::initToolBar()
{
	mButtonGroup = new QButtonGroup();
	mTxtButton = new QPushButton("");
	mFreeLineButton = new QPushButton("");
	mLineButton = new QPushButton("");
	mArrowButton = new QPushButton("");
	mEllipseButton = new QPushButton("");
	mCircularButton = new QPushButton("");
	mRoundRectButton = new QPushButton("");
	mAttributeButton = new QPushButton("");
	mSaveButton = new QPushButton("");


	mTxtButton->setFixedSize(QSize(DIALOG_MIN_TOOLS_HEIGHT, DIALOG_MIN_TOOLS_HEIGHT));
	mFreeLineButton->setFixedSize(QSize(DIALOG_MIN_TOOLS_HEIGHT, DIALOG_MIN_TOOLS_HEIGHT));
	mLineButton->setFixedSize(QSize(DIALOG_MIN_TOOLS_HEIGHT, DIALOG_MIN_TOOLS_HEIGHT));
	mArrowButton->setFixedSize(QSize(DIALOG_MIN_TOOLS_HEIGHT, DIALOG_MIN_TOOLS_HEIGHT));
	mEllipseButton->setFixedSize(QSize(DIALOG_MIN_TOOLS_HEIGHT, DIALOG_MIN_TOOLS_HEIGHT));
	mCircularButton->setFixedSize(QSize(DIALOG_MIN_TOOLS_HEIGHT, DIALOG_MIN_TOOLS_HEIGHT));
	mRoundRectButton->setFixedSize(QSize(DIALOG_MIN_TOOLS_HEIGHT, DIALOG_MIN_TOOLS_HEIGHT));
	mAttributeButton->setFixedSize(QSize(DIALOG_MIN_TOOLS_HEIGHT, DIALOG_MIN_TOOLS_HEIGHT));
	mSaveButton->setFixedSize(QSize(DIALOG_MIN_TOOLS_HEIGHT, DIALOG_MIN_TOOLS_HEIGHT));

	mButtonGroup->addButton(mTxtButton);
	mButtonGroup->addButton(mFreeLineButton);
	mButtonGroup->addButton(mLineButton);
	mButtonGroup->addButton(mArrowButton);
	mButtonGroup->addButton(mCircularButton);
	mButtonGroup->addButton(mEllipseButton);
	mButtonGroup->addButton(mRoundRectButton);
//	mButtonGroup->addButton(mAttributeButton);
//	mButtonGroup->addButton(mSaveButton);
	mButtonGroup->setExclusive(true);

	mFreeLineButton->setCheckable(true);
	mTxtButton->setCheckable(true);
	mLineButton->setCheckable(true);
	mArrowButton->setCheckable(true);
	mCircularButton->setCheckable(true);
	mEllipseButton->setCheckable(true);
	mRoundRectButton->setCheckable(true);
//	mAttributeButton->setCheckable(true);
//	mSaveButton->setCheckable(true);

	mTxtButton->setStyleSheet("QPushButton{background-image:url(:/EmrClient/TxtUnchoose);\
											background-repeat:no-repeat;\
											background-position:center;\
											background-attachment:fixed;}\
							 QPushButton:checked{background-color:rgb(255,233,157);}");

	mLineButton->setStyleSheet("QPushButton{background-image:url(:/EmrClient/LineUnchoose);\
												background-repeat:no-repeat;\
												background-position:center;\
												background-attachment:fixed;}\
									QPushButton:checked{background-color:rgb(255,233,157);}");

	mFreeLineButton->setStyleSheet("QPushButton{background-image:url(:/EmrClient/FreelineUnchoose);\
											background-repeat:no-repeat;\
											background-position:center;\
											background-attachment:fixed;}\
									QPushButton:checked{background-color:rgb(255,233,157);}");

	mArrowButton->setStyleSheet("QPushButton{background-image:url(:/EmrClient/ArrowUnchoose);\
											background-repeat:no-repeat;\
											background-position:center;\
											background-attachment:fixed;}\
									QPushButton:checked{background-color:rgb(255,233,157);}");
	
	mCircularButton->setStyleSheet("QPushButton{background-image:url(:/EmrClient/CircularUnchoose);\
											background-repeat:no-repeat;\
											background-position:center;\
											background-attachment:fixed;}\
									QPushButton:checked{background-color:rgb(255,233,157);}");

	mEllipseButton->setStyleSheet("QPushButton{background-image:url(:/EmrClient/EllipseUnchoose);\
											background-repeat:no-repeat;\
											background-position:center;\
											background-attachment:fixed;}\
									QPushButton:checked{background-color:rgb(255,233,157);}");

	mRoundRectButton->setStyleSheet("QPushButton{background-image:url(:/EmrClient/RoundRect);\
											background-repeat:no-repeat;\
											background-position:center;\
											background-attachment:fixed;}\
									QPushButton:checked{background-color:rgb(255,233,157);}");

	mAttributeButton->setStyleSheet("QPushButton{background-image:url(:/EmrClient/AttributeSet);\
											background-repeat:no-repeat;\
											background-position:center;\
											background-attachment:fixed;}\
									QPushButton:checked{background-color:rgb(255,233,157);}");

	mSaveButton->setStyleSheet("QPushButton{background-image:url(:/EmrClient/SaveUnchoose);\
											background-repeat:no-repeat;\
											background-position:center;\
											background-attachment:fixed;}\
									QPushButton:pressed{background-color:rgb(255,233,157);}");

	QHBoxLayout *pHLayout = new QHBoxLayout();
	pHLayout->addWidget(mTxtButton, 0, Qt::AlignLeft | Qt::AlignTop);
	pHLayout->addWidget(mFreeLineButton, 0, Qt::AlignLeft | Qt::AlignTop);
	pHLayout->addWidget(mLineButton, 0, Qt::AlignLeft | Qt::AlignTop);
	pHLayout->addWidget(mArrowButton, 0, Qt::AlignLeft | Qt::AlignTop);
	pHLayout->addWidget(mCircularButton, 0, Qt::AlignLeft | Qt::AlignTop);
	pHLayout->addWidget(mEllipseButton, 0, Qt::AlignLeft | Qt::AlignTop);
	pHLayout->addWidget(mRoundRectButton, 0, Qt::AlignLeft | Qt::AlignTop);
	pHLayout->addWidget(mAttributeButton, 0, Qt::AlignLeft | Qt::AlignTop);
	pHLayout->addWidget(mSaveButton, 0, Qt::AlignLeft | Qt::AlignTop);

	pHLayout->addStretch();
	pHLayout->setMargin(0);
	pHLayout->setSpacing(0);

	setLayout(pHLayout);
// 
// 	QActionGroup * group = new QActionGroup(this);
// 	group->addAction(arrowAct);
// 	group->addAction(lineAct);
// 	group->addAction(editAct);
// 	group->addAction(pathAct);

	connect(mTxtButton, SIGNAL(clicked()), this, SLOT(actionClicked()));
	connect(mFreeLineButton, SIGNAL(clicked()), this, SLOT(actionClicked()));
	connect(mLineButton, SIGNAL(clicked()), this, SLOT(actionClicked()));
	connect(mArrowButton, SIGNAL(clicked()), this, SLOT(actionClicked()));
	connect(mCircularButton, SIGNAL(clicked()), this, SLOT(actionClicked()));
	connect(mEllipseButton, SIGNAL(clicked()), this, SLOT(actionClicked()));
	connect(mRoundRectButton, SIGNAL(clicked()), this, SLOT(actionClicked()));
	connect(mAttributeButton, SIGNAL(clicked()), this, SLOT(actionClicked()));
	connect(mSaveButton, SIGNAL(clicked()), this, SLOT(actionClicked()));
}

void PictureEditWindow::initItem()
{
	BaseEditParam * baseParam = mImageData->getBaseEditParam();
	QList<StrItemData>& items = baseParam->getItem();
	if (!items.isEmpty())
	{
		mScene->addLastItems(items);
	}
}


void PictureEditWindow::closeEvent(QCloseEvent * event)
{
	if (mScene->isChanged())
	{
		QMessageBox message(QMessageBox::Question,
			QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("是否保存"),
			QMessageBox::Yes | QMessageBox::No, NULL);
		message.setButtonText(QMessageBox::Yes, QString::fromLocal8Bit("是"));
		message.setButtonText(QMessageBox::No, QString::fromLocal8Bit("否"));
		if (message.exec() == QMessageBox::Yes)
		{
			BaseEditParam * baseParam = mImageData->getBaseEditParam();
			baseParam->setItem(mScene->saveData());
			saveSnap();
		}
	}
	event->accept();  //接受退出信号，程序退出  
}

void PictureEditWindow::actionClicked()
{
	QObject * obj = sender();
	if (obj == mTxtButton)
	{
		mScene->setType(PictureShape::Txt);
	}
	else  if (obj == mFreeLineButton)
	{
		mScene->setType(PictureShape::FreeLine);
	}
	else  if (obj == mLineButton)
	{
		mScene->setType(PictureShape::Line);
	}
	else  if (obj == mArrowButton)
	{
		mScene->setType(PictureShape::Arrow);
	}
	else  if (obj == mCircularButton)
	{
		mScene->setType(PictureShape::Circular);
	}
	else  if (obj == mEllipseButton)
	{
		mScene->setType(PictureShape::EllipseItem);
	}
	else if (obj == mRoundRectButton)
	{
		mScene->setType(PictureShape::RoundRect);
	}
	else if (obj == mAttributeButton)
	{
		AttributeDialog * dlg = new AttributeDialog();
		if (Accepted == dlg->exec())
		{
			mScene->setAttribute();
		}
	}
	else  if (obj == mSaveButton)
	{
		BaseEditParam * baseParam = mImageData->getBaseEditParam();
		baseParam->setItem(mScene->saveData());
		saveSnap();
	}
	mScene->changedFoucs();
}

void PictureEditWindow::resizeEvent(QResizeEvent * event)
{
	QSize size = event->size();
	mGraphicsview->move((size.width() - mScene->width()) / 2,
		(size.height() - mScene->height() + DIALOG_MIN_TOOLS_HEIGHT) / 2);
	QDialog::resizeEvent(event);
}

void PictureEditWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
	{
		if (mScene->isChanged())
		{
			QMessageBox message(QMessageBox::Question,
				QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("是否保存"),
				QMessageBox::Yes | QMessageBox::No, NULL);
			message.setButtonText(QMessageBox::Yes, QString::fromLocal8Bit("是"));
			message.setButtonText(QMessageBox::No, QString::fromLocal8Bit("否"));
			if (message.exec() == QMessageBox::Yes)
			{
				BaseEditParam * baseParam = mImageData->getBaseEditParam();
				baseParam->setItem(mScene->saveData());
				saveSnap();
			}
		}
		close();
	}
	break;
	default:
		QDialog::keyPressEvent(event);        // 其他按键处理交给父类  
		break;
	}
}
void PictureEditWindow::canleFocus()
{
	setFocus();
}


void PictureEditWindow::getImageSnap(ImgConTentData* ImageData)
{
	BaseEditParam * baseParam = ImageData->getBaseEditParam();
	QList<StrItemData>& items = baseParam->getItem();
	if (!items.isEmpty())
	{
		saveSnap();
	}
}
