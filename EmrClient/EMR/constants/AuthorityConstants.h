#ifndef _AUTHORITY_CONSTANTS_H_
#define _AUTHORITY_CONSTANTS_H_


#include <QString>
//#define DEBUG_MODE

class AuthorityConstants
{

public:
	// 文件类型：模板、宏元素、病历等
	class FileType
	{
	public:
		static const int MEDICAL_RECORDS = 1;								// 病历
		static const int TEMPLETE = 3;										// 模板
		static const int COURSE_RECORDS = 11;								// 病程记录
		//static const char GLOBAL_ELEMENT = 2;								// 宏元素
	};
	class Authority
	{
	public:
		static const int ONLY_READ_VIEW_GRAY = 1;										// 只读同时置灰显示
		static const int ONLY_READ_VIEW_NORMAL = 2;										// 只读但显示正常
		static const int WRITE_WITHOUT_ELEMENT = 12;								// 可读写，但不能编辑元素属性

		static const int WRITE_ALL = 100;								// 可读写，并且能编辑元素属性
	};
};


#endif // _AUTHORITY_CONSTANTS_H_