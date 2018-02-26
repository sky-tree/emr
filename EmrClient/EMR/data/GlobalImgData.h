#ifndef GLOBAL_IMG_H_
#define GLOBAL_IMG_H_


#include <QString>
#include <QImage>

// 全局使用的图片数据，对图片进行封装，避免重复创建图片
class GlobalImgData
{
public:
	GlobalImgData(QString imgName);
	//GlobalImgData(QString imgName, QImage * img);

	virtual ~GlobalImgData();


public:
	//bool canLoadImg();
	bool isSameImg(QString name);
	QImage * getImg();
	// 图片使用数量-1，如果使用数小于1，则返回true，代表需要由外面delete this
	bool delNum();
	void addNum();

	QString getImgName();

protected:
	QString mImgName;
	QImage * mImage;

	int mUseNum;

	//bool mCanLoadImg;

};

#endif
