#include "GameLayer.h"
#include "config.h"

USING_NS_CC;
const int mapRow = 8;
const int mapCol = 8;
const int iconType =7;
int mapConfig[mapCol][mapRow] = 
{
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
};
CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

bool GameLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GameIcon.plist","GameIcon.png");
    CCSprite* bg = CCSprite::create("game_bg.png");
	bg->setScale(visibleSize.height/bg->getContentSize().height);
    bg->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bg, 0);
	/*CCSprite* bg2 = CCSprite::create("game_bg_02.png");
	bg2->setAnchorPoint(ccp(0.5,1));
	bg2->setScale(0.7);
    bg2->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height + origin.y));
	this->addChild(bg2, 0);*/

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(GameLayer::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	row = mapRow;//4;//8;
	col = mapCol;//5;//7;
	iconSize = visibleSize.width/mapCol;
	offset = ccp((visibleSize.width-col*iconSize)/2+iconSize/2,(visibleSize.height-row*iconSize)/2-10);
	touchRect = CCRectMake(offset.x-iconSize/2,offset.y-iconSize/2,col*iconSize,row*iconSize);
	icons = CCArray::createWithCapacity(col*row);
	icons->retain();
	spares = CCArray::createWithCapacity(3);
	spares->retain();
	stables = CCArray::createWithCapacity(0);
	stables->retain();
	prompts = CCArray::createWithCapacity(3);
	prompts->retain();
	promptTime = 0;
	onMove = false;
	moveOnce = false;
	moveDuration = 0.3;
	validDis = 6;
	beginPos = CCPointZero;

	//surroundNum = 4;
	surround[0] = ccp(-1,0);
	surround[1] = ccp(0,1);
	surround[2] = ccp(1,0);
	surround[3] = ccp(0,-1);

	surround[4] = ccp(-1,1);
	surround[5] = ccp(1,1);
	surround[6] = ccp(1,-1);
	surround[7] = ccp(-1,-1);

	

	colFall = new int[col];
	memset(colFall,0,col*sizeof(int));
	selIcon = swapIcon = promptIcon = NULL;
	initGameBoard();
	readLevelFromData(0);
	if(checkAllCell()||noneToMove())
	{
		resetBoard();
	}

	this->scheduleUpdate();

    return true;
}
void GameLayer::readLevelFromData(int level)
{
	for(int i=0;i<col;i++)
	{
		for(int j = 0;j<row;j++)
		{
			mapConfig[i][j] = *(*(clevel+level)+i*row+j);//levels[i][j];
			printf("%d,",mapConfig[i][j]);
		}
		printf("\n");
	}
	resetBoard();
}
void GameLayer::update(float dt)
{
	if(onMove)
	{
		promptTime = 0;
		return;
	}
	promptTime = promptTime + dt;
	if(promptTime > 9)
	{
		
		promptTime = 0;
		/*if(!onMove)
		{*/
		if(!noneToMove()&&promptIcon)
		{
			prompts->removeAllObjects();
			GameIcon* iconptr;
			CCPoint temp = promptIcon->getCanMoveLoc();
			GameIcon* tempIcon = GameIcon::create(temp.x,temp.y,promptIcon->getID());
			checkCell(tempIcon);
			int up = tempIcon->up;
			int down = tempIcon->down;
			int left = tempIcon->left;
			int right = tempIcon->right;
			if((up+down)>=2)
			{
				while(up>0)
				{
					if(getIconByLoc(temp.x,temp.y+up))
					{
						prompts->addObject(getIconByLoc(temp.x,temp.y+up));
						up--;
					}else
					{
						break;
					}
				}
				while(down>0)
				{

					if(getIconByLoc(temp.x,temp.y-down))
					{
						prompts->addObject(getIconByLoc(temp.x,temp.y-down));
						down--;
					}else
					{
						break;
					}
				}
			}else if(left+right>=2)
			{
				while(left>0)
				{
					if(getIconByLoc(temp.x-left,temp.y))
					{
						prompts->addObject(getIconByLoc(temp.x-left,temp.y));
						left--;
					}else
					{
						break;
					}
				}
				while(right>0)
				{
					if(getIconByLoc(temp.x+right,temp.y))
					{
						prompts->addObject(getIconByLoc(temp.x+right,temp.y));
						right--;
					}else
					{
						break;
					}
				}
			}
			prompts->addObject(promptIcon);
				
			for(int i=0;i<prompts->count();i++)
			{
				iconptr = (GameIcon*)prompts->objectAtIndex(i);
				//iconptr->setColor(ccc3(128,0,0));
				iconptr->runAction(CCBlink::create(0.5,2));
			}
			CC_SAFE_RELEASE(tempIcon);
			promptIcon = NULL;
		}
	}
}
void GameLayer::initGameBoard()
{
	
	for(int i=0;i<col;i++)
	{
		for (int j=0;j<row;j++)
		{
			int id = rand()%iconType+1;
			GameIcon* icon = GameIcon::create(i,j,id*mapConfig[i][j]);
			icon->setPosition(correctPos(i,j));
			icon->retain();
			addChild(icon);
			icons->addObject(icon);
			if(id==0) 
				stables->addObject(icon);
		}
	}
	//resetBoard();
}
void GameLayer::resetBoard()
{
	GameIcon* icon;
	stables->removeAllObjects();
	for(int i=0;i<icons->count();i++)
	{
		icon = (GameIcon*)icons->objectAtIndex(i);
		if(mapConfig[icon->getX()][icon->getY()] == 0)
		{
			icon->setID(0);
			stables->addObject(icon);
		}
		else
 			icon->setID((rand()%iconType+1));
	}
	while(noneToMove()||checkAllCell())//noneToMove()||
	{
		resetBoard();
	}
}

