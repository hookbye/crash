#pragma once
#include "cocos2d.h"
class GameIcon :
	public cocos2d::CCSprite
{
public:
	GameIcon(int x = 0,int y=0,int id = 1,int status = 1);
	static GameIcon* create(int x = 0,int y=0,int id = 1,int status = 1);

	void updateUI();
	int getStatus(){return status;}
	void setStatus(int sstatus,bool change = true);
	int getID(){return id;}
	void setID(int cid,bool change = true);
	CC_SYNTHESIZE(int,x,X);
	CC_SYNTHESIZE(int,y,Y);
	CC_SYNTHESIZE(bool,beRemove,BeRemove);
	CC_SYNTHESIZE(cocos2d::CCPoint,canMoveLoc,CanMoveLoc);

	bool canbeRemove(bool isSelIcon = false);
	bool checkStatus(bool isSelIcon = false);
public:	
	int left;
	int right;
	int up;
	int down;
	int fallNum;
	bool isStable(){return istable;}
protected:
	int id;
	int status;
	bool istable;
	cocos2d::CCLabelTTF* properties;
};

