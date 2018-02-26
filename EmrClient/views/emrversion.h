/************************************************************************/
/*             �汾�Ź���1.0.0.Release				                    */
/*			   �汾�Ź������汾��.�ΰ汾��.�޶��汾.�汾����                */
/*			   ���汾�ţ�(�ش�ṹ���ܵ���)									*/
/*			   �ΰ汾�ţ�(��������)							    		*/
/*			   �޶��汾��(��Ҫ���bug�޸�)									*/
/*			   �汾���ͣ�(����ʱ�������)	         						*/
/************************************************************************/
// 3.06.09 ��Գ�����������Ȩ��

#define EMR_VERSION			"3.06.14."

#ifndef EMRVERSION_H
#define EMRVERSION_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include "ui_emrversion.h"

class EMRVersion : public QDialog
{
	Q_OBJECT

public:
	EMRVersion(QWidget *parent = 0);
	~EMRVersion();
private:
	void initUI();
	void initConnect();

private:
	Ui::EMRVersion ui;
	QPushButton	*		m_BtnOk;
	QLabel *			m_LbTime;
	QLabel *			m_LbDate;
	QLabel *			m_LbVersion;

	QLabel *			m_LbTimeHead;
	QLabel *			m_LbDateHead;
	QLabel *			m_LbVersionHead;

};

#endif // EMRVERSION_H