CCPoint GameLayer::correctPos(int x,int y)
{
	CCPoint pos;
	pos.x = offset.x + x*iconSize;
	pos.y = offset.y + y*iconSize;
	return pos;
}
int GameLayer::getIdByPos(int x,int y)
{
	GameIcon* icon = getIconByLoc(x,y);
	if(icon)
	{
		return icon->getID();
	}else
	{
		return -1;
	}
}
GameIcon* GameLayer::getIconByPos(CCPoint pos)
{
	for(int i=0;i<icons->count();i++)
	{
		GameIcon* icon = (GameIcon*)icons->objectAtIndex(i);
		if (icon->boundingBox().containsPoint(pos))
			return icon;
	}
	return NULL;
}
GameIcon* GameLayer::getIconByLoc(int x,int y)
{
	if(x<0 ||y<0||x>col-1||y>row-1)
		return NULL;
	CCPoint pos = correctPos(x,y);	
	return getIconByPos(pos);
}
bool GameLayer::ccTouchBegan(CCTouch* ptouch,CCEvent* pevent)
{
	CCPoint pos = ptouch->getLocation();
	if(touchRect.containsPoint(pos))
	{
		if (onMove)
			return false;
		selIcon = getIconByPos(pos);
		/*if(selIcon)
			selIcon->setColor(ccc3(87,65,24));*/
		beginPos = pos;
	}
	
	return true;
}
void GameLayer::ccTouchEnded(CCTouch* ptouch,CCEvent* pevent)
{
	moveOnce = false;
}
void GameLayer::ccTouchMoved(CCTouch* ptouch,CCEvent* pevent)
{
	if(onMove || !selIcon || selIcon->isStable() || (beginPos.x==0&&beginPos.y==0)||moveOnce)
		return;
	CCPoint pos = ptouch->getLocation();
	//CCLOG("距离%f",ccpDistance(beginPos,pos));
	if(ccpDistance(beginPos,pos) > validDis)
	{
		onMove = true;
		moveOnce = true;
		int disx = beginPos.x-pos.x,disy = beginPos.y-pos.y;
		//CCLOG("距离%f,%f",disx,disy);
		if(abs(disx)>abs(disy))
		{
			if(disx>0)
			{
				changeCell(LEFT);
			}else
			{
				changeCell(RIGHT);
			}
		}else
		{
			if(disy>0)
			{
				changeCell(DOWN);
			}else
			{
				changeCell(UP);
			}
		}
	}
}
void GameLayer::setEffectIcons(GameIcon* icon)
{
	GameIcon* tempIcon;
	int id = icon->getID();
	int status = icon->getStatus();
	if(status==2)
	{
		
		CCPoint pos=ccp(icon->getX(),icon->getY());
		CCPoint temp;
		for(int i=0;i<8;i++)
		{
			temp = ccpAdd(pos,surround[i]);
			tempIcon = getIconByLoc(temp.x,temp.y);
			if(tempIcon)
			{
				if(!tempIcon->getBeRemove())
				{
					tempIcon->setBeRemove(true);
					setEffectIcons(tempIcon);
				}
			}
		}
	}else if(status==4)
	{
		for(int i=0;i<icons->count();i++)
		{
			tempIcon = (GameIcon*)icons->objectAtIndex(i);
			if(tempIcon->getID() == id)
			{
				if(!tempIcon->getBeRemove())
				{
					tempIcon->setBeRemove(true);
					setEffectIcons(tempIcon);
				}
			}
		}
	}
}
void GameLayer::onMoveEnd()
{
	if (checkAllCell())
	{
		GameIcon* icon;
		for(int i=0;i<icons->count();i++)
		{
			icon = (GameIcon*)icons->objectAtIndex(i);
			if(icon)
			{
				bool isSel = selIcon == icon || swapIcon == icon;
				if(checkCell(icon))
				{
					//获取受影响的icon
					setEffectIcons(icon);
					//selicon为空时默认选一个来实现特效
					if(icon->canbeRemove(isSel))
					{
						if(selIcon == NULL)
						{
							selIcon = icon;
							icon->canbeRemove(true);
						}
					}
				}
				
			}
		}
		for(int i=0;i<icons->count();i++)
		{
			icon = (GameIcon*)icons->objectAtIndex(i);
			if(icon->getBeRemove())
			{
				spares->addObject(icon);
			}
		}
		for(int i=0;i<spares->count();i++)
		{
			icon = (GameIcon*)spares->objectAtIndex(i);
			icons->removeObject(icon);
			icon->removeFromParentAndCleanup(true);
		}
		selIcon = swapIcon = NULL;
		fallDown();
	}else
	{
		if (selIcon && swapIcon)
		{
			swapTwoIcon(selIcon,swapIcon);
		}
		onMove = false;
	}

	selIcon = swapIcon = NULL;
}
void GameLayer::fallEnd(CCNode* obj)
{
	GameIcon* icon = dynamic_cast<GameIcon*>(obj);
	if(icon)
	{
		icon->fallNum = 0;
	}
}
void GameLayer::fallDown()
{
	int maxTime = 0;
	GameIcon* icon;
	GameIcon* tmpIcon;
	for(int i=0;i<spares->count();i++)
	{
		icon = (GameIcon*)spares->objectAtIndex(i);
		int tx = icon->getX();
		int ty = icon->getY();
		for(int j=0;j<row;j++)
		{
			tmpIcon = getIconByLoc(tx,j);
			if (tmpIcon && !tmpIcon->isStable() && tmpIcon->getY()>ty)
			{
				tmpIcon->fallNum++;
			}
		}

	}
	spares->removeAllObjects();
	
	for(int i=0;i<icons->count();i++)
	{
		icon = (GameIcon*)icons->objectAtIndex(i);
		int tx = icon->getX();
		int ty = icon->getY();
		int fallNum = icon->fallNum;
		if(fallNum>0)
		{
			for(int k=ty;k>ty-fallNum;k--)
			{
				tmpIcon = getIconByLoc(tx,k-1);
				while(getIconByLoc(tx,k-1) && getIconByLoc(tx,k-1)->isStable() && k>0)
				{
					k--;
					fallNum++;
				}
			}

			if (fallNum>maxTime)
			{
				maxTime=fallNum;
			}
			
			icon->setY(ty-fallNum);
			icon->updateUI();
			CCSequence *seq = CCSequence::createWithTwoActions(CCMoveTo::create(fallNum*moveDuration,correctPos(icon->getX(),icon->getY())),
				CCCallFuncN::create(this,SEL_CallFuncN(&GameLayer::fallEnd)));// ccp(0,-fallNum*iconSize)
			icon->runAction(seq);
		}
	}
	this->stopAllActions();
	this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(maxTime*moveDuration),
		CCCallFunc::create(this,callfunc_selector(GameLayer::checkOnFallEnd))));
}
void GameLayer::checkOnFallEnd()
{
	if (checkAllCell())
	{
		onMoveEnd();
	}else
	{
		fillIcons();
	}
}

