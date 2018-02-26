#include "CommonNetWorkingRunnable.h"
  
#include "dataModel/RequestJsonDataModel.h"
#include "tools/ConstCollection.h"
#include "tools/qjson/serializer.h"
#include "tools/qjson/parser.h"
#include "tools/qjson/qobjecthelper.h"
#include "tools/HisConstCollection.h"
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QDate>
#include <QString>
#include "MessageBox.h"
CommonNetWorkingRunnable::CommonNetWorkingRunnable(QObject *parent) :
QObject(parent)
{
	network_manager = NULL;
}

CommonNetWorkingRunnable::CommonNetWorkingRunnable(QByteArray post_data, QUrl reqUrl, QObject *parent)
:QObject(parent)
{
	this->post_data = post_data;
	this->reqUrl = reqUrl;
	network_manager = new QNetworkAccessManager();
	/*设置头信息*/
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	network_request.setHeader(QNetworkRequest::ContentLengthHeader, post_data.length());
	/*设置url*/
	network_request.setUrl(reqUrl);
	network_manager->post(network_request, post_data);
}
void CommonNetWorkingRunnable::setPostDataAndUrl(QByteArray post_data, QUrl reqUrl)
{
	this->post_data = post_data;
	this->reqUrl = reqUrl;
	network_manager = new QNetworkAccessManager();
	/*设置头信息*/
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	network_request.setHeader(QNetworkRequest::ContentLengthHeader, post_data.length());
	/*设置url*/
	network_request.setUrl(reqUrl);
	network_manager->post(network_request, post_data);
}

void CommonNetWorkingRunnable::setPutDataAndUrl(QByteArray post_data, QUrl reqUrl)
{
	this->post_data = post_data;
	this->reqUrl = reqUrl;
	network_manager = new QNetworkAccessManager();
	/*设置头信息*/
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	network_request.setHeader(QNetworkRequest::ContentLengthHeader, post_data.length());
	/*设置url*/
	network_request.setUrl(reqUrl);
	network_manager->put(network_request, post_data);
}

void CommonNetWorkingRunnable::setGetUrl(QUrl reqUrl)
{
	this->reqUrl = reqUrl;
	network_manager = new QNetworkAccessManager();
	network_request.setUrl(reqUrl);
	network_manager->get(network_request);
}

void CommonNetWorkingRunnable::setDeleteUrl(QUrl reqUrl)
{
	this->reqUrl = reqUrl;
	network_manager = new QNetworkAccessManager();
	network_request.setUrl(reqUrl);
	network_manager->deleteResource(network_request);
}
//
CommonNetWorkingRunnable *CommonNetWorkingRunnable::getNewInstance(QByteArray post_data, QUrl reqUrl)
{
	RequestJsonDataModel::destorySingleton();
	CommonNetWorkingRunnable *pInstance = new CommonNetWorkingRunnable(post_data, reqUrl);
	if (pInstance != NULL)
	{
		return pInstance;
	}
	return NULL;
}

QString CommonNetWorkingRunnable::replyResDataFinished(QNetworkReply *reply)
{
	QString result="";
	if (reply->error() == QNetworkReply::NoError)
	{
		resData = NULL;
		resData = reply->readAll();  //获取字节
		result.prepend(resData);//转化为字符串
	}
	else
	{
		MessageBox::showErrorMessageBox(QStringLiteral("提示"), QStringLiteral("网络异常,请检查网络链接是否正常!"));
	}

	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	return result;
}
/*获取地区数据*/
#define dInstance RequestJsonDataModel::getSingleton() 
void CommonNetWorkingRunnable::getRegionData(QString REGION_ID, int regionType)
{
	this->regionType = regionType;
	//this->views = views;
	//请求json数据封装
	dInstance.clearReqData();
	dInstance.reqJson.insert("REGION_ID", REGION_ID);
	dInstance.setPostData(dInstance.reqJson);
	//启动联网 传递 请求数据 和 URL
	this->setPostDataAndUrl(dInstance.post_data
		, QUrl(ConstCollection::getSingleton().GET_AREA_URL));
	//建立信号槽，联网完成后的事件触发处理
	connect(this->network_manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyGetAreaRegionDataFinished(QNetworkReply*, QString*)));
}
void CommonNetWorkingRunnable::replyGetAreaRegionDataFinished(QNetworkReply *reply, QString *pJson)
{	
	if (reply->error() == QNetworkReply::NoError)
	{
		resData = NULL;
		resData = reply->readAll();  //获取字节
		pJson->prepend(resData);//转化为字符串
	}
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
}