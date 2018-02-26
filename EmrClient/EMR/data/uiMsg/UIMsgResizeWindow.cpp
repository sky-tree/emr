#include "UIMsgResizeWindow.h"




#include "WriterManager.h"
#include "OperateMgr.h"

#include "ViewObj.h"



#include "BaseViewParam.h"


#include "BaseLayout.h"

#include "ConstantsID.h"
#include "BaseConstants.h"

UIMsgResizeWindow::UIMsgResizeWindow(WriterManager * writerMgr, QSize size) : BaseUIMsgData(writerMgr, ConstantsID::ClassId::UI_DATA_RESIZE_WINDOW)
, mSizeParam(size)
{
}

UIMsgResizeWindow::~UIMsgResizeWindow()
{
}
void UIMsgResizeWindow::doAction()
{
	mOperateMgr->resizeWindow(mSizeParam.width(), mSizeParam.height());
}