#include "CompressTool.h"
#include "JlCompress.h"
#include "QCoreApplication.h"
#pragma comment(lib,"zdll.lib")
#include "QMessageBox"
//#pragma comment(lib,"quazip.lib")
#include <QByteArray>
#include <QString>
#include <QBuffer>
#include "MyAppEntrance.h"
bool CompressTool::copyData(QIODevice &inFile, QIODevice &outFile)
{
	while (!inFile.atEnd()) {
		char buf[4096];
		qint64 readLen = inFile.read(buf, 4096);
		if (readLen <= 0)
			return false;
		if (outFile.write(buf, readLen) != readLen)
			return false;
	}
	return true;
}
CompressTool::CompressTool()
{
}


CompressTool::~CompressTool()
{
}
void CompressTool::compressQString(QString &src, QString & des)
{
	QByteArray srcbyteArray = src.toUtf8();
	QByteArray desByteArray;
	compressByteArray(srcbyteArray, desByteArray);
	des = QString::fromUtf8(desByteArray);
}
void CompressTool::extractQString(QString &src, QString & des)
{
	QByteArray srcbyteArray = src.toUtf8();
	QByteArray desByteArray;
	extractByteArray(srcbyteArray, desByteArray);
	des = QString::fromUtf8(desByteArray);
}
void CompressTool::compressByteArray(QByteArray &srcByteArray, QByteArray & desByteArray)
{
	QString exePath = QCoreApplication::applicationDirPath();
	if (!exePath.endsWith(QDir::separator()))
	{
		exePath += QDir::separator();
	}
	QString srcFileNave = exePath+"111.data";
	QString desFileNave = exePath + "222.data";
	saveByteArrayToFile(srcFileNave, srcByteArray);
	JlCompress::compressFile(desFileNave,srcFileNave);
	readByteArrayFromFile(desFileNave, desByteArray);
}
void CompressTool::extractByteArray(QByteArray &srcByteArray, QByteArray & desByteArray)
{
	QString exePath = QCoreApplication::applicationDirPath();
	if (!exePath.endsWith(QDir::separator()))
	{
		exePath += QDir::separator();
	}
	QString extractDirName = "333";
	QString srcFileNave = exePath + extractDirName+".zip";
	
	saveByteArrayToFile(srcFileNave, srcByteArray);
	/*QFileInfo fi;
	QString file_full;
	fi = QFileInfo(file_full);*/
	deleteFilesInDir(exePath);
	MyAppEntrance::saveByteArrayBeforOpen(srcByteArray, "解压前数据88.dat");
	JlCompress::extractDir(srcFileNave, exePath);
	QStringList nameList = getFileNames(exePath);
	QString desFileNave;
	if (nameList.size()==1)
	{
		desFileNave = nameList.at(0);
		readByteArrayFromFile(exePath+desFileNave, desByteArray);
	}
	else
	{

	}

}
QString CompressTool::compressString(QString srcStr)
{
	return NULL;
}
QString CompressTool::extractString(QString srcStr)
{
	return NULL;
}
bool CompressTool::extract(const QString& in_file_path, const QString& out_file_path)
{

	QuaZip archive(in_file_path);
	if (!archive.open(QuaZip::mdUnzip))
		return false;

	QString path = out_file_path;
	if (!path.endsWith("/") && !out_file_path.endsWith("\\"))
		path += "/";

	QDir dir(out_file_path);
	if (!dir.exists())
		dir.mkpath(out_file_path);

	for (bool f = archive.goToFirstFile(); f; f = archive.goToNextFile())
	{
		QString filePath = archive.getCurrentFileName();
		QuaZipFile zFile(archive.getZipName(), filePath);
		zFile.open(QIODevice::ReadOnly);
		QByteArray ba = zFile.readAll();
		zFile.close();

		if (filePath.endsWith("/"))
		{
			dir.mkpath(filePath);
		}
		else
		{
			QFile dstFile(path + filePath);
			if (!dstFile.open(QIODevice::WriteOnly))
				return false;
			dstFile.write(ba);
			dstFile.close();
		}
	}
	return true;
}
void  CompressTool::extractFile(const QString& in_file_path, const QString& out_file_path)
{
	JlCompress::extractFile(in_file_path, out_file_path);
	
}
void  CompressTool::compressFile(const QString& in_file_path, const QString& out_file_path)
{
	JlCompress::compressFile(in_file_path, out_file_path);
}
void CompressTool::saveByteArrayToFile(const QString& file_path, QByteArray& srcData)
{
	if (file_path.isNull())
	{
		
		return;
	}
	/*if (!file_path.contains(".dat"))
	{
		file_path = file_path + ".dat";
	}*/
	QFile file(file_path);
	if (file.exists())
	{
		file.open(QFile::WriteOnly | QFile::Truncate);
		file.close();
	}
	else
	{
		//file.open(QIODevice::WriteOnly);
	}
	if (!file.open(QIODevice::WriteOnly)) {
		//std::cerr << "Cannot open file for writing: "
		//	<< qPrintable(file.errorString()) << std::endl;
		QMessageBox::information(NULL, "CompressTool::saveByteArrayToFile保存状态1", "文件不可写", QMessageBox::Yes);
		return;
	}
	QBuffer  buffer(&srcData);
	buffer.open(QIODevice::ReadOnly);
	copyData(buffer,file);
	//delete fDialog;
	//QDataStream out(&file);
	//out.setVersion(QDataStream::Qt_4_3);
	//out << srcData;
	buffer.close();
	file.close();
	//QMessageBox::information(NULL, "保存状态2", "保存成功", QMessageBox::Yes);
	
}
void CompressTool::readByteArrayFromFile(const QString& file_path, QByteArray& srcData)
{
	if (file_path.isNull())
	{

		return;
	}
	QFile file(file_path);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(NULL, "CompressTool::readByteArrayFromFile 打开文件状态1", "打开文件不可读", QMessageBox::Yes);
		return;
	}
	//QDataStream in(&file);
	//in.device()->seek(0);
	//in.setVersion(QDataStream::Qt_4_3);
	//QByteArray array;
	QBuffer  buffer(&srcData);
	buffer.open(QIODevice::WriteOnly);
	copyData(file, buffer);
	//in >> srcData;
	//QMessageBox::information(NULL, "打开文件状态1", "打开文件成功", QMessageBox::Yes);
	file.close();
	buffer.close();
}
QStringList CompressTool::getFileNames(const QString &path)
{
    QDir dir(path);
   QStringList nameFilters;
   nameFilters << "*.data";
   QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    return files;
}
void CompressTool::deleteFilesInDir(const QString &path)
{
	QString pathTemp;
	pathTemp = path;
	if (!path.endsWith(QDir::separator()))
	{
		pathTemp += QDir::separator();
	}
	QStringList nameList = getFileNames(pathTemp);
	for (int i = 0; i < nameList.size();i++)
	{
		QString nae = nameList.at(i);
		QFile fileTemp(pathTemp + nae);
		fileTemp.remove();
	}
}
