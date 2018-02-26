#ifndef _NOCOPYTEXTWIDGET_H_
#define _NOCOPYTEXTWIDGET_H_
#include "TextWidget.h"
class NoCopyTextWidget :
	public TextWidget
{
public:
	NoCopyTextWidget(WriterManager * mgr, BaseContainer * templeteParent, int objId, bool isCreateStrTxtData = false);
	NoCopyTextWidget(WriterManager * mgr, NoCopyTextWidget * otherStructureation);
	bool setTempleteProperty(QMap<QString, QString>& setMap);
};
#endif