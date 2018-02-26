#ifndef MSC_VER_H
#define MSC_VER_H
/********qt版本高于等于qt5，VS版本高于或等于VS2010 ******/
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif

//ui创建函数占位宏
#ifndef CREATE_UI_BEGIN
#define CREATE_UI_BEGIN
#define CREATE_UI_END
#endif

#endif //define MSC_VER_H 