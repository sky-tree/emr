/*********************************************************************************
*Copyright(C),Shine
*FileName:  RequestJsonDataModel
*Author:  liuyi
*Modifier:caokewen
*Version:  1.0
*Date:  //2016/3/9
*Description:  //此类为一个公共的Json的数据操作类，用于Json的序列化或反序列化。
**********************************************************************************/
#ifndef REQUESTJSONDATAMODEL_H
#define REQUESTJSONDATAMODEL_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include "Template/SingletonTemplate.h"
// 代码一定要是: UTF-8(BOM)       
//qt版本高于等于qt5，VS版本高于或等于VS2010    
#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif
/**
 * 网络请求Json数据封装
 * @brief The RequestJsonDataModel class
 */
class RequestJsonDataModel :public Singleton<RequestJsonDataModel>
{
	//qt版本低于qt5    
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)       
	//VS版本低于VS2010     
#if defined(_MSC_VER) && (_MSC_VER < 1600)        
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
#else        
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif        
#endif   
public:
    RequestJsonDataModel();
    void clearReqData();
	QByteArray setPostData(QJsonObject  reqJson);
	QByteArray setPostData(QByteArray reqByteArray);
	/*Qjson序列化方法的再次封装*/
	QByteArray serialize(const QVariant &v);

	/*
	 JsonToObject或者ObjectToJson,其数据的类必须从QObject中继承下来，其中的属性必须为private;
	 其中结构如何构造，如下：
	 QvariantMap var;
	 如果其中的格式为 { A：{B:C}};  由于B：c,是最底层,则直接有对象转换过来，QVariantMap variant = QJson::QObjectHelper::qobject2qvariant
	 可以直接用 var.insert("A",variant);
	 如果格式为 A:[{B:{C:D}}],的格式，那么则需要用variantList varlist>>{B:{C:D}},var.insert(A,varlist);
	*/
	template <typename T>
	void qJsonToObject(QString pJson, T *pObject)
	{
		if (pJson.isEmpty() == true || pObject == NULL)return;
		QVariant variantJson = parser.parse(pJson.toUtf8());
		QJson::QObjectHelper::qvariant2qobject(variantJson.toMap(), pObject);
	}
	template<typename T>
	void qObjectToJson(QByteArray &pJson, T *pObject)
	{
		if (pObject == NULL) return;
		QVariantMap variant = QJson::QObjectHelper::qobject2qvariant(pObject);
		QJson::Serializer serializer;
		pJson = serializer.serialize(variant);
	}
	template<typename T>
	QVariantMap qObjectToVariantMap(T *pObject)
	{
		if (pObject == NULL)return;
		QVariantMap variantMap = QJson::QObjectHelper::qobject2qvariant(pObject);
		return variantMap;
	}
	/*获取拼IP和reqData接好的URL访问路径*/
	QString getReqUrl(QString url,QByteArray reqData);
    QJsonObject  reqJson;
    QJsonDocument  document;
    QByteArray  post_data;
protected:
   // static RequestJsonDataModel *pInstance;
};

#endif // REQUESTJSONDATAMODEL_H
