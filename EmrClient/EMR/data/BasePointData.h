#ifndef BASE_POINT_DATA_H_
#define BASE_POINT_DATA_H_


// 基本坐标数据
class BasePointData
{
public:
	BasePointData(int x, int y);
	BasePointData(BasePointData * source);
	~BasePointData();

public:
	int mX;
	int mY;
};

#endif
