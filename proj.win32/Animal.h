#pragma once
#include "cocos2d.h"
#include "BasicEle.h"
USING_NS_CC;
class Animal :
	public CCNode
{
public:
	enum AimalType{
		FLYS,
		BEATS,
	};
	static Animal* create(AimalType type);
	bool initBody(AimalType type);
	bool initLimps();
	bool initSwings();
	void run();
	void jump();
private:
	BasicEle* body;
	BasicEle* leftarm;
	BasicEle* rightarm;
	BasicEle* leftleg;
	BasicEle* rightleg;
	BasicEle* leftswing;
	BasicEle* rightswing;

};

