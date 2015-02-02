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
	cocos2d::CCArray* icons;//��ʼ����icon����
	cocos2d::CCArray* spares;//��������icon����
	cocos2d::CCArray* stables;//�����ƶ�����
	cocos2d::CCArray* prompts;//��ʾ���ƶ�����
	int col;//�� x
	int row;//�� y
	cocos2d::CCPoint offset;
	int iconSize;

	GameIcon* selIcon;//���ѡ�е�icon
	GameIcon* swapIcon;//������icon
	float promptTime;
	GameIcon* promptIcon;//10�����Ҳ�������ʾ���ƶ��Ľڵ�

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