#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "../proj.win32/Animal.h"
class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
	virtual bool ccTouchBegan(CCTouch* ptouch,CCEvent* pevent);
	virtual void ccTouchMoved(CCTouch* ptouch,CCEvent* pevent);
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
	CCMotionStreak* streak;
};

#endif // __HELLOWORLD_SCENE_H__
