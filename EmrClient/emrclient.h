#ifndef EMRCLIENT_H
#define EMRCLIENT_H

#include <QtWidgets/QMainWindow>
#include "ui_emrclient.h"
class ImgConTentData;

class EmrClient : public QMainWindow
{
	Q_OBJECT

public:
	EmrClient(QWidget *parent = 0);
	~EmrClient();
	void OpenEidtImageWindow(ImgConTentData* imageData);

private:
	Ui::EmrClientClass ui;
};

#endif // EMRCLIENT_H
