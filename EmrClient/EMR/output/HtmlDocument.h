#ifndef HTMLDOCUMENT_H
#define HTMLDOCUMENT_H
 
#include <QObject>
#include <QByteArray>
#include <QColor>

#include "htmlBaseObj.h"
#include "MyAppEntrance.h"

class ViewCellParams;

class HTMLDocument : public QObject
{
    Q_OBJECT
public:
	explicit HTMLDocument(QList<htmlBaseObj *> &list);
	HTMLDocument(MyAppEntrance * widget);

    //html节点
    void startHTML();
    void endHTML();

    //head节点
    void startHead();
    void endHead();

    //title节点
    void insertTitle(QString title = "MedicalRecord");

    //style节点 html默认字体大小12px
    void insertStyle(int fontSize = 12);

    //body节点
    void startBody();
    void endBody();

    //insert line
	void insertLine(htmlBaseObj *);
    //insert image
	void insertImage(htmlBaseObj *);
    //insert text
	void insertText(htmlBaseObj *);

	static  HTMLDocument * create(MyAppEntrance * widget);//以前用的接口。数据未压缩
	static  HTMLDocument * createAndCompressed(MyAppEntrance * widget); //获取的data 是经过压缩的
	static  void createAndCompressed(MyAppEntrance * widget,QString &resultHtmlQString); //获取压缩过得Html字符串
signals:

public slots:

public:
    QByteArray data;
};

#endif // HTMLDOCUMENT_H
