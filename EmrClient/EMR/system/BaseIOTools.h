#ifndef FILE_IO_H_
#define FILE_IO_H_

class QString;
class QFile;
class QDataStream;

#include <qglobal.h>
#include <QImage>
#include <QBuffer.h>
#include <QTextStream>
#include <QMessageBox.h>
#include <QFileDialog.h>

class MyAppEntrance;
class WriterManager;
struct StrItemData;

class BaseIOTools
{
public:
	BaseIOTools(WriterManager * mgr);
	~BaseIOTools();
public:
	bool openSaveFileDialog();
	bool saveFile(QString fileDir);
	bool openReadFileDialog();
	bool openFile(QString fileName, bool needAlignWhenNull);
	QDataStream *readFile();
	QDataStream *writeFile();
	bool readFile(const QString &fileName, bool needAlignWhenNull);
	QDataStream *writeFile(const QString &fileName);
	void writeBool(bool value);
	void writeByte(char value);
	void writeShort(short value);
	void writeInt(int value);
	void writeFloat(float value);
	void writeString(QString value);
	void writeImage(QImage * img);
	void wirteStrItemData(StrItemData strData);


	bool readBool();
	qint32 readByte();
	short readShort();
	int readInt();
	float readFloat();
	QString readString();
	QImage * readImage(QString imgName);
	StrItemData readStrItemData();


	void close();
	void setByteArray(QByteArray * tmp_array);
	QByteArray getByteArray();
	void initWriteManager(WriterManager * write);
private:
	WriterManager * mWriterMgr;
	QFile *mFile;
	QDataStream *mFileStream;
	QByteArray* mByteArray;
	QBuffer* buffer;
	//	(*fileStream) >> tmp_array;
	//	
	//	dataStreamToByteArray(*fileStream, tmp_array);
	//static	QBuffer buffer(&tmp_array);
	//	buffer.open(QIODevice::ReadWrite);
	//	QString  strtemp(tmp_array);
	//	QDataStream out(&buffer);
};

#endif // FILE_IO_H_
