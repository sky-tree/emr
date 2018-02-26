#ifndef GLOBAL_TEMPLATE_H_
#define GLOBAL_TEMPLATE_H_



// 全局数组回收模板
template<typename T>
static bool isNullVector(QList<T *> *vec)
{
	return (vec == NULL) || vec->size() < 1;
}
template<typename T>
static void deleteVector(QList<T *> *vec)
{
	unsigned int length = vec->size();

	if (length <= 0) return;

	for (unsigned int i = 0; i < length; i++)
	{
		T * p = vec->at(i);
		if (p == NULL) continue;
		else
		{
			delete p;
			p = NULL;
		}
	}
	vec->clear();
	delete vec;
}
template<typename T>
static void deleteVectorContent(QList<T *> *vec)
{
	unsigned int length = vec->size();

	if (length <= 0) return;

	for (unsigned int i = 0; i < length; i++)
	{
		T * p = vec->at(i);
		if (p == NULL) continue;
		else
		{
			delete p;
			p = NULL;
		}
	}
	vec->clear();
}
template<typename T>
static bool qListHaveChildren(QList<T *> *vec)
{
	if (!vec)
		return false;
	if (vec->size() <= 0)
		return false;
	return true;
}

template<typename T>
bool deletePointerArray(T * arr, int length)
{
	if (arr == NULL) return false;

	for (int i = 0; i < length; i++)
	{
		if (arr[i] == NULL)
			continue;
		else
		{
			delete arr[i];
			arr[i] = NULL;
		}
	}

	delete[] arr;
	return true;
}

#endif // GLOBAL_TEMPLATE_H_
