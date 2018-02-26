#include "emrclient.h"
#include"EMR/entrance/MyAppEntrance.h"
#include <qfiledialog.h>

EmrClient::EmrClient(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	MyAppEntrance* widget = new MyAppEntrance("appEntrance", 0, 0, this);
	this->setCentralWidget(widget);
	setMouseTracking(true);
//	OpenEidtImageWindow();
}

EmrClient::~EmrClient()
{

}


void EmrClient::OpenEidtImageWindow(ImgConTentData* imageData)
{
	
}
