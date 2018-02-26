#ifndef _FILE_DATA_H_
#define _FILE_DATA_H_


#include <QString>

class ViewObj;

class FileData
{
public:
	FileData(QString fileName, char fileType, int authority);
	~FileData();

public:
	QString getFileName();
	char getFileType();
	int getFileAuthority();
	bool isReadOnly();
	bool setFileAuthority(char authority);
	void saveReadWriteAuthorityLastReadWriteAuthority();
	void backupReadWriteAuthority();
	bool canEditTemplateProperty();
	bool canEditTemplate();
	bool canWrite();

protected:

protected:
	QString mFileName;
	// 文件类型
	char mFileType;
	// 文件读写权限
	int mFileAuthority;

	//保存上一次文件读写权限
	int mLastReadWriteAuthority;
};

#endif	//_FILE_DATA_H_