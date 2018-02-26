#include "TempleteViewObjKeyManager.h"
#include "ConstantsID.h"
#include "BaseKey.h"
#include "ViewObj.h"
#include "qdebug.h"
#include "BaseConstants.h"
#include "BaseTemplate.h"
TempleteViewObjKeyManager::TempleteViewObjKeyManager(ViewObj * view) :ObjKeyManager(view, ConstantsID::ClassId::TEMP_VIEWOBJ_KEY_MANAGER)
, mKey(NULL)
{
}


TempleteViewObjKeyManager::~TempleteViewObjKeyManager()
{
	if (mKey)
	{
		delete mKey;
		mKey = NULL;
	}
}
void TempleteViewObjKeyManager::setKey(BaseKey* key)
{
	if (mKey)
	{
		delete mKey;
	}
	mKey = key;
}
int TempleteViewObjKeyManager::keyPress(QString str)
{
	int returnType = BaseConstants::NONE;
	if (mOwner->canEditTemplete())
	{
		if (mKey)
		{
			returnType = mKey->keyPress(str);
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}
		BaseContainer * tempParent = mOwner->getTempParent();
		if (tempParent)
		{
			if (tempParent->isTemplateElement())
			{
				returnType = ((BaseTemplate*)tempParent)->keyPress(str, mOwner);
			}
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}
	}
	
	if (mOwner->haveChildren() > 0)
	{
		QList<BaseContainer *> * children = mOwner->getChildren();
		int start = children->size() - 1;
		ViewObj * child = NULL;
		for (int i = start; i >= 0; i--)
		{
			child = (ViewObj *)children->at(i);
			returnType = child->getKeyMgr()->keyPress(str);
			if (returnType >= BaseConstants::ReturnType::RETURN_ONE_LEVEL)
			{
				return returnType;
			}
		}
	}
	
	return returnType;
}
int TempleteViewObjKeyManager::keyRelease(QString str)
{
	int returnType = BaseConstants::NONE;
	if (mOwner->canEditTemplete())
	{
		if (mKey)
		{
			returnType = mKey->keyRelease(str);
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}

		BaseContainer * tempParent = mOwner->getTempParent();
		if (tempParent)
		{
			if (tempParent->isTemplateElement())
			{
				returnType = ((BaseTemplate*)tempParent)->keyPress(str, mOwner);
			}
			if (returnType > BaseConstants::NONE)
			{
				return returnType;
			}
		}
	}
	
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
	
	return returnType;
}