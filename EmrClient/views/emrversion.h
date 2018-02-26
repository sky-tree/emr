/************************************************************************/
/*             版本号管理：1.0.0.Release				                    */
/*			   版本号管理：主版本号.次版本号.修订版本.版本类型                */
/*			   主版本号：(重大结构功能调整)									*/
/*			   次版本号：(新增功能)							    		*/
/*			   修订版本：(主要针对bug修改)									*/
/*			   版本类型：(编译时间和类型)	         						*/
/************************************************************************/
// 3.06.09 针对潮阳三级检诊权限

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
