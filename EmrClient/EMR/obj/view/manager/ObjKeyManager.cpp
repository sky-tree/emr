#include "ObjKeyManager.h"


#include "BaseKey.h"
#include "ViewObj.h"
#include "qdebug.h"
#include "ConstantsID.h"
#include "BaseConstants.h"

ObjKeyManager::ObjKeyManager(ViewObj * view) : BaseContainerParam(view, ConstantsID::ClassId::OBJ_KEY_MANAGER)
, mKey(NULL)
{
}
ObjKeyManager::ObjKeyManager(ViewObj * view, int classId) : BaseContainerParam(view, classId)
{
	mKey = NULL;
}
ObjKeyManager::ObjKeyManager(ViewObj * view, ObjKeyManager * source) : BaseContainerParam(view, source->mClassID)
{
	mKey = NULL;
}
ObjKeyManager::~ObjKeyManager()
{
	if (mKey)
	{
		delete mKey;
		mKey = NULL;
	}
}
void ObjKeyManager::setKey(BaseKey* key)
{
	if (mKey)
	{
		delete mKey;
	}
	mKey = key;
}
int ObjKeyManager::keyPress(QString str)
{
	int returnType = BaseConstants::NONE;
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			if (child->isObj(ConstantsID::ContainerId::TMPLATE_TXT))
			{
				int a = 0;
			}
			returnType = child->getKeyMgr()->keyPress(str);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	if (mKey)
	{
		return mKey->keyPress(str);
	}
	//if (mViewChildren)
	//{
	//	int size = mViewChildren->size();
	//	// 倒序执行
	//	for (int i = size - 1; i >= 0; i--)
	//	{
	//		ViewObj * view = mViewChildren->at(i);
	//		returnType = view->keyPress(str);
	//		// 如果某个子对象逻辑执行后需要中断后续逻辑，则返回
	//		if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
	//			return returnType;
	//	}
	//}
	//bool needBlock = isBlock(BaseConstants::BlockType::ONLY_KEY);
	//if (!mInputManager)
	//{
	//	if (needBlock)
	//		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	//	return returnType;
	//}
	//returnType = mInputManager->keyPress(str);
	//if (returnType > BaseConstants::NONE)
	//	return returnType;
	//if (needBlock)
	//	return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	return returnType;
}
int ObjKeyManager::keyRelease(QString str)
{
	int returnType = BaseConstants::NONE;
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			returnType = child->getKeyMgr()->keyRelease(str);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	if (mKey)
	{
		return mKey->keyRelease(str);
	}
	//if (mViewChildren)
	//{
	//	int size = mViewChildren->size();
	//	// 倒序执行
	//	for (int i = size - 1; i >= 0; i--)
	//	{
	//		ViewObj * view = mViewChildren->at(i);
	//		returnType = view->keyRelease(str);
	//		// 如果某个子对象逻辑执行后需要中断后续逻辑，则返回
	//		if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
	//			return returnType;
	//	}
	//}
	//bool needBlock = isBlock(BaseConstants::BlockType::ONLY_KEY);
	//if (!mInputManager)
	//{
	//	if (needBlock)
	//		return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	//	return returnType;
	//}
	//returnType = mInputManager->keyRelease(str);
	//if (returnType > BaseConstants::NONE)
	//	return returnType;
	//if (needBlock)
	//	return BaseConstants::ReturnType::RETURN_ONE_LEVEL;
	return returnType;
}