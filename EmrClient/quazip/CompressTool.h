#ifndef _COMPRESSTOOL_H_
#define _COMPRESSTOOL_H_
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class  QByteArray;
class QString;
class QStringList;
class QIODevice;
class CompressTool
{
public:
	CompressTool();
	~CompressTool();
	static bool copyData(QIODevice &inFile, QIODevice &outFile);
	static  void compressQString(QString &src, QString & des);//Ñ¹Ëõ×Ö·û´®
	static void extractQString(QString &src, QString & des); //½âÑ¹×Ö·û´®
	static  void compressByteArray(QByteArray &srcByteArray, QByteArray & desByteArray);//Ñ¹ËõByteArray 
	static void extractByteArray(QByteArray &srcByteArray, QByteArray & desByteArray);//½âÑ¹ByteArray
	static QString compressString(QString srcStr);
	static QString extractString(QString srcStr);
	static bool extract(const QString& in_file_path, const QString& out_file_path);
	static void extractFile(const QString& in_file_path, const QString& out_file_path);
	static void compressFile(const QString& in_file_path, const QString& out_file_path);
	static void saveByteArrayToFile(const QString& file_path, QByteArray& srcData);
	static void readByteArrayFromFile(const QString& file_path, QByteArray& srcData);
	static 	QStringList getFileNames(const QString &path);
	static void deleteFilesInDir(const QString &path);
};
#endif