void GameLayer::changeCell(MoveDirect direct)
{
	int x = selIcon->getX(),y = selIcon->getY();
	int sx = x,sy = y;
	switch (direct)
	{
	case GameLayer::LEFT:
		sx-=1;
		break;
	case GameLayer::RIGHT:
		sx+=1;
		break;
	case GameLayer::UP:
		sy+=1;
		break;
	case GameLayer::DOWN:
		sy-=1;
		break;
	default:
		break;
	}
	//CCLog("%d,%d 交换位置是%d,%d",x,y,sx,sy);
	if(sx<0 ||sy<0||sx>col-1||sy>row-1)
	{
		onMoveEnd();
	}else
	{
		swapIcon = getIconByLoc(sx,sy);
		swapTwoIcon(selIcon,swapIcon);
	}
}
void GameLayer::swapTwoIcon(GameIcon* icon1,GameIcon* icon2,bool animate /* = false */)
{
	//swapIcon->setColor(ccc3(255,255,255));
	if (!icon1 || !icon2 || (icon1&&icon1->isStable()) || (icon2&&icon2->isStable()))
	{
		//selIcon = swapIcon = NULL;
		//onMoveEnd();
		onMove = false;
		return;
	}
	icon1->stopAllActions();
	icon2->stopAllActions();
	int x = icon1->getX(),y = icon1->getY(),sx = icon2->getX(),sy = icon2->getY();
	icon1->setX(sx);
	icon1->setY(sy);
	icon2->setX(x);
	icon2->setY(y);
	CCMoveTo* move1 = CCMoveTo::create(moveDuration,correctPos(x,y));
	CCMoveTo* move2 = CCMoveTo::create(moveDuration,correctPos(sx,sy));
	icon2->runAction(move1);
	CCSequence *seq = CCSequence::createWithTwoActions(move2,CCCallFunc::create(this,SEL_CallFunc(&GameLayer::onMoveEnd)));
	icon1->runAction(seq); 
}
bool GameLayer::checkAllCell()
{
	bool canbecrash=false;
	GameIcon* icon;
	for(int i=0;i<icons->count();i++)
	{
		icon = (GameIcon*)icons->objectAtIndex(i);
		if(icon&&!icon->isStable()&&checkCell(icon))
		{
			return true;
		}
	}
	
	return false;
}
bool GameLayer::checkCell(GameIcon * icon)
{
	//CCPoint pos = ccp(icon->getX(),icon->getY());
	if(icon->isStable())
		return false;
	int x = icon->getX(),y = icon->getY();
	int id = icon->getID();
	int tx=x,ty=y;
	int tid = 1;
	GameIcon* ticon;
	//每次判断前重置防止叠加
	icon->left = icon->right = icon->up = icon->down = 0;	
	//判断竖向
	while(tid>0)
	{
		tid = getIdByPos(tx,--ty);
		if(id == tid)
			icon->down++;
		else
			break;
	}
	tid = 1;
	tx=x,ty=y;
	while(tid>0)
	{
		++ty;
		//CCLog("(%d,%d)",tx,ty);
		tid = getIdByPos(tx,ty);
		if(id == tid)
			icon->up++;
		else
			break;
	}
	
	//横向
	tx = x,ty = y;
	tid = 1;
	while(tid>0)
	{
		tid = getIdByPos(--tx,ty);
		if(id == tid)
			icon->left++;
		else
			break;
	}
	tid = 1;
	tx=x,ty=y;
	while(tid>0)
	{
		tid = getIdByPos(++tx,ty);
		if(id == tid)
			icon->right++;
		else
			break;
	}
	
	//CCLOG("pos (%d,%d) id is %d ,left %d  right %d  up %d  down %d",x,y,id,icon->left,icon->right,icon->up,icon->down);
	if((icon->left+icon->right)>1 || (icon->up+icon->down)>1)
	{
		return true;
	}
	return false;
}

