#include <QtWidgets/QApplication>
#include "views\mubaneditdlg.h"
#include "views\propertySettingDlgs\selectionpropertydlg.h"
#include "views\propertySettingDlgs\cellpropertydlg.h"
#include "views\propertySettingDlgs\inputpromptdlg.h"
#include "views\propertySettingDlgs\keyworddlg.h"
#include "views\propertySettingDlgs\pagesettingdlg.h"
#include "views\propertySettingDlgs\structuringdlg.h"
#include "views\MainViewContainer.h"
#include "tools\CrashStack.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <ShlObj.h>  
#include <Shlwapi.h>//PathAppendW  
#include <QDateTime>
#pragma comment (lib, "shlwapi.lib")   
//#include "vld.h"

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QMutex mutex;
	mutex.lock();

	QString text;
	switch (type)
	{
	case QtDebugMsg:
		text = QString("Debug:");
		break;

	case QtWarningMsg:
		text = QString("Warning:");
		break;

	case QtCriticalMsg:
		text = QString("Critical:");
		break;

	case QtFatalMsg:
		text = QString("Fatal:");
	}
	QByteArray byMsg = msg.toLocal8Bit();
	char * chMsg = byMsg.data();
	QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString current_date = QString("(%1)").arg(current_date_time);
	QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

	QFile file("log.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&file);
	text_stream << message << "\r\n";
	file.flush();
	file.close();

	mutex.unlock();
}

#define Q_OS_WIN

#ifdef Q_OS_WIN
long __stdcall   callback(_EXCEPTION_POINTERS*   excp)
{
	CCrashStack crashStack(excp);
	QString sCrashInfo = crashStack.GetExceptionInfo();

	TCHAR my_documents[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	QString file_path = QString::fromWCharArray(my_documents);

	QDir *folder_path = new QDir;
	bool exist = folder_path->exists(file_path.append("\\MyApp"));
	if (!exist)
	{
		folder_path->mkdir(file_path);
	}
	delete folder_path;
	folder_path = nullptr;

	QString sFileName = file_path + "\\crash.log";

	QFile file(sFileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		file.write(sCrashInfo.toUtf8());
		file.close();
	}

	return   EXCEPTION_EXECUTE_HANDLER;
}
#endif


int main(int argc, char *argv[])
{
	QFile file("log.txt");
	if (file.exists())
	{
		file.remove();
	}
	file.close();
#ifdef Q_OS_WIN
	SetUnhandledExceptionFilter(callback);
#endif
	qInstallMessageHandler(outputMessage);
	QApplication a(argc, argv);
	MainViewContainer w;
	QFont font;
	font.setFamily("微软雅黑");
	a.setFont(font);
	a.setActiveWindow(&w);
	if (w.m_bIsClose == true)
	{
		a.quit();
		return 0;
	}
	else
	{
		w.show();
	}
	return a.exec();
}