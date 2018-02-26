#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>
#include <QMessageBox>

class MessageBox : public QObject
{
	Q_OBJECT

public:
	static void showErrorMessageBox(QString title,QString message)
	{
		QMessageBox msg(NULL);

		msg.setWindowTitle(title);
		msg.setText(QString(message));
		msg.setIcon(QMessageBox::Critical);
		msg.setButtonText(QMessageBox::Ok, QString("ȷ��")); //���ð�ť����ť�ı�
		msg.exec();
	}
	static int showQueryMessageBox(QString title,QString message)
	{
		QMessageBox msg(NULL);

		msg.setWindowTitle(title);
		msg.setText(QString(message));
		msg.setIcon(QMessageBox::Question);
		msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		msg.setButtonText(QMessageBox::Yes, QString("��"));
		msg.setButtonText(QMessageBox::No, QString("��"));
		msg.setButtonText(QMessageBox::Cancel, QString("ȡ��"));

		return msg.exec();
	}
	static void showAboutMessageBox(QString title, QString message)
	{
		QMessageBox msg(NULL);
		msg.setWindowTitle(title);
		msg.setText(QString(message));
		msg.setIcon(QMessageBox::NoIcon);
		msg.setStandardButtons(QMessageBox::Yes );
		msg.setButtonText(QMessageBox::Yes, QString("ȷ��"));
		msg.exec();
	}
private:

};

#endif // MESSAGEBOX_H
