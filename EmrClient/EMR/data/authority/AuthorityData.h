#ifndef _AUTHORITY_DATA_H_
#define _AUTHORITY_DATA_H_



#include <QString>

class BaseIOTools;

class AuthorityData
{
public:
	AuthorityData();
	AuthorityData(int level, QString job, int authorityViewIdx);
	~AuthorityData();


public:
	void doSave(BaseIOTools * ioTools);
	void doLoad(BaseIOTools * ioTools);

public:
	int getLevel();
	QString getJobName();
	//int getAuthorityViewIndex();
	 
private:
	// 权限等级
	int mLevel;
	// 职位
	QString mJobName;
	// 对应权限在痕迹显示列表中的索引
	int mAuthorityViewIndex;
};

#endif
