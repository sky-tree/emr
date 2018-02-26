/******************************************************************* 
Copyright:    

Created by: zhangrong

Description:导航页面
******************************************************************/  
 
#ifndef NAVITATIONVIEW_H
#define NAVIGATIONVIEW_H

#include <QWidget>
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class NavigationView : public QWidget
{
public:
	NavigationView(QWidget *parent = NULL);
	~NavigationView();
};

#endif //define NAVIGATIONVIEW_H