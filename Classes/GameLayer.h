#pragma once
#include "cocos2d.h"
#include "GameIcon.h"
class GameLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
	virtual bool ccTouchBegan(cocos2d::CCTouch* ptouch,cocos2d::CCEvent* pevent);
	virtual void ccTouchMoved(cocos2d::CCTouch* ptouch,cocos2d::CCEvent* pevent);
    virtual void ccTouchEnded(cocos2d::CCTouch* ptouch,cocos2d::CCEvent* pevent);
    void menuCloseCallback(CCObject* pSender);
    
    CREATE_FUNC(GameLayer);

	void initGameBoard();
	void readLevelFromData(int level = 0);
	cocos2d::CCPoint correctPos(int x,int y);
	int getIdByPos(int x,int y);
	GameIcon* getIconByPos(cocos2d::CCPoint pos);
	GameIcon* getIconByLoc(int x,int y);

	enum MoveDirect
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};
	void onMoveEnd();
	void changeCell(MoveDirect direct);
	void swapTwoIcon(GameIcon* icon1,GameIcon* icon2,bool animate = false);
	bool checkCell(GameIcon* icon);
	bool checkAllCell();

	void resetBoard();
	void fallDown();
	void fallEnd(cocos2d::CCNode* obj);
	void checkOnFallEnd();

	void fillIcons();
	bool canbeMove(GameIcon* icon);
	bool noneToMove();

	void update(float dt);
private:
	cocos2d::CCArray* icons;//初始所有icon集合
	cocos2d::CCArray* spares;//可消除的icon集合
	cocos2d::CCArray* stables;//不可移动集合
	cocos2d::CCArray* prompts;//提示可移动集合
	int col;//列 x
	int row;//行 y
	cocos2d::CCPoint offset;
	int iconSize;

	GameIcon* selIcon;//点击选中的icon
	GameIcon* swapIcon;//交换的icon
	float promptTime;
	GameIcon* promptIcon;//10秒内找不到，提示可移动的节点

	bool onMove;
	bool moveOnce;
	bool filling;
	float moveDuration;
	int validDis;
	cocos2d::CCPoint beginPos;
	cocos2d::CCRect touchRect;
	int *colFall;

	int *icontmap;

	cocos2d::CCPoint surround[8];

	
};