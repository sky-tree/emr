#include "dataModel/RequestJsonDataModel.h"
#include "tools/qjson/serializer.h"
#include <QStringList>
#include "tools/qjson/qobjecthelper.h"
#include "tools/qjson/parser.h" 
//RequestJsonDataModel *RequestJsonDataModel::pInstance = NULL;
RequestJsonDataModel::RequestJsonDataModel()
{

}
QByteArray RequestJsonDataModel::setPostData(QJsonObject reqJson)
{
    document.setObject(reqJson);
    QByteArray post_data1 = document.toJson(QJsonDocument::Compact);
	post_data.clear();
    post_data = "json="+post_data1;

	return post_data;
}

QByteArray RequestJsonDataModel::setPostData(QByteArray reqByteArray)
{
	post_data.clear();
	post_data = "json=" + reqByteArray;
	return post_data;
}

QByteArray RequestJsonDataModel::serialize(const QVariant &v)
{
	QJson::Serializer serializer;
	bool ok;
	QByteArray json = serializer.serialize(v, &ok);

	if (ok) 
	{
		return json;
	}
	else 
	{
		// qCritical() << "Something went wrong:" << serializer.errorMessage();
		 return serializer.errorMessage().toUtf8();
	}
}
QString RequestJsonDataModel::getReqUrl(QString url, QByteArray reqData)
{
	return url + QString::fromUtf8(setPostData(reqData));
}
void RequestJsonDataModel::clearReqData()
{
      if(reqJson.length() > 0)
      {
        const QStringList keyList = reqJson.keys();

        for(int i = 0;i < keyList.length();i++)
        {
            reqJson.remove(keyList.at(i));
        }
      }
      post_data.clear();
}



