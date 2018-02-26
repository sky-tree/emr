#ifndef _MEASUREWH_H_
#define _MEASUREWH_H_
class MeasureWH
{
public:
	MeasureWH();
	~MeasureWH();
	void clearData(){
		measureWidth = 0;
		measureHeight = 0;
	}
	int measureWidth;
	int measureHeight;
	//char measureWidthMode;
	//char measureHeightMode;
};
#endif
