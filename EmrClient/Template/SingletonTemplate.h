/******************************************************************* 
Copyright:    

Created by: zhangrong

Description:非线程安全单例模板类
******************************************************************/
 
#ifndef SINGLETONTEMPLATE_H
#define SINGLETONTEMPLATE_H

template<class T>
class Singleton
{

public:
	static T & getSingleton()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new T();
		}
		return *m_pInstance;
	}
	static void destorySingleton()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance=NULL;
		}
	}
protected:
	Singleton(void){}
	~Singleton(void){}

private:
	Singleton(const Singleton &){}
	Singleton& operator=(const Singleton &){}

	static T * m_pInstance;
};

template<class T>
T * Singleton<T>::m_pInstance = NULL;

#endif // SINGLETONTEMPLATE_H

