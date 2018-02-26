#include "BaseIOTools.h"
#include <QString>
#include <QFile>
#include <iostream>
#include <assert.h>
#include <QDataStream>

#include "MyAppEntrance.h"
#include "WriterManager.h"
#include "MessageBox.h"
#include "BaseEditParam.h"

BaseIOTools::BaseIOTools(WriterManager * mgr):
mWriterMgr(mgr)
, mFile(NULL)

, mFileStream(NULL)
, mByteArray(NULL)
, buffer(NULL)
{

}
BaseIOTools::~BaseIOTools()
{
	if (mByteArray)
	{
		delete mByteArray;
		mByteArray = NULL;
	}
	close();
}
bool BaseIOTools::openFile(QString fileName, bool needAlignWhenNull)
{
	bool isSuccessful = readFile(fileName, needAlignWhenNull);
	return isSuccessful;
}
bool BaseIOTools::openReadFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(mWriterMgr->getAppEntrance());
	if (fileName.isEmpty())
		return false;
	else
	{
		return readFile(fileName, false);
	}
}
bool BaseIOTools::openSaveFileDialog()
{
	QString fileName = QFileDialog::getSaveFileName(mWriterMgr->getAppEntrance());
	if (fileName.isEmpty())
		return false;
	else
		return saveFile(fileName);
}
QDataStream *BaseIOTools::writeFile(const QString &fileName)
{
	mFile = new QFile(fileName);

	bool state = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
	if (!state)
	{
		MessageBox::showErrorMessageBox(QStringLiteral("错误"), QStringLiteral("写入文件失败"));
		return NULL;
	}
	mFileStream = new QDataStream(mFile);
	//ByteOrder * q = mFileStream->byteOrder();
	//mFileStream->writeBytes()

	/*if (mByteArray)
	{
		mByteArray->clear();
		buffer = new QBuffer(mByteArray);
		buffer->open(QIODevice::WriteOnly);
		mFileStream = new QDataStream(buffer);
	}
	else
	{
		mByteArray = new QByteArray();
		buffer = new QBuffer(mByteArray);
		buffer->open(QIODevice::WriteOnly);
		mFileStream = new QDataStream(buffer);
	}*/
	return mFileStream;
}

bool BaseIOTools::saveFile(QString fileDir)
{
	mFile = new QFile(fileDir);

	bool state = mFile->open(QIODevice::WriteOnly);
	if (!state)
	{
		MessageBox::showErrorMessageBox(QStringLiteral("错误"), QStringLiteral("导出文件失败"));
		return false;
	}
	else
	{
		mFileStream = new QDataStream(mFile);
		/*QDataStream out(mFile);
		out << mByteArray;
		close();*/
		return true;
	}
}
bool BaseIOTools::readFile(const QString &fileName, bool needAlignWhenNull)
{
	mFile = new QFile(fileName);

	bool state = mFile->open(QIODevice::ReadOnly);
	if (!state)
	{
		if (needAlignWhenNull)
		MessageBox::showErrorMessageBox(QStringLiteral("错误"), QStringLiteral("写入文件失败"));
		return false;
	}
	else
	{
		mFileStream = new QDataStream(mFile);
		return true;
		/*QDataStream out(mFile);
		out << mByteArray;
		close();*/
	
	}
}
QDataStream *BaseIOTools::readFile()
{
#ifdef QT_DEBUG
	//assert(isSuccess);
	//if (!isSuccess)
	//	return NULL;
#endif
	//if (source)
	//	mByteArray = source;
	if (mByteArray)
	{
		if (mByteArray->size() <= 0)
		{
			delete mByteArray;
			mByteArray = NULL;
			return NULL;
		}
		buffer = new QBuffer(mByteArray);
		buffer->open(QIODevice::ReadOnly);
		mFileStream = new QDataStream(buffer);
	}
	return mFileStream;
}
QDataStream *BaseIOTools::writeFile()
{
	//if (source)
	//{
	//	mByteArray = source;
	//}
	if (mByteArray)
	{
		mByteArray->clear();
		buffer = new QBuffer(mByteArray);
		buffer->open(QIODevice::WriteOnly);
		mFileStream = new QDataStream(buffer);
	}
	else
	{
		mByteArray = new QByteArray();
		buffer = new QBuffer(mByteArray);
		buffer->open(QIODevice::WriteOnly);
		mFileStream = new QDataStream(buffer);
	}
	return mFileStream;
}

void BaseIOTools::writeBool(bool value)
{
	*mFileStream << value;
}
void BaseIOTools::writeByte(char param)
{
	qint32 value = param;
	*mFileStream << value;
}
void BaseIOTools::writeShort(short value)
{
	*mFileStream << value;
}
void BaseIOTools::writeInt(int value)
{
	*mFileStream << value;
}
void BaseIOTools::writeFloat(float value)
{
	*mFileStream << value;
}
void BaseIOTools::writeString(QString value)
{
	*mFileStream << value;
}
void BaseIOTools::writeImage(QImage * img)
{
	*mFileStream << *img;
}
void BaseIOTools::wirteStrItemData(StrItemData strData)
{
	*mFileStream << strData.strType;
	*mFileStream << strData.strPath;
	*mFileStream << strData.strRect;
	*mFileStream << strData.strLine;
	*mFileStream << strData.strPoint;
	*mFileStream << strData.strQstr;
	*mFileStream << strData.strPolygon;
	*mFileStream << strData.strOffsetPoint;
}

StrItemData BaseIOTools::readStrItemData()
{
	StrItemData strData;
	*mFileStream >> strData.strType;
	*mFileStream >> strData.strPath;
	*mFileStream >> strData.strRect;
	*mFileStream >> strData.strLine;
	*mFileStream >> strData.strPoint;
	*mFileStream >> strData.strQstr;
	*mFileStream >> strData.strPolygon;
	*mFileStream >> strData.strOffsetPoint;
	return strData;
}


bool BaseIOTools::readBool()
{
	bool value;
	*mFileStream >> value;
	return value;
}
qint32 BaseIOTools::readByte()
{
	qint32 value;
	*mFileStream >> value;
	return value;
}
short BaseIOTools::readShort()
{
	short value;
	*mFileStream >> value;
	return value;
}
int BaseIOTools::readInt()
{
	int value;
	*mFileStream >> value;
	return value;
}
float BaseIOTools::readFloat()
{
	float value;
	*mFileStream >> value;
	return value;
}
QString BaseIOTools::readString()
{
	QString value;
	*mFileStream >> value;
	return value;
}
QImage * BaseIOTools::readImage(QString imgName)
{
	QImage * img = new QImage();
	//img = new QImage(imgName);

	*mFileStream >> *img;
	return img;
}
void BaseIOTools::close()
{
	if (mFile == NULL)
	{
		
	}
	else
	{
		mFile->close();
		delete mFile;
		mFile = NULL;
	}
	if (buffer)
	{
		buffer->close();
		delete buffer;
		buffer = NULL;
	}
	if (mByteArray)
	{
		delete mByteArray;
		mByteArray = NULL;
	}
	if (mFileStream)
	{
		delete mFileStream;
		mFileStream = NULL;
	}
	mWriterMgr = NULL;
}
void BaseIOTools::setByteArray(QByteArray * tmp_array)
{
	if (mByteArray)
	{
		delete mByteArray;
		mByteArray = NULL;
	}
	mByteArray = new QByteArray(*tmp_array);
}
QByteArray BaseIOTools::getByteArray()
{
	return *mByteArray;
}
void BaseIOTools::initWriteManager(WriterManager * write)
{
	mWriterMgr = write;
}