void GameLayer::fillIcons()
{
	int maxTime = 0;
	for(int i=0;i<col;i++)
	{
		for(int j=0;j<row;j++)
		{
			if(!getIconByLoc(i,j))
			{
				++colFall[i];
			}
		}
		for(int j=0;j<row;j++)
		{
			if(!getIconByLoc(i,j))
			{
				int dis = colFall[i];
				if(maxTime<dis)
					maxTime = dis;
				int targety = j;
				GameIcon* icon = GameIcon::create(i,targety,(rand()%iconType+1));
				icon->setPosition(correctPos(i,targety+dis));
				this->addChild(icon);
				icons->addObject(icon);
				icon->runAction(CCMoveTo::create((dis)*moveDuration,correctPos(icon->getX(),icon->getY())));
			}
		}
	}
	for(int i=0;i<col;i++)
	{
		if(colFall[i]>0)
		{
			this->stopAllActions();
			this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(maxTime*moveDuration),
				CCCallFunc::create(this,callfunc_selector(GameLayer::checkOnFallEnd))));
			onMove = true;
			break;
		}
		onMove = false;
	}
	memset(colFall,0,col*sizeof(int));
	if (!onMove&&noneToMove())
	{
		resetBoard();
	}
	
}
bool GameLayer::canbeMove(GameIcon* icon)
{
	CCPoint pos = ccp(icon->getX(),icon->getY());
	int id = icon->getID();
	int tid;
	CCPoint temp;
	GameIcon* ticon;
	for (int i=0;i<4;i++)
	{
		temp = ccpAdd(pos,surround[i]);
		ticon = getIconByLoc(temp.x,temp.y);
		if (ticon && !ticon->isStable())
		{
			tid = ticon->getID();
			ticon->setID(id,false);
			icon->setID(tid,false);
			if (checkCell(ticon))
			{
				ticon->setID(tid,false);
				icon->setID(id,false);
				icon->setCanMoveLoc(temp);
				return true;
			}
			ticon->setID(tid,false);
			icon->setID(id,false);
			icon->setCanMoveLoc(ccp(-1,-1));
		}
	}
	return false;
}
bool GameLayer::noneToMove()
{
	GameIcon* icon;
	for(int i=0;i<icons->count();i++)
	{
		icon = (GameIcon*)icons->objectAtIndex(i);
		if (!icon->isStable() &&canbeMove(icon))
		{
			promptIcon = icon;
			return false;
		}
	}
	return true;
}
void GameLayer::menuCloseCallback(CCObject* pSender)
{
	static int glevel = 0;
	glevel = (++glevel)%4;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    //CCDirector::sharedDirector()->end();
	readLevelFromData(glevel);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}