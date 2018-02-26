#include "HtmlDocument.h"
#include "EMR\system\GlobalTemplate.h"
#include "commonWidgets/TabWidgetCollection.h"
 
#include <iostream>
#include <string.h>
#include <cstring>
#include <sstream>
#include <QDebug>
#include "CompressTool.h"
using namespace std;

HTMLDocument::HTMLDocument(QList<htmlBaseObj *> &list)
{
	startHTML();
	startHead();
	insertTitle(tr("电子病历"));
	insertStyle();
	endHead();
	startBody();

	for (size_t i = 0; i < list.size(); i++)
	{
		switch (list[i]->mType)
		{
		case 1:
		{
				  list[i]->mY -= list[i]->mFontSize;
				  insertText(list[i]);
		}
			break;
		case 2:
		{
				  insertImage(list[i]);
		}
			break;
		case 3:
		{
				  insertLine(list[i]);
		}
			break;
		default:
			break;
		}
	}

	endBody();
	endHTML();
}

void HTMLDocument::startHTML()
{
    data.append("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\"><html> \n");
}

void HTMLDocument::endHTML()
{
    data.append("</html>");
}

void HTMLDocument::startHead()
{
    data.append("<head> \n <meta http-equiv=\"Content-Type\" name=\"medicalrecord\" content=\"text/html; charset=utf-8\" />");
}

void HTMLDocument::endHead()
{
    data.append("</head> \n");
}

void HTMLDocument::insertTitle(QString title)
{
    QString tem = "<title>%1</title>";
    data.append(tem.arg(title));
}

void HTMLDocument::insertStyle(int fontSize)
{
    QString tem = "<style type=\"text/css\"> \
            body{font-size:%1px;} \
            </style>";
            data.append(tem.arg(fontSize));
}

void HTMLDocument::startBody()
{
    data.append("<body>\n");
}

void HTMLDocument::endBody()
{
    data.append("</body>\n");
}

void HTMLDocument::insertLine(htmlBaseObj* obj)
{
	char info[256];
	memset(info, 0, sizeof(info));
	//margin
	
	if (obj->mStartPoint.y() != obj->mEndPoint.y())
	{
		sprintf(info, "<table height=%d style=\"border-color:rgb(%d, %d, %d); border-left-style:solid; border-width:1px; position:absolute; margin:%dpx 0px 0px %dpx;\"><tr><td valign=bottom></table>", obj->mEndPoint.y() - obj->mStartPoint.y(), obj->mFontColor.red(), obj->mFontColor.green(), obj->mFontColor.blue(), obj->mStartPoint.y(), obj->mStartPoint.x());
	}
	else
	{
		sprintf(info, "<hr width=%d style=\"border-color:rgb(%d, %d, %d); border-style:solid; border-width:1px; position:absolute; margin:%dpx 0px 0px %dpx;\"></hr>", obj->mEndPoint.x() - obj->mStartPoint.x(), obj->mFontColor.red(), obj->mFontColor.green(), obj->mFontColor.blue(), obj->mStartPoint.y(), obj->mStartPoint.x());
	}
		
	this->data.append(QString(info));
}

void HTMLDocument::insertImage(htmlBaseObj* obj)
{
	char info[64];
	memset(info, 0, sizeof(info));
	//margin
	sprintf(info, "margin:%dpx 0px 0px %dpx;", obj->mY, obj->mX);
	QString margin(info);

	QString imageCode = obj->mByteArray->toBase64();

	stringstream strStream;
	//strStream << "<span style=\"position:absolute;" <<  margin.toStdString() << "><img src= \"file:///"
	//	<< obj->mStr.toStdString() << "\"" << /*" width=\""<< data.width<< "\""<< " height=\""<< data.height<< "\"*/"> </span>\n";
	strStream << "<span style=\"position:absolute;" << margin.toStdString() <<" \"><img src=\"data:image/png;base64,"<< imageCode.toStdString()
		<<"\"></span>\n";
		this->data.append(QString(strStream.str().c_str()));
}

void HTMLDocument::insertText(htmlBaseObj* obj)
{
	char info[64];
	memset(info, 0, sizeof(info));

	//颜色
	sprintf(info, "color:rgb(%d,%d,%d);", obj->mFontColor.red(), obj->mFontColor.green(), obj->mFontColor.blue());
	QString color(info);

	//位置
	sprintf(info, "margin:%dpx 0px 0px %dpx;", obj->mY, obj->mX);
	QString margin(info);

	//字体大小
	stringstream strStream;
	strStream << "font-size:" << obj->mFontSize << "px;";
	

	if ( obj->mIsBold )
	{
		strStream << "font-weight:bold;";
	}
	QString font(strStream.str().c_str());

	//斜体
	strStream.clear();
	if (obj->mIsItalic)
	{
		strStream << "font-style:italic;";
	}
	QString italic(strStream.str().c_str());
	//加粗
	strStream.clear();
	if (obj->mIsBold)
	{
		strStream << "font-weight:bold;";
	}
	QString bold(strStream.str().c_str());

	//text
	strStream.str("");
	strStream << "<span style=\"position:absolute;" << color.toStdString() << margin.toStdString() << font.toStdString() << italic.toStdString() << bold.toStdString() << "\">" << obj->mStr.toStdString() << "</span>\n";
	this->data.append(QString(strStream.str().c_str()));
}

HTMLDocument *HTMLDocument::create(MyAppEntrance * widget)
{
	QList<htmlBaseObj *> list;
	widget->getHtml(&list);
	HTMLDocument *doc = new HTMLDocument(list);

 	QFile file("test10.html");
 	if (file.open(QFile::WriteOnly | QFile::Truncate))
 	{
 		QTextStream out(&file);
 		out.setCodec("UTF-8");
 		out << doc->data;
 		file.close();
 	}
	//压缩使用例子
	/*QString htmlStr = QString(doc->data);
	QByteArray desData;
	CompressTool::compressByteArray(doc->data,desData);
	QByteArray desData1;
	CompressTool::extractByteArray(desData, desData1);
	QString htmlStr1 = QString(desData1);*/
	deleteVectorContent<htmlBaseObj>(&list);
	return doc;
}
HTMLDocument * HTMLDocument::createAndCompressed(MyAppEntrance * widget) //获取的data 是经过压缩的
{
	HTMLDocument *doc = create(widget);
	QByteArray desData;
	CompressTool::compressByteArray(doc->data, desData);
	doc->data.clear();
	doc->data = desData;
	return doc;
}
void HTMLDocument:: createAndCompressed(MyAppEntrance * widget, QString &resultHtmlQString)
{
	HTMLDocument *doc = createAndCompressed(widget);
	resultHtmlQString = QString::fromUtf8(doc->data);
	delete doc;
	doc = NULL;
}
