/*********************************************************************************
*Copyright(C),Shine
*FileName:  customComboBoxDelegate
*Author:  liuyi
*Modifier:caokewen
*Version:  1.0
*Date:  //2016/3/8
*Description:  //封装连接网络，只向外提供相关的接口即可。
**********************************************************************************/
#ifndef COMMONNETWORKINGRUNNABLE_H
#define COMMONNETWORKINGRUNNABLE_H
 
//#include <views/PlanAndSummaryViews.h>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include"Template/SingletonTemplate.h"
// 代码一定要是: UTF-8(BOM)      
//qt版本高于等于qt5，VS版本高于或等于VS2010    
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
class CommonNetWorkingRunnable : public QObject, virtual public Singleton<CommonNetWorkingRunnable>
{
    Q_OBJECT
public:
    explicit CommonNetWorkingRunnable(QObject *parent = 0);
    explicit CommonNetWorkingRunnable(QByteArray post_data,QUrl reqUrl,QObject *parent = 0);
    static CommonNetWorkingRunnable *getNewInstance(QByteArray post_data,QUrl reqUrl);
	/*联网推送给服务器json数据*/
    QByteArray  post_data;
	/*联网服务器返回json数据*/
	QByteArray  resData;
    QNetworkAccessManager *network_manager;
    QNetworkRequest network_request;
    QUrl reqUrl;
    QWidget *parent;

    int ownerType;
    bool firstInitApp;
//    PlanAndSummaryViews *views;
    int regionType;
	/*设置Post方式的联网请求 1.post_data：请求数据 2.reqUrl请求联网路径*/
    void setPostDataAndUrl(QByteArray post_data,QUrl reqUrl);
	/*设置Put方式的联网请求 1.post_data：请求数据 2.reqUrl请求联网路径*/
	void setPutDataAndUrl(QByteArray post_data, QUrl reqUrl);
	/*设置get方式的联网请求 1.reqUrl请求联网路径*/
	void setGetUrl(QUrl reqUrl);
	/*设置delete方式的联网请求 1.reqUrl请求联网路径*/
	void setDeleteUrl(QUrl reqUrl);
	/*联网正常完成服务器返回数据*/
	QString replyResDataFinished(QNetworkReply *reply);
	/*界面跳转*/
	void initViews(QString reqId, QWidget *parent);
	void getRegionData(QString REGION_ID, int regionType);
public slots:
    void replyGetAreaRegionDataFinished(QNetworkReply *reply,QString *pJson);
};

#endif // COMMONNETWORKINGRUNNABLE_H
