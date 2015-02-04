#include "GameIcon.h"
USING_NS_CC;
GameIcon::GameIcon(int tx,int ty,int tid,int tstatus):x(tx),y(ty),id(tid),status(tstatus),istable(false),beRemove(false),canMoveLoc(ccp(-1,-1)),properties(NULL)
{
	left = right = up = down = fallNum = 0;
}
GameIcon* GameIcon::create(int x,int y,int id,int status)
{

	GameIcon* icon = new GameIcon(x,y,id,status);
	int tid = id;
	if (tid == 0)
	{
		tid = 1;
	}
	CCString * name = CCString::createWithFormat("icon%d%d.png",status,tid);
	if (icon && icon->initWithSpriteFrameName(name->getCString()))
	{
		icon->setScale(0.6);
		icon->autorelease();
		icon->updateUI();
		return icon;
	}
	CC_SAFE_DELETE(icon);
	return NULL;
}

void GameIcon::setID(int cid,bool change)
{
	id = cid;
	if (change)
	{
		updateUI();
	}
}
void GameIcon::setStatus(int sstatus,bool change)
{
	status = sstatus;
	if (change)
	{
		updateUI();
	}
}
void GameIcon::updateUI()
{
	if(properties == NULL)
	{
		CCString* lbl = CCString::createWithFormat("x:%d,y:%d,id:%d",x,y,id);
		properties = CCLabelTTF::create(lbl->getCString(),"arial",20);
		properties->setAnchorPoint(CCPointZero);
		lbl->release();
		properties->retain();
		addChild(properties);
	}
	else
	{
		CCString* lbl = CCString::createWithFormat("x:%d,y:%d,id:%d",x,y,id);
		properties->setString(lbl->getCString());
		lbl->release();
	}
	if (id == 0)
	{
		this->setVisible(false);
		istable = true;
		return;
	}
	else
	{
		this->setVisible(true);
		istable = false;
	}
	CCString * name = CCString::createWithFormat("icon%d%d.png",status,id);
	CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
	this->setDisplayFrame(frame);
}
bool GameIcon::canbeRemove(bool isSelIcon)
{
	if((left+right)>1 || (up+down)>1)
	{
		checkStatus(isSelIcon);
		return true;
	}
	return false;
}

bool GameIcon::checkStatus(bool isSelIcon)
{
	if((left == right && left == 2) ||(up == down && up == 2))
	{
		setStatus(2);
		beRemove = false;
		return false;
	}else if((left+right > 2 || up+down > 2)&&isSelIcon)
	{
		setStatus(4);
		beRemove = false;
		return false;
	}
	beRemove = true;
	return true;
}