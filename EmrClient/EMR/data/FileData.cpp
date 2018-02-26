#include "FileData.h"

#include "StrConstants.h"
#include "AuthorityConstants.h"
FileData::FileData(QString fileName, char fileType, int authority)
{
	mFileName = fileName;
	mFileType = fileType;
	mFileAuthority = authority;
}

FileData::~FileData()
{
}

QString FileData::getFileName()
{
	return mFileName;
}
char FileData::getFileType()
{
	return mFileType;
}
int FileData::getFileAuthority()
{
	return mFileAuthority;
}
bool FileData::isReadOnly()
{
	if (mFileAuthority == AuthorityConstants::Authority::ONLY_READ_VIEW_GRAY || mFileAuthority == AuthorityConstants::Authority::ONLY_READ_VIEW_NORMAL)
	{
		return true;
	}
	return false;
}
bool FileData::setFileAuthority(char authority)
{
	if (mFileAuthority == authority)
	{
		return false;
	}
	mFileAuthority = authority;
	return true;
}
void FileData::saveReadWriteAuthorityLastReadWriteAuthority()
{
	mLastReadWriteAuthority = mFileAuthority;
}
void FileData::backupReadWriteAuthority()
{
	mFileAuthority = mLastReadWriteAuthority;
}
bool FileData::canEditTemplateProperty()
{
	return mFileAuthority >= AuthorityConstants::Authority::WRITE_ALL;
}
bool FileData::canEditTemplate()
{
	return mFileAuthority >= AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT;
}
bool FileData::canWrite()
{
	return mFileAuthority >= AuthorityConstants::Authority::WRITE_WITHOUT_ELEMENT;
}